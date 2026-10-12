#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "DataAwal.h"
#include "Gudang.h"
#include "Utilitas.h"
using namespace std;

// Membaca satu baris dan membuang spasi di awal dan akhir.
// Kalau input habis, program langsung berhenti.
string bacaBaris(const string& pesan) {
    cout << pesan;
    string baris;
    if (!getline(cin, baris)) {
        cout << endl;
        exit(0);
    }
    size_t awal = baris.find_first_not_of(" \t\r");
    if (awal == string::npos) {
        return "";
    }
    size_t akhir = baris.find_last_not_of(" \t\r");
    return baris.substr(awal, akhir - awal + 1);
}

string bacaTeks(const string& pesan, size_t panjangMaks) {
    while (true) {
        string teks = bacaBaris(pesan);
        if (teks.empty()) {
            cout << "Tidak boleh kosong." << endl;
        } else if (teks.length() > panjangMaks) {
            cout << "Maksimal " << panjangMaks << " karakter." << endl;
        } else {
            return teks;
        }
    }
}

int bacaAngka(const string& pesan) {
    while (true) {
        istringstream isi(bacaBaris(pesan));
        int angka;
        char sisa;
        if (isi >> angka && !(isi >> sisa)) {
            return angka;
        }
        cout << "Masukkan angka bulat tanpa titik." << endl;
    }
}

int bacaPilihan(const string& pesan, int batasBawah, int batasAtas) {
    while (true) {
        int angka = bacaAngka(pesan);
        if (angka >= batasBawah && angka <= batasAtas) {
            return angka;
        }
        cout << "Masukkan angka " << batasBawah << " sampai " << batasAtas << "." << endl;
    }
}

// Kode barang diubah jadi huruf besar, contoh mk-001 menjadi MK-001
string bacaKode(const string& pesan) {
    string kode = bacaTeks(pesan, 7);
    for (char& huruf : kode) {
        huruf = toupper((unsigned char)huruf);
    }
    return kode;
}

int bacaTanggal(const string& pesan) {
    while (true) {
        int tanggal = bacaAngka(pesan);
        if (tanggalValid(tanggal)) {
            return tanggal;
        }
        cout << "Tanggal tidak valid. Gunakan format YYYYMMDD." << endl;
    }
}

void tungguEnter() {
    cout << endl << "Tekan Enter untuk kembali ke menu.";
    string abaikan;
    getline(cin, abaikan);
}

void garis(char karakter) {
    cout << string(75, karakter) << endl;
}

// Polymorphism: jenis, keterangan, biaya, dan status mengikuti jenis asli barangnya
void cetakTabel(const Gudang& gudang, const vector<const Barang*>& daftar) {
    cout << left << setw(8) << "KODE" << setw(26) << "NAMA BARANG" << setw(11) << "JENIS"
         << right << setw(12) << "HARGA" << setw(6) << "STOK" << setw(12) << "BIAYA/BLN"
         << endl;
    garis('-');
    for (const Barang* b : daftar) {
        cout << left << setw(8) << b->getKode() << setw(26) << b->getNama()
             << setw(11) << b->getJenis() << right << setw(12) << formatRupiah(b->getHarga())
             << setw(6) << b->getStok() << setw(12) << formatRupiah(b->hitungBiayaPenyimpanan())
             << endl;
        cout << "        " << b->getKeterangan() << endl;
        if (!b->layakDijual(gudang.getTanggalHariIni())) {
            cout << "        Status: tidak layak dijual" << endl;
        }
    }
    garis('-');
}

void menuLihatSemua(const Gudang& gudang) {
    cout << endl << "DAFTAR BARANG " << gudang.getNama() << endl;
    cout << "Tanggal: " << formatTanggal(gudang.getTanggalHariIni()) << endl;
    garis('=');
    vector<const Barang*> daftar = gudang.getDaftarBarang();
    cetakTabel(gudang, daftar);
    cout << "Jumlah barang      : " << daftar.size() << endl;
    cout << "Total nilai stok   : " << formatRupiah(gudang.hitungTotalNilai()) << endl;
    cout << "Biaya simpan/bulan : " << formatRupiah(gudang.hitungTotalBiaya()) << endl;
}

void menuStokSedikit(const Gudang& gudang) {
    int batas = bacaAngka("Tampilkan barang dengan stok di bawah: ");
    cout << endl << "BARANG DENGAN STOK DI BAWAH " << batas << endl;
    cout << "Tanggal: " << formatTanggal(gudang.getTanggalHariIni()) << endl;
    garis('=');
    vector<const Barang*> daftar = gudang.getDaftarBarang(batas);
    cetakTabel(gudang, daftar);
    cout << daftar.size() << " barang ditemukan." << endl;
}

void menuTambahBarang(Gudang& gudang) {
    cout << "Jenis barang: 1. Umum  2. Makanan  3. Elektronik  4. Pecah Belah" << endl;
    int jenis = bacaPilihan("Pilih jenis: ", 1, 4);
    string kode = bacaKode("Kode barang (contoh MK-067): ");
    string nama = bacaTeks("Nama barang (maks 25 karakter): ", 25);
    int harga = bacaPilihan("Harga (Rp, tanpa titik): ", 1, 100000000);
    int stok = bacaPilihan("Stok awal: ", 0, 100000);

    // Objek dibuat sesuai jenisnya, lalu disimpan Gudang sebagai Barang
    Barang* barangBaru;
    if (jenis == 1) {
        string kategori = bacaTeks("Kategori: ", 20);
        barangBaru = new BarangUmum(kode, nama, harga, stok, kategori);
    } else if (jenis == 2) {
        int kedaluwarsa = bacaTanggal("Tanggal kedaluwarsa (YYYYMMDD): ");
        barangBaru = new BarangMakanan(kode, nama, harga, stok, kedaluwarsa);
    } else if (jenis == 3) {
        int garansi = bacaPilihan("Lama garansi (bulan): ", 0, 120);
        barangBaru = new BarangElektronik(kode, nama, harga, stok, garansi);
    } else {
        int proteksi = bacaPilihan("Biaya proteksi/unit (Rp): ", 0, 1000000);
        barangBaru = new BarangPecahBelah(kode, nama, harga, stok, proteksi);
    }
    cout << gudang.tambahBarangBaru(barangBaru) << endl;
}

void menuBarangMasuk(Gudang& gudang) {
    string kode = bacaKode("Kode barang: ");
    int jumlah = bacaAngka("Jumlah masuk: ");
    cout << gudang.terimaBarang(kode, jumlah) << endl;
}

void menuBarangKeluar(Gudang& gudang) {
    string kode = bacaKode("Kode barang: ");
    int jumlah = bacaAngka("Jumlah keluar: ");
    cout << gudang.keluarkanBarang(kode, jumlah) << endl;
}

void menuRiwayat(const Gudang& gudang) {
    cout << endl << "RIWAYAT BARANG MASUK DAN KELUAR" << endl;
    garis('=');
    const vector<string>& riwayat = gudang.getRiwayat();
    if (riwayat.empty()) {
        cout << "Belum ada transaksi." << endl;
    }
    for (size_t i = 0; i < riwayat.size(); i++) {
        cout << i + 1 << ". " << riwayat[i] << endl;
    }
}

void menuCariBarang(const Gudang& gudang) {
    string kata = bacaBaris("Masukkan kata kunci pencarian nama barang: ");
    cout << endl << "HASIL PENCARIAN DENGAN KATA KUNCI: \"" << kata << "\"" << endl;
    garis('=');
    vector<const Barang*> daftar = gudang.cariBarangNama(kata);
    if (daftar.empty()) {
        cout << "Tidak ada barang yang cocok." << endl;
    } else {
        cetakTabel(gudang, daftar);
        cout << daftar.size() << " barang ditemukan." << endl;
    }
}

// Overloading: getDaftarBarang dengan parameter jenis (string)
void menuFilterJenis(const Gudang& gudang) {
    cout << "Pilih jenis barang: 1. Umum  2. Makanan  3. Elektronik  4. Pecah Belah" << endl;
    int p = bacaPilihan("Pilihan: ", 1, 4);
    string jenis = (p == 1) ? "Umum" : (p == 2) ? "Makanan" : (p == 3) ? "Elektronik" : "Pecah Belah";
    cout << endl << "BARANG JENIS " << jenis << endl;
    garis('=');
    vector<const Barang*> daftar = gudang.getDaftarBarang(jenis);
    cetakTabel(gudang, daftar);
    cout << daftar.size() << " barang ditemukan." << endl;
}

void menuHapusBarang(Gudang& gudang) {
    string kode = bacaKode("Masukkan kode barang yang akan dihapus: ");
    cout << gudang.hapusBarang(kode) << endl;
}

void tampilkanMenu() {
    cout << endl;
    cout << "[GUDANG MINIMARKET TETITB]" << endl;
    cout << "1. Lihat semua barang" << endl;
    cout << "2. Lihat barang dengan stok sedikit" << endl;
    cout << "3. Tambah barang baru" << endl;
    cout << "4. Barang masuk" << endl;
    cout << "5. Barang keluar" << endl;
    cout << "6. Riwayat barang masuk dan keluar" << endl;
    cout << "7. Cari barang berdasarkan nama" << endl;
    cout << "8. Filter barang berdasarkan jenis" << endl;
    cout << "9. Hapus barang" << endl;
    cout << "0. Keluar" << endl;
    cout << "----------------------------------------" << endl;
}

int main() {
    Gudang gudang(NAMA_GUDANG, TANGGAL_HARI_INI);
    isiDataAwal(gudang);

    while (true) {
        tampilkanMenu();
        int pilihan = bacaPilihan("Pilih menu: ", 0, 9);
        if (pilihan == 0) {
            break;
        }

        switch (pilihan) {
            case 1: menuLihatSemua(gudang); break;
            case 2: menuStokSedikit(gudang); break;
            case 3: menuTambahBarang(gudang); break;
            case 4: menuBarangMasuk(gudang); break;
            case 5: menuBarangKeluar(gudang); break;
            case 6: menuRiwayat(gudang); break;
            case 7: menuCariBarang(gudang); break;
            case 8: menuFilterJenis(gudang); break;
            case 9: menuHapusBarang(gudang); break;
        }
        tungguEnter();
    }

    cout << endl << "Program selesai." << endl;
    return 0;
}
