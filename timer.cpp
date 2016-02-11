#include "timer.h"

// Declaring address host to send. (e.g. "http://tickmo-web.dev/")
const char* SERVER_TO_SEND = "http://st.evilephant.com/";

Timer::Timer(MainWindow *parent) :
    QTimer(parent)
{
    running = false;
    nextSyncTime = QTime().currentTime();
    increaseInterval();
    connect(this, SIGNAL (timeout()), this, SLOT (tick()));
    start(1000);
    mainWindow = parent;
}

bool Timer::active()
{
    return running;
}

void Timer::run()
{
    running = true;
    increaseInterval();
    shoot();
}

void Timer::pause()
{
    running = false;
}

void Timer::toggle()
{
    if (active())
        pause();
    else
        run();
}

void Timer::tick()
{
    QTime current = QTime().currentTime();
    if (running && nextSyncTime <= current) {
        increaseInterval();
        shoot();
    }
}

void Timer::increaseInterval()
{
    nextSyncTime = nextSyncTime.currentTime().addSecs(INTERVAL);
}

void Timer::save()
{
    qint64 second = QDateTime::currentMSecsSinceEpoch();
    QString format = "png";
    if (tempDir.isValid()) {
        QString fileName = QString(tempDir.path() + "/screenshot-" + QString::number(second) + "." + format);
        originalPixmap.save(fileName, format.toUtf8().constData());
        mainWindow->setImage(originalPixmap);
        QString url = SERVER_TO_SEND;
        Uploader.uploadImage(fileName, url, "screenshots[]");
    }
}

void Timer::shoot()
{
    originalPixmap = QPixmap();
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        originalPixmap = screen->grabWindow(0);
        save();
    }
}
