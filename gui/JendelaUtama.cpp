#include "JendelaUtama.h"
#include "DataAwal.h"
#include "Utilitas.h"
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFontDatabase>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

static QString teks(const std::string& isi) {
    return QString::fromStdString(isi);
}

static QTableWidgetItem* selAngka(const std::string& isi) {
    QTableWidgetItem* sel = new QTableWidgetItem(teks(isi));
    sel->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return sel;
}

JendelaUtama::JendelaUtama(QWidget* parent)
    : QMainWindow(parent),
      gudang(NAMA_GUDANG, TANGGAL_HARI_INI) {
    isiDataAwal(gudang);
    setWindowTitle("Gudang Minimarket TETITB");
    resize(1000, 650);

    QLabel* labelJudul = new QLabel(teks(gudang.getNama() + "    Tanggal: " +
                                         formatTanggal(gudang.getTanggalHariIni())));
    labelJudul->setStyleSheet("font-weight: bold; font-size: 15px;");

    QPushButton* tombolTambah = new QPushButton("Tambah Barang");
    QPushButton* tombolMasuk = new QPushButton("Barang Masuk");
    QPushButton* tombolKeluar = new QPushButton("Barang Keluar");
    QPushButton* tombolHapus = new QPushButton("Hapus Barang");

    inputCari = new QLineEdit();
    inputCari->setPlaceholderText("Cari nama barang...");
    QPushButton* tombolCari = new QPushButton("Cari");

    comboJenis = new QComboBox();
    comboJenis->addItems({"Semua", "Umum", "Makanan", "Elektronik", "Pecah Belah"});

    QPushButton* tombolFilter = new QPushButton("Tampilkan");
    QPushButton* tombolSemua = new QPushButton("Semua Barang");
    inputBatas = new QSpinBox();
    inputBatas->setRange(0, 100000);
    inputBatas->setValue(10);

    QHBoxLayout* barisAksi = new QHBoxLayout();
    barisAksi->addWidget(tombolTambah);
    barisAksi->addWidget(tombolMasuk);
    barisAksi->addWidget(tombolKeluar);
    barisAksi->addWidget(tombolHapus);
    barisAksi->addStretch();

    QHBoxLayout* barisFilter = new QHBoxLayout();
    barisFilter->addWidget(new QLabel("Cari:"));
    barisFilter->addWidget(inputCari);
    barisFilter->addWidget(tombolCari);
    barisFilter->addSpacing(10);
    barisFilter->addWidget(new QLabel("Jenis:"));
    barisFilter->addWidget(comboJenis);
    barisFilter->addSpacing(10);
    barisFilter->addWidget(new QLabel("Stok di bawah:"));
    barisFilter->addWidget(inputBatas);
    barisFilter->addWidget(tombolFilter);
    barisFilter->addWidget(tombolSemua);

    tabel = new QTableWidget(0, 8);
    tabel->setHorizontalHeaderLabels({"Kode", "Nama Barang", "Jenis", "Harga", "Stok",
                                      "Biaya/Bulan", "Keterangan", "Status"});
    tabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabel->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabel->setSelectionMode(QAbstractItemView::SingleSelection);
    tabel->verticalHeader()->hide();
    tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tabel->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    labelRingkasan = new QLabel();
    daftarRiwayat = new QListWidget();
    daftarRiwayat->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    daftarRiwayat->setMaximumHeight(160);

    QVBoxLayout* susunan = new QVBoxLayout();
    susunan->addWidget(labelJudul);
    susunan->addLayout(barisAksi);
    susunan->addLayout(barisFilter);
    susunan->addWidget(tabel);
    susunan->addWidget(labelRingkasan);
    susunan->addWidget(new QLabel("Riwayat barang masuk dan keluar:"));
    susunan->addWidget(daftarRiwayat);

    QWidget* isi = new QWidget();
    isi->setLayout(susunan);
    setCentralWidget(isi);

    connect(tombolTambah, &QPushButton::clicked, this, &JendelaUtama::tambahBarang);
    connect(tombolMasuk, &QPushButton::clicked, this, &JendelaUtama::barangMasuk);
    connect(tombolKeluar, &QPushButton::clicked, this, &JendelaUtama::barangKeluar);
    connect(tombolHapus, &QPushButton::clicked, this, &JendelaUtama::hapusBarang);
    connect(tombolCari, &QPushButton::clicked, this, &JendelaUtama::cariBarangNama);
    connect(inputCari, &QLineEdit::returnPressed, this, &JendelaUtama::cariBarangNama);
    connect(comboJenis, &QComboBox::currentIndexChanged, this, &JendelaUtama::filterBerdasarkanJenis);
    connect(tombolFilter, &QPushButton::clicked, this, &JendelaUtama::tampilkanStokSedikit);
    connect(tombolSemua, &QPushButton::clicked, this, &JendelaUtama::perbaruiTampilan);

    perbaruiTampilan();
}

// Polymorphism: jenis, keterangan, biaya, dan status mengikuti jenis asli barangnya
void JendelaUtama::tampilkanBarang(const std::vector<const Barang*>& daftar) {
    tabel->setRowCount((int)daftar.size());
    for (int i = 0; i < (int)daftar.size(); i++) {
        const Barang* b = daftar[i];
        tabel->setItem(i, 0, new QTableWidgetItem(teks(b->getKode())));
        tabel->setItem(i, 1, new QTableWidgetItem(teks(b->getNama())));
        tabel->setItem(i, 2, new QTableWidgetItem(teks(b->getJenis())));
        tabel->setItem(i, 3, selAngka(formatRupiah(b->getHarga())));
        tabel->setItem(i, 4, selAngka(std::to_string(b->getStok())));
        tabel->setItem(i, 5, selAngka(formatRupiah(b->hitungBiayaPenyimpanan())));
        tabel->setItem(i, 6, new QTableWidgetItem(teks(b->getKeterangan())));

        QTableWidgetItem* status = new QTableWidgetItem("Layak dijual");
        if (!b->layakDijual(gudang.getTanggalHariIni())) {
            status->setText("Tidak layak dijual");
            status->setForeground(Qt::red);
        }
        tabel->setItem(i, 7, status);
    }
    tabel->clearSelection();
}

// Menampilkan ulang semua barang, ringkasan, dan riwayat
void JendelaUtama::perbaruiTampilan() {
    std::vector<const Barang*> semua = gudang.getDaftarBarang();
    tampilkanBarang(semua);

    labelRingkasan->setText(teks("Jumlah barang: " + std::to_string(semua.size()) +
                                 "      Total nilai stok: " + formatRupiah(gudang.hitungTotalNilai()) +
                                 "      Biaya simpan/bulan: " + formatRupiah(gudang.hitungTotalBiaya())));

    daftarRiwayat->clear();
    const std::vector<std::string>& riwayat = gudang.getRiwayat();
    for (size_t i = 0; i < riwayat.size(); i++) {
        daftarRiwayat->addItem(teks(std::to_string(i + 1) + ". " + riwayat[i]));
    }
}

// Overloading: memakai getDaftarBarang dengan batas stok
void JendelaUtama::tampilkanStokSedikit() {
    tampilkanBarang(gudang.getDaftarBarang(inputBatas->value()));
}

std::string JendelaUtama::kodeTerpilih() {
    QList<QTableWidgetItem*> terpilih = tabel->selectedItems();
    if (terpilih.isEmpty()) {
        QMessageBox::warning(this, "Pilih Barang", "Pilih dulu barangnya di tabel.");
        return "";
    }
    return tabel->item(terpilih.first()->row(), 0)->text().toStdString();
}

void JendelaUtama::barangMasuk() {
    std::string kode = kodeTerpilih();
    if (kode.empty()) {
        return;
    }
    bool ok;
    int jumlah = QInputDialog::getInt(this, "Barang Masuk", teks("Jumlah masuk untuk " + kode + ":"),
                                      1, 0, 100000, 1, &ok);
    if (ok) {
        QMessageBox::information(this, "Barang Masuk", teks(gudang.terimaBarang(kode, jumlah)));
        perbaruiTampilan();
    }
}

void JendelaUtama::barangKeluar() {
    std::string kode = kodeTerpilih();
    if (kode.empty()) {
        return;
    }
    bool ok;
    int jumlah = QInputDialog::getInt(this, "Barang Keluar", teks("Jumlah keluar untuk " + kode + ":"),
                                      1, 0, 100000, 1, &ok);
    if (ok) {
        QMessageBox::information(this, "Barang Keluar", teks(gudang.keluarkanBarang(kode, jumlah)));
        perbaruiTampilan();
    }
}

void JendelaUtama::tambahBarang() {
    bool ok;
    QString jenis = QInputDialog::getItem(this, "Tambah Barang", "Jenis barang:",
                                          {"Umum", "Makanan", "Elektronik", "Pecah Belah"}, 0, false, &ok);
    if (!ok) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Tambah Barang " + jenis);
    dialog.setMinimumWidth(350);
    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* inputKode = new QLineEdit();
    inputKode->setMaxLength(7);
    inputKode->setPlaceholderText("contoh MK-067");
    QLineEdit* inputNama = new QLineEdit();
    inputNama->setMaxLength(25);
    QSpinBox* inputHarga = new QSpinBox();
    inputHarga->setRange(1, 100000000);
    inputHarga->setPrefix("Rp");
    QSpinBox* inputStok = new QSpinBox();
    inputStok->setRange(0, 100000);

    form->addRow("Kode", inputKode);
    form->addRow("Nama", inputNama);
    form->addRow("Harga", inputHarga);
    form->addRow("Stok awal", inputStok);

    QLineEdit* inputKategori = nullptr;
    QDateEdit* inputKedaluwarsa = nullptr;
    QSpinBox* inputGaransi = nullptr;
    QSpinBox* inputProteksi = nullptr;
    if (jenis == "Umum") {
        inputKategori = new QLineEdit();
        inputKategori->setMaxLength(20);
        form->addRow("Kategori", inputKategori);
    } else if (jenis == "Makanan") {
        QDate hariIni = QDate::fromString(QString::number(gudang.getTanggalHariIni()), "yyyyMMdd");
        inputKedaluwarsa = new QDateEdit(hariIni);
        inputKedaluwarsa->setDisplayFormat("dd-MM-yyyy");
        inputKedaluwarsa->setCalendarPopup(true);
        form->addRow("Kedaluwarsa", inputKedaluwarsa);
    } else if (jenis == "Elektronik") {
        inputGaransi = new QSpinBox();
        inputGaransi->setRange(0, 120);
        inputGaransi->setSuffix(" bulan");
        form->addRow("Garansi", inputGaransi);
    } else {
        inputProteksi = new QSpinBox();
        inputProteksi->setRange(0, 1000000);
        inputProteksi->setPrefix("Rp");
        inputProteksi->setSingleStep(500);
        form->addRow("Biaya proteksi", inputProteksi);
    }

    QDialogButtonBox* tombol = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(tombol);
    connect(tombol, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(tombol, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    std::string kode = inputKode->text().trimmed().toUpper().toStdString();
    std::string nama = inputNama->text().trimmed().toStdString();
    if (kode.empty() || nama.empty() ||
        (inputKategori != nullptr && inputKategori->text().trimmed().isEmpty())) {
        QMessageBox::warning(this, "Tambah Barang", "Semua isian wajib diisi.");
        return;
    }

    Barang* barangBaru;
    if (jenis == "Umum") {
        barangBaru = new BarangUmum(kode, nama, inputHarga->value(), inputStok->value(),
                                    inputKategori->text().trimmed().toStdString());
    } else if (jenis == "Makanan") {
        int kedaluwarsa = inputKedaluwarsa->date().toString("yyyyMMdd").toInt();
        barangBaru = new BarangMakanan(kode, nama, inputHarga->value(), inputStok->value(),
                                       kedaluwarsa);
    } else if (jenis == "Elektronik") {
        barangBaru = new BarangElektronik(kode, nama, inputHarga->value(), inputStok->value(),
                                          inputGaransi->value());
    } else {
        barangBaru = new BarangPecahBelah(kode, nama, inputHarga->value(), inputStok->value(),
                                          inputProteksi->value());
    }
    QMessageBox::information(this, "Tambah Barang", teks(gudang.tambahBarangBaru(barangBaru)));
    perbaruiTampilan();
}

void JendelaUtama::hapusBarang() {
    std::string kode = kodeTerpilih();
    if (kode.empty()) {
        return;
    }
    QMessageBox::StandardButton konfirmasi = QMessageBox::question(
        this, "Hapus Barang", teks("Hapus barang dengan kode " + kode + "?\n(Barang hanya bisa dihapus jika stok sudah 0)"),
        QMessageBox::Yes | QMessageBox::No);
    if (konfirmasi == QMessageBox::Yes) {
        std::string hasil = gudang.hapusBarang(kode);
        if (hasil.rfind("Gagal:", 0) == 0) {
            QMessageBox::warning(this, "Hapus Barang", teks(hasil));
        } else {
            QMessageBox::information(this, "Hapus Barang", teks(hasil));
            perbaruiTampilan();
        }
    }
}

void JendelaUtama::cariBarangNama() {
    std::string kata = inputCari->text().trimmed().toStdString();
    if (kata.empty()) {
        perbaruiTampilan();
        return;
    }
    std::vector<const Barang*> hasil = gudang.cariBarangNama(kata);
    tampilkanBarang(hasil);
    labelRingkasan->setText(teks("Hasil pencarian \"" + kata + "\": " + std::to_string(hasil.size()) + " barang ditemukan."));
}

void JendelaUtama::filterBerdasarkanJenis() {
    QString jenis = comboJenis->currentText();
    if (jenis == "Semua") {
        perbaruiTampilan();
        return;
    }
    // Overloading: getDaftarBarang dengan parameter string jenis
    std::vector<const Barang*> hasil = gudang.getDaftarBarang(jenis.toStdString());
    tampilkanBarang(hasil);
    labelRingkasan->setText(teks("Filter jenis \"" + jenis.toStdString() + "\": " + std::to_string(hasil.size()) + " barang ditemukan."));
}
