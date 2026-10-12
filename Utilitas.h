#pragma once
#include <cmath>
#include <string>

// Fungsi bantu untuk menampilkan rupiah dan tanggal.
inline std::string formatRupiah(double nilai) {
    std::string angka = std::to_string(std::llround(nilai));
    for (int i = (int)angka.length() - 3; i > 0; i -= 3) {
        angka.insert(i, ".");
    }
    return "Rp" + angka;
}

// Tanggal disimpan sebagai angka YYYYMMDD, contohnya 20261002.
// Fungsi cek apakah tanggal tersebut benar ada di kalender.
inline bool tanggalValid(int tanggal) {
    int tahun = tanggal / 10000;
    int bulan = (tanggal / 100) % 100;
    int hari = tanggal % 100;
    if (tahun < 2000 || tahun > 2100 || bulan < 1 || bulan > 12 || hari < 1) {
        return false;
    }
    int hariPerBulan[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool kabisat = (tahun % 4 == 0 && tahun % 100 != 0) || tahun % 400 == 0;
    if (kabisat) {
        hariPerBulan[1] = 29;
    }
    return hari <= hariPerBulan[bulan - 1];
}

inline std::string duaDigit(int angka) {
    return (angka < 10 ? "0" : "") + std::to_string(angka);
}

inline std::string formatTanggal(int tanggal) {
    return duaDigit(tanggal % 100) + "-" + duaDigit((tanggal / 100) % 100) + "-" +
           std::to_string(tanggal / 10000);
}
