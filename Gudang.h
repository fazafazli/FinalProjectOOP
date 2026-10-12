#pragma once
#include <string>
#include <vector>
#include "Barang.h"

// Gudang menyimpan semua barang dan riwayat barang masuk atau keluar.
// Abstraction: pemakainya cukup memanggil method public tanpa perlu tahu detailnya.
class Gudang {
private:
    std::string nama;
    int tanggalHariIni;

    // Polymorphism: isinya campuran dari berbagai jenis barang
    std::vector<Barang*> daftarBarang;
    std::vector<std::string> riwayat;

    Barang* cariBarang(std::string kode) const;
    void catatRiwayat(std::string jenis, Barang* barang, int jumlah);

public:
    Gudang(std::string namaGudang, int tanggal);
    ~Gudang();

    // Tidak boleh disalin, supaya objek barang yang sama tidak dihapus dua kali
    Gudang(const Gudang&) = delete;
    Gudang& operator=(const Gudang&) = delete;

    std::string getNama() const;
    int getTanggalHariIni() const;
    const std::vector<std::string>& getRiwayat() const;

    // Overloading:
    // 1. Tanpa parameter untuk semua barang
    // 2. Parameter int untuk barang dengan stok di bawah batas
    // 3. Parameter string untuk filter barang berdasarkan jenis
    // Hasilnya const supaya stok tidak bisa diubah dari luar Gudang.
    std::vector<const Barang*> getDaftarBarang() const;
    std::vector<const Barang*> getDaftarBarang(int batasStok) const;
    std::vector<const Barang*> getDaftarBarang(std::string jenisTertentu) const;

    std::vector<const Barang*> cariBarangNama(std::string kataKunci) const;

    double hitungTotalNilai() const;
    double hitungTotalBiaya() const;

    // Mengembalikan pesan hasil, untuk ditampilkan oleh console maupun GUI
    std::string tambahBarangBaru(Barang* barang);
    std::string terimaBarang(std::string kode, int jumlah);
    std::string keluarkanBarang(std::string kode, int jumlah);
    std::string hapusBarang(std::string kode);
};
