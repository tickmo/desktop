#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifndef DEBUG
    const QString API_URL = "https://trytickmo.com/";
#else
    const QString API_URL = "http://tickmo-web.dev/";
#endif

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

    tickAction = new QAction(tr("&Start"), this);
    connect(tickAction, SIGNAL(triggered()), this, SLOT(on_tickButton_clicked()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(tickAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);

    settingsDialog = 0;

    if (!isLoggedIn()) {
        openSettings();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    timer->Uploader.uploadFiles(true);
    event->ignore();
}


QString MainWindow::api_url(QString str)
{
    return API_URL + str;
}

void MainWindow::on_tickButton_clicked()
{
    if (!isLoggedIn()) {
        openSettings();
    }
    else {
        timer->toggle();
        resetImage();
        setButtonText();
    }
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
        trayIconMenu->actions().at(0)->setText(QString("&Stop"));
    }
    else {
        ui->tickButton->setText("Start");
        trayIconMenu->actions().at(0)->setText(QString("&Start"));
    }
}

void MainWindow::on_toolButton_clicked()
{
    openSettings();
}

void MainWindow::openSettings()
{
    if (!settingsDialog) {
        settingsDialog = new SettingsDialog(settings, this);
    }

    if (settingsDialog->exec()) {
        settingsDialog->show();
    }
}

bool MainWindow::isLoggedIn()
{
    return settings->value("User/token").toString().length() > 0;
}

