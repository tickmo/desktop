#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QGraphicsScene>
#include <QDebug>
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
#include <QRect>

#include "settingsdialog.h"
#include "timer.h"

class Timer;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setImage(QPixmap pixmap);
    static QString api_url(QString str);

private slots:
    void on_tickButton_clicked();
    void on_toolButton_clicked();

private:
    void setButtonText();
    void resetImage();
    void openSettings();
    bool isLoggedIn();
    Ui::MainWindow *ui;
    Timer *timer;
    QSettings *settings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *tickAction;
    QAction *quitAction;
    QAction *openSettingsAction;

    SettingsDialog *settingsDialog;
    QRect basicDimension;
};

#endif // MAINWINDOW_H
