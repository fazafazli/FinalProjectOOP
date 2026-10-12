#include "Barang.h"
#include "Utilitas.h"
#include <algorithm>
using namespace std;

// Barang
// Nilai negatif diubah jadi 0 supaya data barang selalu masuk akal.
Barang::Barang(string kodeBarang, string namaBarang, double hargaBarang, int stokAwal)
    : kode(kodeBarang),
      nama(namaBarang),
      harga(max(0.0, hargaBarang)),
      stok(max(0, stokAwal)) {
}

string Barang::getKode() const { return kode; }
string Barang::getNama() const { return nama; }
double Barang::getHarga() const { return harga; }
int Barang::getStok() const { return stok; }

bool Barang::tambahStok(int jumlah) {
    if (jumlah <= 0) {
        return false;
    }
    stok += jumlah;
    return true;
}

bool Barang::kurangiStok(int jumlah) {
    if (jumlah <= 0 || jumlah > stok) {
        return false;
    }
    stok -= jumlah;
    return true;
}

bool Barang::layakDijual(int) const {
    return true;
}

// BarangUmum
BarangUmum::BarangUmum(string kodeBarang, string namaBarang, double hargaBarang,
                       int stokAwal, string kategoriBarang)
    : Barang(kodeBarang, namaBarang, hargaBarang, stokAwal),
      kategori(kategoriBarang) {
}

string BarangUmum::getJenis() const {
    return "Umum";
}

string BarangUmum::getKeterangan() const {
    return "Kategori: " + kategori;
}

double BarangUmum::hitungBiayaPenyimpanan() const {
    return getStok() * 200;
}

// BarangMakanan
BarangMakanan::BarangMakanan(string kodeBarang, string namaBarang, double hargaBarang,
                             int stokAwal, int kedaluwarsa)
    : Barang(kodeBarang, namaBarang, hargaBarang, stokAwal),
      tanggalKedaluwarsa(kedaluwarsa) {
}

bool BarangMakanan::layakDijual(int tanggal) const {
    return tanggal <= tanggalKedaluwarsa;
}

string BarangMakanan::getJenis() const {
    return "Makanan";
}

string BarangMakanan::getKeterangan() const {
    return "Kedaluwarsa: " + formatTanggal(tanggalKedaluwarsa);
}

double BarangMakanan::hitungBiayaPenyimpanan() const {
    return getStok() * 300;
}

// BarangElektronik
BarangElektronik::BarangElektronik(string kodeBarang, string namaBarang, double hargaBarang,
                                   int stokAwal, int lamaGaransi)
    : Barang(kodeBarang, namaBarang, hargaBarang, stokAwal),
      garansiBulan(max(0, lamaGaransi)) {
}

string BarangElektronik::getJenis() const {
    return "Elektronik";
}

string BarangElektronik::getKeterangan() const {
    return "Garansi: " + to_string(garansiBulan) + " bulan";
}

// Biaya simpan elektronik berupa asuransi 1% dari nilai stok
double BarangElektronik::hitungBiayaPenyimpanan() const {
    return getStok() * getHarga() * 0.01;
}

// BarangPecahBelah
BarangPecahBelah::BarangPecahBelah(string kodeBarang, string namaBarang, double hargaBarang,
                                   int stokAwal, double biayaProteksi)
    : Barang(kodeBarang, namaBarang, hargaBarang, stokAwal),
      biayaPengamanan(max(0.0, biayaProteksi)) {
}

string BarangPecahBelah::getJenis() const {
    return "Pecah Belah";
}

string BarangPecahBelah::getKeterangan() const {
    return "Proteksi: " + formatRupiah(biayaPengamanan) + "/unit";
}

// Biaya simpan pecah belah: biaya dasar Rp250 + biaya proteksi per unit
double BarangPecahBelah::hitungBiayaPenyimpanan() const {
    return getStok() * (250 + biayaPengamanan);
}
