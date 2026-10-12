#pragma once
#include <string>
#include "Gudang.h"

// Data contoh yang dipakai versi console dan versi GUI
const std::string NAMA_GUDANG = "[Gudang Minimarket TETITB]";
const int TANGGAL_HARI_INI = 20261002;

inline void isiDataAwal(Gudang& gudang) {
    gudang.tambahBarangBaru(new BarangMakanan("MK-001", "Susu UHT 1 L", 18500, 40, 20261215));
    gudang.tambahBarangBaru(new BarangMakanan("MK-002", "Roti Tawar Gandum", 16000, 12, 20261010));
    gudang.tambahBarangBaru(new BarangMakanan("MK-003", "Yoghurt Stroberi 200 ml", 9500, 24, 20260930));
    gudang.tambahBarangBaru(new BarangElektronik("EL-001", "Rice Cooker 1,8 L", 325000, 6, 12));
    gudang.tambahBarangBaru(new BarangElektronik("EL-002", "Kabel Data USB-C 1 m", 35000, 25, 3));
    gudang.tambahBarangBaru(new BarangPecahBelah("PB-001", "Piring Keramik 6 Pcs", 65000, 15, 1000));
    gudang.tambahBarangBaru(new BarangUmum("UM-001", "Sabun Cuci Piring 800 ml", 14500, 30, "Kebersihan"));
    gudang.tambahBarangBaru(new BarangUmum("UM-002", "Buku Tulis 67 lembar", 4000, 8, "Alat Tulis"));
}
