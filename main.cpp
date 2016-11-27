#include "codewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CodeWidget w;
    w.show();

    return a.exec();
}
