#include "timer.h"

Timer::Timer(imgUploader *imageUploader, MainImage *mainImage, QObject *parent) :
    QTimer(parent)
{
    uploader = imageUploader;
    image = mainImage;
    running = false;
    idle = new Idle();
    connect(idle, SIGNAL(secondsIdle(int)), this, SLOT(on_idle(int)));
    nextSyncTime = QTime().currentTime();
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
    status_changed();
}

void Timer::pause()
{
    running = false;
    image->reset();
    uploader->uploadFiles(false);
    status_changed();
}

void Timer::toggle()
{
    if (active())
        pause();
    else
        run();
}

void Timer::on_idle(int seconds)
{
    if (seconds > 30)
    {
        pause();
    }
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
        QPixmap pixmap;
        pixmap = screen->grabWindow(0);
        save(pixmap);
    }
}
