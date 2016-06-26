#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QSettings *parentSettings, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingsDialog();

private slots:
    void on_loginButton_clicked();
    void on_login_textChanged(QString text);

private:
    Ui::SettingsDialog *ui;
    QSettings *settings;
    void labelText(QString name);
    QString errorStyle;
    QString loginStyle;
    QString passwdStyle;
};

#endif // SETTINGSDIALOG_H
