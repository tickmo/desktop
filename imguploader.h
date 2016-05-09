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

#include <QDebug>
class imgUploader : public QObject
{
    Q_OBJECT
public:
    explicit imgUploader(QObject *parent = 0);
    void uploadImage(QString FileName);
    void uploadFiles(bool quitAfterAll = false);
    QString UserAgent;
    bool isUploading();
private:
    static const int UPLOAD_INTERVAL = 30 * 60 * 1000;
    QNetworkAccessManager manager;
    QNetworkReply *currentUpload;
    QTimer *timer;
    QStringList filesQueue;
    bool Uploading;
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
