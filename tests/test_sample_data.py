import json
import subprocess
import tempfile
import unittest
from pathlib import Path


EXECUTABLE = Path(__file__).resolve().parents[1] / "bin" / "FinanceApp.exe"


class SampleDataTests(unittest.TestCase):
    def setUp(self):
        if not EXECUTABLE.exists():
            self.skipTest("FinanceApp.exe has not been built")

    def test_load_sample_reads_csv_and_replaces_current_data(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
            data_dir = Path(directory) / "data"
            data_dir.mkdir()
            (data_dir / "finance_data_fixed.csv").write_text(
                "#ACCOUNTS\nName,Balance\nDemo,123.00\n"
                "#BUDGETS\nCategory,Limit\nFood,50.00\n"
                "#TRANSACTIONS\n"
                "Type,Id,Date,Amount,Category,Account,Description,Frequency,DueDay\n"
                "Income,7,2026-10-01,5.00,Salary,Demo,Pay,,\n",
                encoding="utf-8",
            )
            data_file = data_dir / "finance_data.csv"
            data_file.write_bytes(b"existing data")

            result = subprocess.run(
                [str(EXECUTABLE), "load-sample"], cwd=directory,
                capture_output=True, text=True, encoding="utf-8", check=True,
            )
            loaded = json.loads(result.stdout)
            self.assertEqual(loaded["accounts"], [{"name": "Demo", "balance": 123}])
            self.assertEqual(loaded["budgets"], {"Food": 50})
            self.assertEqual(loaded["transactions"][0]["id"], 7)
            self.assertNotEqual(data_file.read_bytes(), b"existing data")

    def test_missing_sample_does_not_replace_existing_data(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
            data_file = Path(directory) / "data" / "finance_data.csv"
            data_file.parent.mkdir()
            data_file.write_bytes(b"existing data")

            result = subprocess.run(
                [str(EXECUTABLE), "load-sample"], cwd=directory,
                capture_output=True, text=True, encoding="utf-8",
            )
            self.assertNotEqual(result.returncode, 0)
            self.assertFalse(json.loads(result.stdout)["ok"])
            self.assertEqual(data_file.read_bytes(), b"existing data")


if __name__ == "__main__":
    unittest.main()
