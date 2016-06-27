#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QGraphicsScene>
#include <QString>
#include <QSystemTrayIcon>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "settings/settingsdialog.h"
#include "uploader/imguploader.h"
#include "timer/timer.h"
#include "mainimage.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    static QString api_url(QString str);

private slots:
    void on_tickButton_clicked();
    void on_toolButton_clicked();
    void status_changed();

private:
    void setButtonText();
    void openSettings();
    bool isLoggedIn();
    Ui::MainWindow *ui;

    Timer *timer;
    QSettings *settings;
    imgUploader *uploader;
    SettingsDialog *settingsDialog;
    MainImage *image;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *tickAction;
    QAction *quitAction;
    QAction *openSettingsAction;
};

#endif // MAINWINDOW_H
