#pragma once
#include <string>

// Class abstrak untuk semua barang di gudang.
class Barang {
private:
    // Encapsulation: data hanya bisa diubah lewat method, supaya stok tidak pernah negatif.
    std::string kode;
    std::string nama;
    double harga;
    int stok;

public:
    Barang(std::string kodeBarang, std::string namaBarang, double hargaBarang, int stokAwal);
    virtual ~Barang() {}

    std::string getKode() const;
    std::string getNama() const;
    double getHarga() const;
    int getStok() const;

    bool tambahStok(int jumlah);
    bool kurangiStok(int jumlah);

    // Bawaannya selalu layak dijual, BarangMakanan menggantinya dengan cek kedaluwarsa.
    virtual bool layakDijual(int tanggal) const;

    // Wajib dibuat oleh setiap class turunan.
    virtual std::string getJenis() const = 0;
    virtual std::string getKeterangan() const = 0;
    virtual double hitungBiayaPenyimpanan() const = 0;
};

// Inheritance: ketiga class ini mewarisi data dan aturan stok dari Barang.
class BarangUmum : public Barang {
private:
    std::string kategori;

public:
    BarangUmum(std::string kodeBarang, std::string namaBarang, double hargaBarang,
               int stokAwal, std::string kategoriBarang);

    std::string getJenis() const override;
    std::string getKeterangan() const override;
    double hitungBiayaPenyimpanan() const override;
};

class BarangMakanan : public Barang {
private:
    int tanggalKedaluwarsa;

public:
    BarangMakanan(std::string kodeBarang, std::string namaBarang, double hargaBarang,
                  int stokAwal, int kedaluwarsa);

    bool layakDijual(int tanggal) const override;
    std::string getJenis() const override;
    std::string getKeterangan() const override;
    double hitungBiayaPenyimpanan() const override;
};

class BarangElektronik : public Barang {
private:
    int garansiBulan;

public:
    BarangElektronik(std::string kodeBarang, std::string namaBarang, double hargaBarang,
                     int stokAwal, int lamaGaransi);

    std::string getJenis() const override;
    std::string getKeterangan() const override;
    double hitungBiayaPenyimpanan() const override;
};
