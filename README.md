# Sistem Gudang Minimarket TETITB

Tugas Implementasi Pilar OOP, mata kuliah Pemrograman Berorientasi Objek (TKIF262304).

Anggota:
- Muhammad Fazli (25/560578/TK/63319)
- Naufal Arkana Maulana (25/564430/TK/63669)

## Deskripsi

Program C++ untuk mencatat stok barang di gudang minimarket. Ada empat jenis barang, yaitu
umum, makanan, elektronik, dan pecah belah. Setiap jenis punya aturan sendiri, misalnya makanan tidak
boleh dijual setelah tanggal kedaluwarsa dan pecah belah memerlukan biaya proteksi penanganan. Program juga menjaga agar stok tidak pernah
negatif dan mencatat riwayat barang masuk dan keluar.

Program tersedia dalam dua tampilan, yaitu console dan GUI (Qt). Keduanya memakai class
`Barang` dan `Gudang` yang sama.

## Fitur

1. Lihat semua barang beserta total nilai stok dan biaya simpan per bulan
2. Lihat barang dengan stok di bawah batas tertentu
3. Tambah barang baru (Umum, Makanan, Elektronik, Pecah Belah)
4. Barang masuk
5. Barang keluar
6. Riwayat barang masuk dan keluar
7. Cari barang berdasarkan nama
8. Filter barang berdasarkan jenis
9. Hapus barang (dengan validasi stok harus nol)

## Cara menjalankan

### Versi console

Butuh compiler C++17 (g++).

```bash
cd source_code
g++ -std=c++17 *.cpp -o gudang
./gudang
```

Di Windows (MinGW) perintah compile-nya sama, lalu jalankan `gudang.exe`.

### Versi GUI

Butuh Qt 6 dan CMake. Di Ubuntu 24.04:

```bash
sudo apt install qt6-base-dev cmake
cd source_code/gui
cmake -B build
cmake --build build
./build/gudang_gui
```

### Data contoh

Program sudah berisi 8 barang contoh dengan tanggal hari ini 02-10-2026. Contoh tampilan
menu 1 di versi console:

```
DAFTAR BARANG [Gudang Minimarket TETITB]
Tanggal: 02-10-2026
===========================================================================
KODE    NAMA BARANG               JENIS             HARGA  STOK   BIAYA/BLN
---------------------------------------------------------------------------
MK-001  Susu UHT 1 L              Makanan        Rp18.500    40    Rp12.000
        Kedaluwarsa: 15-12-2026
MK-002  Roti Tawar Gandum         Makanan        Rp16.000    12     Rp3.600
        Kedaluwarsa: 10-10-2026
MK-003  Yoghurt Stroberi 200 ml   Makanan         Rp9.500    24     Rp7.200
        Kedaluwarsa: 30-09-2026
        Status: tidak layak dijual
EL-001  Rice Cooker 1,8 L         Elektronik    Rp325.000     6    Rp19.500
        Garansi: 12 bulan
EL-002  Kabel Data USB-C 1 m      Elektronik     Rp35.000    25     Rp8.750
        Garansi: 3 bulan
UM-001  Sabun Cuci Piring 800 ml  Umum           Rp14.500    30     Rp6.000
        Kategori: Kebersihan
UM-002  Buku Tulis 67 lembar      Umum            Rp4.000     8     Rp1.600
        Kategori: Alat Tulis
PB-001  Piring Keramik 6 Pcs      Pecah Belah    Rp65.000    15    Rp18.750
        Proteksi: Rp1.000/unit
---------------------------------------------------------------------------
Jumlah barang      : 8
Total nilai stok   : Rp5.427.000
Biaya simpan/bulan : Rp77.400
```

## Struktur file

| File | Isi |
|---|---|
| `Barang.h`, `Barang.cpp` | Class `Barang` dan keempat turunannya |
| `Gudang.h`, `Gudang.cpp` | Class `Gudang` |
| `DataAwal.h` | Data contoh, dipakai versi console dan GUI |
| `Utilitas.h` | Fungsi bantu format rupiah dan tanggal |
| `main.cpp` | Tampilan console (menu) |
| `gui/JendelaUtama.h`, `gui/JendelaUtama.cpp` | Tampilan GUI |
| `gui/main_gui.cpp` | Program utama versi GUI |
| `gui/CMakeLists.txt` | Pengaturan build versi GUI |

## Class

| Class | Keterangan | Biaya simpan per bulan | Layak dijual |
|---|---|---|---|
| `Barang` | Class abstrak, induk semua barang | - | - |
| `BarangUmum` | Punya kategori | stok x Rp200 | selalu |
| `BarangMakanan` | Punya tanggal kedaluwarsa | stok x Rp300 | sampai tanggal kedaluwarsa |
| `BarangElektronik` | Punya garansi | stok x harga x 1% | selalu |
| `BarangPecahBelah` | Punya biaya proteksi penanganan | stok x (Rp250 + proteksi) | selalu |

`Gudang` menyimpan semua barang dalam `vector<Barang*>` beserta riwayat barang masuk dan
keluar. Semua objek barang dihapus oleh destructor `Gudang`. `Gudang` tidak mencetak apa
pun. Hasil setiap operasi dikembalikan sebagai pesan, lalu ditampilkan oleh `main.cpp`
(console) atau `JendelaUtama` (GUI).

## Penerapan OOP

### Class dan object

`Barang`, keempat turunannya, `Gudang`, dan `JendelaUtama` adalah class. Object-nya dibuat
misalnya lewat `Gudang gudang(...)` dan `new BarangMakanan(...)`.

### Encapsulation

Atribut `Barang` (`kode`, `nama`, `harga`, `stok`) dibuat private. Stok hanya bisa diubah
lewat `tambahStok()` dan `kurangiStok()`, yang menolak jumlah 0 atau negatif dan menolak
pengurangan yang melebihi stok. Daftar barang dan riwayat di `Gudang` juga private, dan
`getDaftarBarang()` hanya memberi pointer `const`, jadi stok tidak bisa diubah dari luar
`Gudang`.

```cpp
bool Barang::kurangiStok(int jumlah) {
    if (jumlah <= 0 || jumlah > stok) {
        return false;
    }
    stok -= jumlah;
    return true;
}
```

### Abstraction

`Barang` adalah class abstrak karena punya pure virtual function `getJenis()`,
`getKeterangan()`, dan `hitungBiayaPenyimpanan()`. Selain itu, tampilan console maupun GUI
cukup memanggil method seperti `gudang.keluarkanBarang(kode, jumlah)` tanpa perlu tahu cara
mencari barang, mengecek aturan, dan mencatat riwayat.

### Inheritance

`BarangUmum`, `BarangMakanan`, `BarangElektronik`, dan `BarangPecahBelah` mewarisi data, getter, dan aturan
stok dari `Barang`. Constructor turunan memanggil constructor `Barang`. Di versi GUI,
`JendelaUtama` adalah turunan dari `QMainWindow` milik Qt.

### Polymorphism

- Overriding: `getJenis()`, `getKeterangan()`, dan `hitungBiayaPenyimpanan()` dibuat ulang
  di setiap turunan, sedangkan `layakDijual()` hanya diganti oleh `BarangMakanan`. Saat
  daftar barang ditampilkan, satu loop pada daftar `Barang*` otomatis memanggil versi milik
  jenis barangnya masing-masing.
- Overloading: Tiga variasi method `getDaftarBarang()` pada `Gudang`:
  1. `getDaftarBarang()` untuk semua barang.
  2. `getDaftarBarang(int batasStok)` untuk barang dengan stok di bawah batas.
  3. `getDaftarBarang(std::string jenisTertentu)` untuk filter jenis barang.

## Skenario demo (console)

Tekan Enter setelah setiap hasil untuk kembali ke menu. Skenario yang sama juga bisa
dilakukan di versi GUI.

| No | Menu | Input | Hasil | Konsep |
|---|---|---|---|---|
| 1 | 1 | - | Empat jenis barang tampil dalam satu tabel, yoghurt tertulis tidak layak dijual | Polymorphism |
| 2 | 2 | `10` | Hanya EL-001 dan UM-002 yang tampil | Overloading |
| 3 | 5 | `MK-003`, `5` | Ditolak karena yoghurt sudah kedaluwarsa | Polymorphism |
| 4 | 5 | `MK-001`, `50` | Ditolak karena stok hanya 40 | Encapsulation |
| 5 | 5 | `MK-001`, `0` | Ditolak karena jumlah harus lebih dari 0 | Encapsulation |
| 6 | 5 | `MK-001`, `28` | Berhasil, stok menjadi 12 | Abstraction |
| 7 | 4 | `UM-002`, `30` | Berhasil, stok menjadi 38 | Abstraction |
| 8 | 6 | - | Dua transaksi di atas tercatat | Abstraction |
| 9 | 3 | `2`, `MK-004`, `Keju Cheddar 165 g`, `28000`, `15`, `20270115` | Barang baru ikut tampil di menu 1 | Class dan object, Inheritance |
| 10 | 3 | `4`, `PB-002`, `Gelas Kaca Kristal`, `25000`, `20`, `500` | Barang baru jenis Pecah Belah dengan proteksi ikut tampil di menu 1 | Class dan object, Inheritance, Polymorphism |
| 11 | 7 | `susu` | Menampilkan barang dengan nama Susu UHT 1 L | Abstraction |
| 12 | 8 | `4` | Menampilkan hanya barang Pecah Belah | Overloading |
| 13 | 9 | `MK-001` | Ditolak karena stok masih 40 (harus kosong) | Encapsulation |

## Pembagian Kerja

| Anggota | Tanggung Jawab Utama | Kontribusi Tambahan |
|---|---|---|
| **Muhammad Fazli** (25/560578/TK/63319) | Desain arsitektur dasar class & UML, implementasi awal `Barang` dan `Gudang`, modul Console & GUI dasar | Dokumentasi teknis dasar |
| **Naufal Arkana Maulana** (25/564430/TK/63669) | Perluasan hierarki OOP: class `BarangPecahBelah` (Inheritance & Polymorphism); implementasi fitur `cariBarangNama()`, overloading `getDaftarBarang(string)` untuk filter jenis, dan `hapusBarang()` dengan validasi stok di `Gudang`, Console, dan GUI Qt | Skenario pengujian sistem, penyusunan materi slide presentasi (PPT) & skrip video demo |
