#include "imguploader.h"
#include "mainwindow.h"

imgUploader::imgUploader(QSettings *parentSettings, QObject *parent) :
    QObject(parent)
{
    settings = parentSettings;
    UserAgent = "tickmo";
    uploading = false;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onUploadTimeIsCome()));
    timer->start(UPLOAD_INTERVAL);
    quitAfterUploading = false;
    removeFirstFromFileQueue = 0;
}

void imgUploader::uploadImage(QString FileName)
{
    filesQueue.append(FileName);
}

void imgUploader::uploadFiles(bool quitAfterAll)
{
    quitAfterUploading = quitAfterAll;
    if (filesQueue.isEmpty()) {
        if (quitAfterUploading) {
            QApplication::quit();
        }
        return;
    }
    QUrl serviceUrl = QUrl(MainWindow::api_url("api/v1/file"));

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, UserAgent);

    QString username = settings->value("User/name").toString();
    QString token = settings->value("User/token").toString();
    QString authHeader = "Token token=\"" + token + "\", user_email=\"" + username + "\"";
    request.setRawHeader("Authorization", authHeader.toLocal8Bit());

    for (int i = 0; i < filesQueue.size(); i++)
    {
        QString FileName = filesQueue.at(i);
        QFile *file = new QFile(FileName);
        QFileInfo *fileInfo = new QFileInfo(FileName);
        file->setParent(multiPart);
        file->open(QIODevice::ReadOnly);

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"screenshots[]\"; filename=\"" + fileInfo->fileName() +  "\""));

        imagePart.setBodyDevice(file);
        multiPart->append(imagePart);
        removeFirstFromFileQueue = i + 1;
    }

    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + multiPart->boundary());
    currentUpload =  manager.post(request, multiPart);
    multiPart->setParent(currentUpload);

    connect(currentUpload, SIGNAL(uploadProgress(qint64,qint64)),this, SLOT(onUploadProgress(qint64,qint64)));
    connect(currentUpload, SIGNAL(finished()),this,SLOT(onUploadFinished()));

    uploading = true;
}

bool imgUploader::isUploading()
{
    return uploading;
}

void imgUploader::onUploadFinished()
{
    uploading = false;
    currentUpload->deleteLater();
    emit uploadFinished();
    if (removeFirstFromFileQueue > 0) {
        for (int i = 1; i < removeFirstFromFileQueue; i++) {
            filesQueue.takeAt(i);
        }
    }
    if (quitAfterUploading) {
        QApplication::quit();
    }
}

void imgUploader::onUploadProgress(qint64 uploaded, qint64 total)
{
    emit uploadProgress( uploaded,  total);
}

void imgUploader::onUploadTimeIsCome()
{
    uploadFiles();
}
