#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include "mainwindow.h"

SettingsDialog::SettingsDialog(QSettings *parentSettings, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    settings = parentSettings;
    ui->login->setText(settings->value("User/login").toString());
    errorStyle = "border: 1px solid red;";
    loginStyle = ui->login->styleSheet();
    passwdStyle = ui->passwd->styleSheet();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_loginButton_clicked()
{
    QString login = ui->login->text();
    settings->setValue("User/login", login);
    QJsonObject json;
    QJsonObject userData;
    userData["email"] = login;
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
        ui->login->clear();
        ui->passwd->clear();
        delete reply;
        this->close();
    }
    else
    {
        ui->passwd->clear();
        ui->passwd->setStyleSheet(passwdStyle + errorStyle);
        if (ui->login->text().isEmpty()) {
            ui->login->setStyleSheet(loginStyle + errorStyle);
        }
    }
}

void SettingsDialog::on_login_textChanged(QString text)
{
    if (!text.isEmpty()) {
        ui->login->setStyleSheet(loginStyle);
    }
}
