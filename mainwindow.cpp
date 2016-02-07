#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QString>
#include <QSystemTrayIcon>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>

bool timerActive();
void timerStart();
void timerStop();
void timerToggle();
char* tick();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL (timeout()), this, SLOT (do_work()));
    timer.start(1000);
    QString filename = QString("default.png");
    setImage(filename);

    QPixmap pixmap(filename);
    QPixmap icon = pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setWindowIcon(QIcon(icon));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->show();

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tickButton_clicked()
{
    timerToggle();
    setButtonText();
}

void MainWindow::do_work()
{
    char* filename = tick();
    QString string = QString(filename);
    if (!string.isEmpty()) {
        setImage(string);
    }
    setButtonText();
}

void MainWindow::setImage(QString filename)
{
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap pixmap(filename);
    int x = ui->graphicsView->geometry().x();
    int y = ui->graphicsView->geometry().y();
    int width = ui->graphicsView->geometry().width();
    int height = ui->graphicsView->geometry().height();
    QPixmap scaled = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scene->addPixmap(scaled);
    int centerX = ( ( width - scaled.width() ) / 2 ) + x;
    int centerY = ( ( height - scaled.height() ) / 2 ) + y;
    ui->graphicsView->setGeometry(centerX, centerY, scaled.width(), scaled.height());
    ui->graphicsView->setScene(scene);
}

void MainWindow::setButtonText()
{
    if (timerActive()) {
        ui->tickButton->setText("Stop");
    }
    else {
        ui->tickButton->setText("Start");
    }
}
