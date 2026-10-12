#pragma once
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>
#include <string>
#include <vector>
#include "Gudang.h"

class JendelaUtama : public QMainWindow {
    Q_OBJECT

public:
    JendelaUtama(QWidget* parent = nullptr);

private:
    Gudang gudang;

    QTableWidget* tabel;
    QLabel* labelRingkasan;
    QListWidget* daftarRiwayat;
    QSpinBox* inputBatas;
    QLineEdit* inputCari;
    QComboBox* comboJenis;

    void tampilkanBarang(const std::vector<const Barang*>& daftar);
    void perbaruiTampilan();
    void tampilkanStokSedikit();
    std::string kodeTerpilih();

    void tambahBarang();
    void barangMasuk();
    void barangKeluar();
    void hapusBarang();
    void cariBarangNama();
    void filterBerdasarkanJenis();
};
