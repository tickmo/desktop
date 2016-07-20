#ifndef IMGUPLOADER_H
#define IMGUPLOADER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QSettings>
#include <QQueue>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../timer/timemessage.h"

class imgUploader : public QObject
{
    Q_OBJECT
public:
    explicit imgUploader(QSettings *parentSettings, QObject *parent = 0);
    void uploadImage(QString filename);
    void addTime(TimeMessage timeMessage);
    void uploadFiles(bool quitAfterAll = false);
    QString UserAgent;
    bool isUploading();

private:
    static const int UPLOAD_INTERVAL = 20 * 1000;
    QSettings *settings;
    QNetworkAccessManager manager;
    QNetworkReply *currentUpload;
    QTimer *timer;
    QQueue<QString> filesQueue, failedQueue;
    QQueue<TimeMessage> timeMessagesQueue, timeMessagesFailedQueue;
    bool uploading;
    bool quitAfterUploading;

signals:
    void uploadProgress(qint64 uploaded, qint64 total);
    void uploadFinished();

public slots:
    void onUploadFinished();
    void onUploadProgress(qint64 uploaded, qint64 total);

private slots:
    void onUploadTimeIsCome();
};

#endif // IMGUPLOADER_H
