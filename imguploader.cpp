#include "imguploader.h"

imgUploader::imgUploader(QObject *parent) :
    QObject(parent)
{
    UserAgent = "tickmo";
    Uploading = false;
}

void imgUploader::uploadImage(QString FileName, QString UploadURL, QString FieldName)
{

    QUrl serviceUrl = QUrl(UploadURL);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, UserAgent);

    QFile *file = new QFile(FileName);
    QFileInfo *fileInfo = new QFileInfo(FileName);
    file->setParent(multiPart);
    file->open(QIODevice::ReadOnly);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + FieldName + "\"; filename=\"" + fileInfo->fileName() +  "\""));

    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + multiPart->boundary());
    currentUpload =  manager.post(request, multiPart);
    multiPart->setParent(currentUpload);

    connect(currentUpload,SIGNAL(uploadProgress(qint64,qint64)),this, SLOT(onUploadProgress(qint64,qint64)));
    connect(currentUpload, SIGNAL(finished()),this,SLOT(onUploadFinished()));

    Uploading = true;
}

bool imgUploader::isUploading()
{
    return Uploading;
}

void imgUploader::onUploadFinished()
{
    Uploading = false;
    currentUpload->deleteLater();
    emit uploadFinished();
}

void imgUploader::onUploadProgress(qint64 uploaded, qint64 total)
{
    emit uploadProgress( uploaded,  total);
}
