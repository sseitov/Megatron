#include "controllerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerWidget w;
    w.show();

    return a.exec();
}
