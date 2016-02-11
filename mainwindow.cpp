#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("tickmo", "tickmo");
    timer = new Timer(this);

    basicDimension = ui->graphicsView->geometry();

    resetImage();
    QPixmap icon = QPixmap(":/icon/default.png").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setWindowIcon(QIcon(icon));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(icon));
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

    settingsDialog = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tickButton_clicked()
{
    timer->toggle();
    resetImage();
    setButtonText();
}

void MainWindow::resetImage()
{
    if (!timer->active()) {
        QPixmap pixmap(QString(":/icon/default.png"));
        setImage(pixmap);
    }
}

void MainWindow::setImage(QPixmap pixmap)
{
    int x = basicDimension.x();
    int y = basicDimension.y();
    int width = basicDimension.width();
    int height = basicDimension.height();
    QPixmap scaled = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int centerX = ( ( width - scaled.width() ) / 2 ) + x;
    int centerY = ( ( height - scaled.height() ) / 2 ) + y;
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(scaled);
    ui->graphicsView->setGeometry(centerX, centerY, scaled.width(), scaled.height());
    ui->graphicsView->setScene(scene);
}

void MainWindow::setButtonText()
{
    if (timer->active()) {
        ui->tickButton->setText("Stop");
    }
    else {
        ui->tickButton->setText("Start");
    }
}

void MainWindow::on_toolButton_clicked()
{
    if (!settingsDialog) {
        settingsDialog = new SettingsDialog(settings, this);
    }

    if (settingsDialog->exec()) {
        settingsDialog->show();
    }
}

