#include <QApplication>
#include "JendelaUtama.h"

int main(int argc, char* argv[]) {
    QApplication aplikasi(argc, argv);
    JendelaUtama jendela;
    jendela.show();
    return aplikasi.exec();
}
