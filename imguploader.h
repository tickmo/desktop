#ifndef IMGUPLOADER_H
#define IMGUPLOADER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>

#include <QDebug>
class imgUploader : public QObject
{
    Q_OBJECT
public:
    explicit imgUploader(QObject *parent = 0);

    void uploadImage(QString FileName, QString UploadURL, QString FieldName);
    QString UserAgent;
    bool isUploading();
private:
    QNetworkAccessManager manager;
    QNetworkReply *currentUpload;

    bool Uploading;

signals:
    void uploadProgress(qint64 uploaded, qint64 total);
    void uploadFinished();

public slots:
    void onUploadFinished();
    void onUploadProgress(qint64 uploaded, qint64 total);

};

#endif // IMGUPLOADER_H
