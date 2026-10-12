#include "Gudang.h"
#include "Utilitas.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
using namespace std;

Gudang::Gudang(string namaGudang, int tanggal)
    : nama(namaGudang),
      tanggalHariIni(tanggal) {
}

// Semua barang milik Gudang, jadi dihapus di sini
Gudang::~Gudang() {
    for (Barang* b : daftarBarang) {
        delete b;
    }
}

Barang* Gudang::cariBarang(string kode) const {
    for (Barang* b : daftarBarang) {
        if (b->getKode() == kode) {
            return b;
        }
    }
    return nullptr;
}

void Gudang::catatRiwayat(string jenis, Barang* barang, int jumlah) {
    ostringstream catatan;
    catatan << formatTanggal(tanggalHariIni) << "  " << left << setw(8) << jenis
            << setw(8) << barang->getKode() << setw(26) << barang->getNama()
            << jumlah << " unit (sisa " << barang->getStok() << ")";
    riwayat.push_back(catatan.str());
}

string Gudang::getNama() const { return nama; }
int Gudang::getTanggalHariIni() const { return tanggalHariIni; }
const vector<string>& Gudang::getRiwayat() const { return riwayat; }

vector<const Barang*> Gudang::getDaftarBarang() const {
    return vector<const Barang*>(daftarBarang.begin(), daftarBarang.end());
}

vector<const Barang*> Gudang::getDaftarBarang(int batasStok) const {
    vector<const Barang*> hasil;
    for (Barang* b : daftarBarang) {
        if (b->getStok() < batasStok) {
            hasil.push_back(b);
        }
    }
    return hasil;
}

vector<const Barang*> Gudang::getDaftarBarang(string jenisTertentu) const {
    vector<const Barang*> hasil;
    for (Barang* b : daftarBarang) {
        if (b->getJenis() == jenisTertentu) {
            hasil.push_back(b);
        }
    }
    return hasil;
}

static string keHurufKecil(string teks) {
    for (char& c : teks) {
        c = tolower((unsigned char)c);
    }
    return teks;
}

vector<const Barang*> Gudang::cariBarangNama(string kataKunci) const {
    vector<const Barang*> hasil;
    string kunci = keHurufKecil(kataKunci);
    for (Barang* b : daftarBarang) {
        if (keHurufKecil(b->getNama()).find(kunci) != string::npos) {
            hasil.push_back(b);
        }
    }
    return hasil;
}

double Gudang::hitungTotalNilai() const {
    double total = 0;
    for (Barang* b : daftarBarang) {
        total += b->getHarga() * b->getStok();
    }
    return total;
}

// Polymorphism: tiap jenis barang menghitung biaya simpan dengan rumusnya sendiri
double Gudang::hitungTotalBiaya() const {
    double total = 0;
    for (Barang* b : daftarBarang) {
        total += b->hitungBiayaPenyimpanan();
    }
    return total;
}

// Kalau kodenya sudah dipakai, objek barang langsung dihapus di sini
string Gudang::tambahBarangBaru(Barang* barang) {
    string kode = barang->getKode();
    if (cariBarang(kode) != nullptr) {
        delete barang;
        return "Gagal: kode " + kode + " sudah dipakai.";
    }
    daftarBarang.push_back(barang);
    return "Barang " + kode + " berhasil ditambahkan.";
}

string Gudang::terimaBarang(string kode, int jumlah) {
    Barang* b = cariBarang(kode);
    if (b == nullptr) {
        return "Gagal: barang dengan kode " + kode + " tidak ditemukan.";
    }
    if (!b->tambahStok(jumlah)) {
        return "Gagal: jumlah harus lebih dari 0.";
    }
    catatRiwayat("MASUK", b, jumlah);
    return "Berhasil. Stok " + b->getNama() + " sekarang " + to_string(b->getStok()) + ".";
}

string Gudang::keluarkanBarang(string kode, int jumlah) {
    Barang* b = cariBarang(kode);
    if (b == nullptr) {
        return "Gagal: barang dengan kode " + kode + " tidak ditemukan.";
    }
    if (!b->layakDijual(tanggalHariIni)) {
        return "Gagal: " + b->getNama() + " sudah tidak layak dijual.";
    }
    if (!b->kurangiStok(jumlah)) {
        if (jumlah <= 0) {
            return "Gagal: jumlah harus lebih dari 0.";
        }
        return "Gagal: stok tidak cukup, sisa " + to_string(b->getStok()) + ".";
    }
    catatRiwayat("KELUAR", b, jumlah);
    return "Berhasil. Stok " + b->getNama() + " sekarang " + to_string(b->getStok()) + ".";
}

string Gudang::hapusBarang(string kode) {
    for (auto it = daftarBarang.begin(); it != daftarBarang.end(); ++it) {
        if ((*it)->getKode() == kode) {
            Barang* b = *it;
            if (b->getStok() > 0) {
                return "Gagal: barang " + kode + " masih memiliki stok " + to_string(b->getStok()) + " unit. Kosongkan stok terlebih dahulu.";
            }
            string namaBarang = b->getNama();
            catatRiwayat("HAPUS", b, 0);
            daftarBarang.erase(it);
            delete b;
            return "Berhasil. Barang " + kode + " (" + namaBarang + ") telah dihapus.";
        }
    }
    return "Gagal: barang dengan kode " + kode + " tidak ditemukan.";
}
