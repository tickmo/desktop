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
    uploader = new imgUploader(settings, this);
    image = new MainImage(ui->graphicsView, this);
    timer = new Timer(uploader, image, this);
    connect(timer, SIGNAL(status_changed()), this, SLOT(status_changed()));

    QPixmap icon = QPixmap(":/icon/icon.png").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);

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
    uploader->uploadFiles(true);
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
        setButtonText();
    }
}

void MainWindow::setButtonText()
{
    QPalette *palette = new QPalette(ui->tickButton->palette());
    if (timer->active()) {
        palette->setColor(QPalette::Button, QColor::fromRgb(165, 44, 47));
        ui->tickButton->setText("Stop");
        trayIconMenu->actions().at(0)->setText(QString("&Stop"));
    }
    else {
        palette->setColor(QPalette::Button, QColor::fromRgb(53, 104, 126));
        ui->tickButton->setText("Start");
        trayIconMenu->actions().at(0)->setText(QString("&Start"));
    }
    ui->tickButton->setPalette(*palette);
}

void MainWindow::status_changed()
{
    setButtonText();
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

