import json
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from finance_data import import_finance_csv, validate_finance_csv


VALID_CSV = (
    "#ACCOUNTS\nName,Balance\nVí,1000.00\n"
    "#BUDGETS\nCategory,Limit\nĂn uống,500.00\n"
    "#TRANSACTIONS\n"
    "Type,Id,Date,Amount,Category,Account,Description,Frequency,DueDay\n"
    "Expense,1,2026-10-01,100.00,Ăn uống,Ví,Bữa sáng,,1\n"
).encode("utf-8")


class FinanceImportTests(unittest.TestCase):
    def test_accepts_export_format_and_utf8_bom(self):
        counts, normalized = validate_finance_csv(b"\xef\xbb\xbf" + VALID_CSV)
        self.assertEqual(counts, {"accounts": 1, "budgets": 1, "transactions": 1})
        self.assertEqual(normalized, VALID_CSV)

    def test_rejects_rows_that_cpp_would_silently_skip(self):
        invalid_files = (
            VALID_CSV.replace(b"100.00", b"not-a-number"),
            VALID_CSV.replace("Bữa sáng".encode(), "Bữa,sáng".encode()),
            VALID_CSV.replace(b"2026-10-01", b"2026-02-30"),
            VALID_CSV.replace(b",V\xc3\xad,", b",Kh\xc3\xa1c,"),
        )
        for content in invalid_files:
            with self.subTest(content=content):
                with self.assertRaises(ValueError):
                    validate_finance_csv(content)

    def test_import_keeps_a_backup(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
            data_file = Path(directory) / "finance_data.csv"
            data_file.write_bytes(b"old data")
            loaded = {"accounts": [{}], "budgets": {"food": 100}, "transactions": [{}]}
            with patch("finance_data.get_data", return_value=loaded):
                result, backup = import_finance_csv(VALID_CSV, data_file)
            self.assertEqual(result, loaded)
            self.assertEqual(data_file.read_bytes(), VALID_CSV)
            self.assertEqual(backup.read_bytes(), b"old data")

    def test_failed_import_restores_original(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
            data_file = Path(directory) / "finance_data.csv"
            data_file.write_bytes(b"old data")
            with patch("finance_data.get_data", side_effect=RuntimeError("read failed")):
                with self.assertRaises(RuntimeError):
                    import_finance_csv(VALID_CSV, data_file)
            self.assertEqual(data_file.read_bytes(), b"old data")
            self.assertEqual(list(Path(directory).glob("*.backup-*.csv")), [])

    def test_import_is_readable_by_cpp_backend(self):
        executable = Path(__file__).resolve().parents[1] / "bin" / "FinanceApp.exe"
        if not executable.exists():
            self.skipTest("FinanceApp.exe has not been built")
        with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
            data_file = Path(directory) / "data" / "finance_data.csv"

            def native_data():
                result = subprocess.run(
                    [str(executable), "get-data"], cwd=directory,
                    capture_output=True, text=True, encoding="utf-8", check=True,
                )
                return json.loads(result.stdout.splitlines()[-1])

            with patch("finance_data.get_data", side_effect=native_data):
                loaded, backup = import_finance_csv(VALID_CSV, data_file)
            self.assertIsNone(backup)
            self.assertEqual(loaded["accounts"][0]["name"], "Ví")
            self.assertEqual(loaded["transactions"][0]["description"], "Bữa sáng")


if __name__ == "__main__":
    unittest.main()
