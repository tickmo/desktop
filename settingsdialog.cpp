#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "mainwindow.h"

SettingsDialog::SettingsDialog(QSettings *parentSettings, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    settings = parentSettings;
    QString name = settings->value("User/name").toString();
    if (name.length() > 0) {
        labelText(name);
    }
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

    QString url_str = MainWindow::api_url("api/v1/sessions");
    QUrl url = QUrl(url_str);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;");
    QNetworkReply *reply = mgr.post(req, bytes);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument replyData = QJsonDocument::fromJson(reply->readAll());
        settings->setValue("User/token", replyData.object().value("token").toString());
        QString name = replyData.object().value("name").toString();
        settings->setValue("User/name", name);
        labelText(name);
        ui->login->clear();
        ui->passwd->clear();
        delete reply;
        this->close();
    }
    else
    {
        ui->passwd->clear();
        ui->passwd->setStyleSheet("border: 1px solid red; padding: 5px 20px");
    }
}

void SettingsDialog::labelText(QString name) {
    ui->label->setText(tr("Hello, %1").arg(name));
}
