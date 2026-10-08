# Sistem Gudang Minimarket TETITB

Tugas Implementasi Pilar OOP, mata kuliah Pemrograman Berorientasi Objek (TKIF262304).

Anggota:
- Muhammad Fazli (25/560578/TK/63319)
- Naufal Arkana Maulana (25/564430/TK/63669)

## Deskripsi

Program C++ untuk mencatat stok barang di gudang minimarket. Ada tiga jenis barang, yaitu
umum, makanan, dan elektronik. Setiap jenis punya aturan sendiri, misalnya makanan tidak
boleh dijual setelah tanggal kedaluwarsa. Program juga menjaga agar stok tidak pernah
negatif dan mencatat riwayat barang masuk dan keluar.

Program tersedia dalam dua tampilan, yaitu console dan GUI (Qt). Keduanya memakai class
`Barang` dan `Gudang` yang sama.

## Fitur

1. Lihat semua barang beserta total nilai stok dan biaya simpan per bulan
2. Lihat barang dengan stok di bawah batas tertentu
3. Tambah barang baru
4. Barang masuk
5. Barang keluar
6. Riwayat barang masuk dan keluar

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

Program sudah berisi 7 barang contoh dengan tanggal hari ini 02-10-2026. Contoh tampilan
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
---------------------------------------------------------------------------
Jumlah barang      : 7
Total nilai stok   : Rp4.452.000
Biaya simpan/bulan : Rp58.650
```

## Struktur file

| File | Isi |
|---|---|
| `Barang.h`, `Barang.cpp` | Class `Barang` dan ketiga turunannya |
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

`Gudang` menyimpan semua barang dalam `vector<Barang*>` beserta riwayat barang masuk dan
keluar. Semua objek barang dihapus oleh destructor `Gudang`. `Gudang` tidak mencetak apa
pun. Hasil setiap operasi dikembalikan sebagai pesan, lalu ditampilkan oleh `main.cpp`
(console) atau `JendelaUtama` (GUI).

## Penerapan OOP

### Class dan object

`Barang`, ketiga turunannya, `Gudang`, dan `JendelaUtama` adalah class. Object-nya dibuat
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

`BarangUmum`, `BarangMakanan`, dan `BarangElektronik` mewarisi data, getter, dan aturan
stok dari `Barang`. Constructor turunan memanggil constructor `Barang`. Di versi GUI,
`JendelaUtama` adalah turunan dari `QMainWindow` milik Qt.

### Polymorphism

- Overriding: `getJenis()`, `getKeterangan()`, dan `hitungBiayaPenyimpanan()` dibuat ulang
  di setiap turunan, sedangkan `layakDijual()` hanya diganti oleh `BarangMakanan`. Saat
  daftar barang ditampilkan, satu loop pada daftar `Barang*` otomatis memanggil versi milik
  jenis barangnya masing-masing.
- Overloading: `getDaftarBarang()` untuk semua barang dan `getDaftarBarang(int batasStok)`
  untuk barang dengan stok di bawah batas.

## Skenario demo (console)

Tekan Enter setelah setiap hasil untuk kembali ke menu. Skenario yang sama juga bisa
dilakukan di versi GUI.

| No | Menu | Input | Hasil | Konsep |
|---|---|---|---|---|
| 1 | 1 | - | Tiga jenis barang tampil dalam satu tabel, yoghurt tertulis tidak layak dijual | Polymorphism |
| 2 | 2 | `10` | Hanya EL-001 dan UM-002 yang tampil | Overloading |
| 3 | 5 | `MK-003`, `5` | Ditolak karena yoghurt sudah kedaluwarsa | Polymorphism |
| 4 | 5 | `MK-001`, `50` | Ditolak karena stok hanya 40 | Encapsulation |
| 5 | 5 | `MK-001`, `0` | Ditolak karena jumlah harus lebih dari 0 | Encapsulation |
| 6 | 5 | `MK-001`, `28` | Berhasil, stok menjadi 12 | Abstraction |
| 7 | 4 | `UM-002`, `30` | Berhasil, stok menjadi 38 | Abstraction |
| 8 | 6 | - | Dua transaksi di atas tercatat | Abstraction |
| 9 | 3 | `2`, `MK-004`, `Keju Cheddar 165 g`, `28000`, `15`, `20270115` | Barang baru ikut tampil di menu 1 | Class dan object, Inheritance |
