#include "timer.h"

static xcb_screen_t * screen;

Timer::Timer(imgUploader *imageUploader, MainImage *mainImage, QObject *parent) :
    QTimer(parent)
{
    uploader = imageUploader;
    image = mainImage;
    running = false;
    nextSyncTime = QTime().currentTime();
    increaseInterval();
    connect(this, SIGNAL (timeout()), this, SLOT (tick()));
    start(1000);
    screen = xcb_setup_roots_iterator (xcb_get_setup (QX11Info::connection())).data;
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
    uploader->uploadFiles(false);
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
    xcb_screensaver_query_info_cookie_t cookie = xcb_screensaver_query_info (QX11Info::connection(), screen->root);
    xcb_screensaver_query_info_reply_t *info = xcb_screensaver_query_info_reply (QX11Info::connection(), cookie, NULL);
    uint idle = info->ms_since_user_input;
    free (info);
    if (idle/1000 > 30)
    {
        pause();
        return;
    }
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
