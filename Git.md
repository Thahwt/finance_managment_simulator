# 🛠️ Hướng Dẫn Git Cho Nhóm 7 Người (Đẩy Trực Tiếp Lên Main)

Tài liệu này hướng dẫn cách 7 thành viên cùng làm việc và đẩy mã nguồn trực tiếp lên nhánh **`main`** mà **không cần tạo branch** hay tạo Pull Request phức tạp.

---

## 📋 1. Bảng Phân Chia File Cho 7 Thành Viên

| Thành Viên | Module Phụ Trách | Các File Cần Copy & Push |
|:---:|---|---|
| **Người 1** *(Leader)* | **Khởi tạo dự án & Nền tảng** | `.gitignore`, `README.md`, `build.bat`, `run_test.bat`, `data/finance_data.csv`, `include/Common.h` |
| **Người 2** | **Quản lý Tài khoản (Account)** | `include/Account.h`, `src/Account.cpp` |
| **Người 3** | **Quản lý Ngân sách (Budget)** | `include/Budget.h`, `src/Budget.cpp` |
| **Người 4** | **Hệ thống Giao dịch (Transaction)** | `include/Transaction.h`, `src/Transaction.cpp` |
| **Người 5** | **Đọc / Ghi File CSV (Storage)** | `include/FinanceStorage.h`, `src/FinanceStorage.cpp` |
| **Người 6** | **Bộ Điều Phối Trung Tâm (Manager)** | `include/FinanceManager.h`, `src/FinanceManager.cpp` |
| **Người 7** | **Giao diện Menu & Bộ Kiểm thử** | `src/main.cpp`, `tests/test_finance.cpp` |

---

## 🎯 2. Quy Tắc Vàng Để Không Bị Xung Đột (0% Conflict)

1. **Làm tuần tự từng người:**
   $$\text{Người 1} \longrightarrow \text{Người 2} \longrightarrow \text{Người 3} \longrightarrow \text{Người 4} \longrightarrow \text{Người 5} \longrightarrow \text{Người 6} \longrightarrow \text{Người 7}$$
2. **Trước khi copy file, luôn kéo code mới nhất về:**
   ```powershell
   git pull origin main
   ```
3. **Mỗi người chỉ copy và add ĐÚNG file của mình**, không đụng vào file người khác.

---

## 🚀 3. Câu Lệnh Chi Tiết Cho Từng Thành Viên

### 👑 NGƯỜI 1 (Trưởng Nhóm - Khởi Tạo Repo)
1. Tạo một repository mới trên GitHub (ví dụ: `https://github.com/<username>/ProjectNhi.git`).
2. Mở PowerShell tại thư mục dự án và chạy:
```powershell
# 1. Khởi tạo Git
git init
git branch -M main

# 2. Add các file khởi tạo
git add .gitignore README.md build.bat run_test.bat data/finance_data.csv include/Common.h

# 3. Commit
git commit -m "feat: initialize project structure, build scripts and common utilities"

# 4. Kết nối và đẩy lên GitHub (thay link repo của bạn)
git remote add origin https://github.com/<username>/ProjectNhi.git
git push -u origin main
```
3. **Mời các thành viên:** Vào GitHub Repo $\rightarrow$ **Settings** $\rightarrow$ **Collaborators** $\rightarrow$ Bấm **Add people** $\rightarrow$ Thêm tài khoản GitHub của 6 bạn còn lại.

---

### 👤 NGƯỜI 2 (Account Module)
*(Đợi Người 1 push xong thì Người 2 làm)*
```powershell
# 1. Clone dự án về máy
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 1 về
git pull origin main

# 3. Add, Commit và Push thẳng lên main
git add include/Account.h src/Account.cpp
git commit -m "feat(account): implement Account class and balance management"
git push origin main
```

---

### 👤 NGƯỜI 3 (Budget Module)
*(Đợi Người 2 push xong thì Người 3 làm)*
```powershell
# 1. Clone dự án (nếu chưa clone) hoặc cd vào thư mục đã có
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 2 về
git pull origin main

# 3. Add, Commit và Push thẳng lên main
git add include/Budget.h src/Budget.cpp
git commit -m "feat(budget): implement Budget limits and alert thresholds"
git push origin main
```

---

### 👤 NGƯỜI 4 (Transaction Module)
*(Đợi Người 3 push xong thì Người 4 làm)*
```powershell
# 1. Clone dự án (nếu chưa clone) hoặc cd vào thư mục đã có
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 3 về
git pull origin main



# 3. Add, Commit và Push thẳng lên main
git add include/Transaction.h src/Transaction.cpp
git commit -m "feat(transaction): implement Transaction hierarchy and polymorphism"
git push origin main
```

---

### 👤 NGƯỜI 5 (Storage Module)
*(Đợi Người 4 push xong thì Người 5 làm)*
```powershell
# 1. Clone dự án (nếu chưa clone) hoặc cd vào thư mục đã có
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 4 về
git pull origin main


# 3. Add, Commit và Push thẳng lên main
git add include/FinanceStorage.h src/FinanceStorage.cpp
git commit -m "feat(storage): implement FinanceStorage CSV file I/O"
git push origin main
```

---

### 👤 NGƯỜI 6 (FinanceManager Controller)
*(Đợi Người 5 push xong thì Người 6 làm)*
```powershell
# 1. Clone dự án (nếu chưa clone) hoặc cd vào thư mục đã có
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 5 về
git pull origin main


# 3. Add, Commit và Push thẳng lên main
git add include/FinanceManager.h src/FinanceManager.cpp
git commit -m "feat(manager): implement FinanceManager controller, recurring simulation and ASCII charts"
git push origin main
```

---

### 👤 NGƯỜI 7 (CLI Menu & Test Automation Suite)
*(Đợi Người 6 push xong thì Người 7 làm)*
```powershell
# 1. Clone dự án (nếu chưa clone) hoặc cd vào thư mục đã có
git clone https://github.com/<username>/ProjectNhi.git
cd ProjectNhi

# 2. Kéo code mới nhất từ Người 6 về
git pull origin main


# 3. Add, Commit và Push thẳng lên main
git add src/main.cpp tests/test_finance.cpp
git commit -m "feat(app): implement interactive console menu and automated test suite"
git push origin main
```

---

## 🧪 4. Kiểm Tra Sau Khi Cả 7 Người Đã Hoàn Thành

Sau khi Người 7 báo đã push xong, bất kỳ ai kéo code về cũng đều có trọn vẹn dự án:

```powershell
git pull origin main

# Biên dịch lại toàn bộ dự án
.\build.bat

# Chạy test kiểm thử tự động
.\run_test.bat

# Chạy ứng dụng
.\bin\FinanceApp.exe
```

Khi kết quả hiển thị **`[7 YEU CAU DEU PASS TEST 100%!]`**, dự án đã hoàn thành hoàn hảo và cả 7 bạn đều có lịch sử đóng góp rõ ràng trên GitHub!
