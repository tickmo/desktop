#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QSettings>
#include <QTime>
#include <QPixmap>
#include <QByteArray>
#include <QTime>
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
#include <QDateTime>

#include "../uploader/imguploader.h"
#include "../mainimage.h"
#include "../idledetector/idle.h"

class Timer : public QTimer
{
    Q_OBJECT

public:
    explicit Timer(imgUploader *imageUploader, MainImage *mainImage, QObject *parent = 0);
    bool active();
    void run();
    void pause();
    void toggle();

public slots:
    void on_idle(int seconds);

signals:
    void status_changed();

private slots:
    void tick();

private:
    bool running;
    QTime nextSyncTime;
    // Set interval to ~8 minutes, to create about 60 screenshot per day.
    static const int INTERVAL = 5;
    void increaseInterval();
    void save(QPixmap pixmap);
    void shoot();
    uint currentTime();
    void saveTime();
    QPixmap originalPixmap;
    QTemporaryDir tempDir;
    imgUploader *uploader;
    MainImage *image;
    Idle *idle;
    uint unixtime;
    QDateTime currentDateTime;
};

#endif // TIMER_H
