#include "settingsdialog.h"
#include "ui_settingsdialog.h"

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
    settings->beginGroup("User");
    settings->setValue("name", ui->login->text());
    settings->setValue("pass", ui->passwd->text());
    settings->endGroup();
}
