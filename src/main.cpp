/*
 * main.cpp  –  CLI Entry Point cho FinanceApp
 *
 * Cach dung:
 *   FinanceApp.exe get-data
 *   FinanceApp.exe add-account <name> <balance>
 *   FinanceApp.exe add-transaction <type> <date> <amount> <category> <account> <desc> [freq] [dueDay]
 *   FinanceApp.exe set-budget <category> <limit>
 *   FinanceApp.exe simulate-recurring <year> <month>
 *   FinanceApp.exe preview-recurring <year> <month>
 *   FinanceApp.exe load-sample
 *
 * Moi lenh doc/ghi CSV theo nhu cau va in ket qua JSON ra stdout.
 * Streamlit goi qua subprocess va doc JSON tu stdout.
 */

#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "FinanceManager.h"

using namespace std;

static const string DATA_FILE = "data/finance_data.csv";

// In loi dang JSON
static void errJson(const string& message) {
    cout << "{\"ok\":false,\"message\":\"" << message << "\"}" << endl;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Khong co lenh -> in huong dan
    if (argc < 2) {
        cerr << "Finance App CLI\n"
             << "  get-data\n"
             << "  add-account <name> <balance>\n"
             << "  add-transaction <type> <date> <amount> <category> <account> <desc> [freq] [dueDay]\n"
             << "  set-budget <category> <limit>\n"
             << "  simulate-recurring <year> <month>\n"
             << "  preview-recurring <year> <month>\n"
             << "  load-sample (doc data/finance_data_fixed.csv va ghi de du lieu hien tai)\n";
        return 1;
    }

    string command = argv[1];

    // ──────────────────────────────────────────────
    //  get-data  ->  tra ve JSON toan bo du lieu
    // ──────────────────────────────────────────────
    if (command == "get-data") {
        FinanceManager manager;
        manager.loadData(DATA_FILE);
        cout << manager.toJson() << endl;
        return 0;
    }

    // ──────────────────────────────────────────────
    //  add-account <name> <balance>
    // ──────────────────────────────────────────────
    if (command == "add-account") {
        if (argc < 4) { errJson("Thieu tham so: add-account <name> <balance>"); return 1; }
        string name = argv[2];
        double balance = atof(argv[3]);

        FinanceManager manager;
        manager.loadData(DATA_FILE);
        manager.addAccount(name, balance);
        manager.saveData(DATA_FILE);

        cout << manager.toJson() << endl;
        return 0;
    }

    // ──────────────────────────────────────────────
    //  add-transaction <type> <date> <amount> <category> <account> <desc> [freq] [dueDay]
    // ──────────────────────────────────────────────
    if (command == "add-transaction") {
        if (argc < 9) {
            errJson("Thieu tham so: add-transaction type date amount category account desc [freq] [dueDay]");
            return 1;
        }
        string type     = argv[2];
        string date     = argv[3];
        double amount   = atof(argv[4]);
        string category = argv[5];
        string account  = argv[6];
        string desc     = argv[7];
        string freq     = (argc > 8) ? argv[8] : "Monthly";
        int dueDay      = (argc > 9) ? atoi(argv[9]) : 1;

        FinanceManager manager;
        manager.loadData(DATA_FILE);

        // Kiem tra canh bao ngan sach truoc khi them
        int txYear  = 0;
        int txMonth = 0;
        if ((int)date.size() >= 7) {
            txYear  = atoi(date.substr(0, 4).c_str());
            txMonth = atoi(date.substr(5, 2).c_str());
        }
        string alertJson = "";
        if (type != "Income") {
            alertJson = manager.budgetAlertJson(category, txMonth, txYear, amount);
        }

        int txId = manager.generateNextId();
        bool ok = false;

        if (type == "Income") {
            ok = manager.addTransaction(make_unique<Income>(txId, date, amount, category, account, desc));
        } else if (type == "Expense") {
            ok = manager.addTransaction(make_unique<Expense>(txId, date, amount, category, account, desc));
        } else if (type == "RecurringExpense") {
            ok = manager.addTransaction(make_unique<RecurringExpense>(txId, date, amount, category, account, desc, freq, dueDay));
        } else {
            errJson("Loai giao dich khong hop le: " + type);
            return 1;
        }

        if (!ok) {
            errJson("Giao dich bi tu choi (co the khong du so du hoac tai khoan khong ton tai)");
            return 1;
        }

        manager.saveData(DATA_FILE);

        // Tra ve du lieu moi + canh bao ngan sach
        stringstream result;
        result << "{\"ok\":true,\"budget_alert\":" << (alertJson.empty() ? "null" : alertJson)
               << ",\"data\":" << manager.toJson() << "}";
        cout << result.str() << endl;
        return 0;
    }

    // ──────────────────────────────────────────────
    //  set-budget <category> <limit>
    // ──────────────────────────────────────────────
    if (command == "set-budget") {
        if (argc < 4) { errJson("Thieu tham so: set-budget <category> <limit>"); return 1; }
        string category = argv[2];
        double limit    = atof(argv[3]);

        FinanceManager manager;
        manager.loadData(DATA_FILE);
        manager.setBudgetLimit(category, limit);
        manager.saveData(DATA_FILE);

        cout << manager.toJson() << endl;
        return 0;
    }

    // ──────────────────────────────────────────────
    //  preview-recurring / simulate-recurring <year> <month>
    // ──────────────────────────────────────────────
    if (command == "simulate-recurring" || command == "preview-recurring") {
        if (argc < 4) { errJson("Thieu tham so: simulate-recurring <year> <month>"); return 1; }
        int year  = atoi(argv[2]);
        int month = atoi(argv[3]);
        if (year < 1 || month < 1 || month > 12) {
            errJson("Thang hoac nam khong hop le");
            return 1;
        }

        FinanceManager manager;
        manager.loadData(DATA_FILE);
        int count = manager.executeRecurringExpensesForMonth(year, month);
        if (command == "simulate-recurring" && !manager.saveData(DATA_FILE)) {
            errJson("Khong the luu du lieu mo phong");
            return 1;
        }

        stringstream result;
        result << "{\"ok\":true,\"applied\":" << count
               << ",\"data\":" << manager.toJson() << "}";
        cout << result.str() << endl;
        return 0;
    }

    // ──────────────────────────────────────────────
    //  load-sample  ->  nap CSV demo vao file du lieu dang dung
    // ──────────────────────────────────────────────
    if (command == "load-sample") {
        FinanceManager manager;
        if (!manager.loadSampleData()) {
            errJson("Khong the doc data/finance_data_fixed.csv");
            return 1;
        }
        if (!manager.saveData(DATA_FILE)) {
            errJson("Khong the luu du lieu mau");
            return 1;
        }
        cout << manager.toJson() << endl;
        return 0;
    }

    // Lenh khong nhan ra
    errJson("Lenh khong hop le: " + command);
    return 1;
}
