#include "timer.h"

Timer::Timer(imgUploader *imageUploader, MainImage *mainImage, QObject *parent) :
    QTimer(parent)
{
    uploader = imageUploader;
    image = mainImage;
    running = false;
    nextSyncTime->currentTime();
    increaseInterval();
    connect(this, SIGNAL (timeout()), this, SLOT (tick()));
    start(1000);
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
    image->reset();
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
    if (running && *nextSyncTime <= current) {
        increaseInterval();
        shoot();
    }
}

void Timer::increaseInterval()
{
    nextSyncTime->currentTime().addSecs(INTERVAL);
}

void Timer::save(QPixmap pixmap)
{
    if (tempDir.isValid()) {
        image->set(pixmap);
        // Send image to uploader.
        qint64 second = QDateTime::currentMSecsSinceEpoch();
        QString format = "png";
        QString fileName = QString(tempDir.path() + "/screenshot-" + QString::number(second) + "." + format);
        pixmap.save(fileName, format.toUtf8().constData());
        uploader->uploadImage(fileName);
    }
}

void Timer::shoot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QPixmap pixmap = screen->grabWindow(0);
        save(pixmap);
    }
}
