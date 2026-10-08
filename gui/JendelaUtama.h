#pragma once
#include <QLabel>
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

    void tampilkanBarang(const std::vector<const Barang*>& daftar);
    void perbaruiTampilan();
    void tampilkanStokSedikit();
    std::string kodeTerpilih();

    void tambahBarang();
    void barangMasuk();
    void barangKeluar();
};
