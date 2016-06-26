#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QSettings>
#include <QTime>
#include <QPixmap>
#include <QByteArray>
#include <QTime>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QScreen>
#include <QPixmap>
#include <QDateTime>
#include <QTemporaryDir>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrlQuery>
#include <QUrl>
#include <QByteArray>
#include <QFile>

#include "mainwindow.h"
#include "imguploader.h"

class MainWindow;
class Timer;

class Timer : public QTimer
{
    Q_OBJECT

public:
    explicit Timer(MainWindow *parent = 0);
    bool active();
    void run();
    void pause();
    void toggle();
    imgUploader Uploader;

private slots:
    void tick();

private:
    bool running;
    QTime nextSyncTime;
    static const int INTERVAL = 3 * 60;
    void increaseInterval();
    void save();
    void shoot();
    QPixmap originalPixmap;
    QTemporaryDir tempDir;
    MainWindow *mainWindow;
};

#endif // TIMER_H
