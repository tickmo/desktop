#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "timer.h"

SettingsDialog::SettingsDialog(QSettings *parentSettings, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    settings = parentSettings;
    settings->beginGroup("User");
    ui->login->setText(settings->value("name").toString());
    ui->passwd->setText(settings->value("pass").toString());
    settings->endGroup();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_loginButton_clicked()
{
    QJsonObject json;
    QJsonObject userData;
    userData["email"] = ui->login->text();
    userData["password"] = ui->passwd->text();
    json["user"] = userData;
    QJsonDocument doc(json);
    QByteArray bytes = doc.toJson();

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QString url_str = "http://tickmo-web.dev/api/v1/sessions";
    QUrl url = QUrl(url_str);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;");
    QNetworkReply *reply = mgr.post(req, bytes);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument replyData = QJsonDocument::fromJson(reply->readAll());
        settings->beginGroup("User");
        settings->setValue("token", replyData.object().value("token").toString());
        settings->endGroup();
        delete reply;
        this->close();
    }
    else
    {
        ui->passwd->setStyleSheet("border: 1px solid red; padding: 5px 20px");
    }
}
