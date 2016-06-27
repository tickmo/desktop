#include "src/mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile style(":/assets/style.css");
    style.open(QFile::ReadOnly);
    app.setStyleSheet(style.readAll());
    MainWindow window;
    window.show();
    return app.exec();
}
