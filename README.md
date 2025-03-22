## Petunjuk Menjalankan Program Vending Machine

Program ini dibuat menggunakan bahasa pemrograman **C++**. Untuk memastikan program berjalan dengan semestinya, harap perhatikan beberapa hal berikut sebelum menjalankan:

---

### Persiapan File dan Folder
Pastikan sudah terdapat file dan folder berikut di directory:
1. **vending_dfa.txt** — definisi DFA (Deterministic Finite Automaton) untuk vending machine.
2. **vending.cpp** — file kode program utama.
3. **.vscode/** folder — folder akan terbuat otomatis ketika kita klik run & debug.
   - **tasks.json** — untuk automasi compile/run command.
4. **vending.exe** — file ini akan terbuat secara otomatis setelah file vending.cpp dikompilasi.

---

### Prosedur Menjalankan Program

1. **Buka terminal baru** di direktori yang sama dengan file **vending.cpp**.

2. **Kompilasi program menggunakan g++:**

   ```bash
   g++ vending.cpp -o vending
   ```

   Perintah ini akan meng-compile file `vending.cpp` menjadi file executable bernama `vending` (hasilnya di direktori seharusnya/biasanya bernama `vending.exe`).

3. **Jalankan program:**

   ```bash
   ./vending
   ```

---

### Catatan Penting:
- Pastikan file **vending_dfa.txt** berada dalam folder yang sama dengan file executable, karena program akan membaca konfigurasi DFA dari file ini.
- Program ini hanya menerima input berupa:
  - **Uang kertas:** `1000`, `2000`, `5000`, `10000`
  - **Pilihan minuman:** `A`, `B`, `C`

  Input selain itu akan dianggap tidak valid.

---

### Kesimpulan
Dengan mengikuti langkah-langkah di atas, program vending machine ini seharusnya dapat dijalankan dengan mudah dan tidak ada error.
