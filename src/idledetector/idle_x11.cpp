#include "idle.h"
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)

#include <QApplication>
#include <QDesktopWidget>
#include <xcb/xcb.h>
#include <xcb/screensaver.h>
#include <QtX11Extras/QX11Info>

static xcb_screen_t * screen;

IdlePlatform::IdlePlatform()
{
	screen = xcb_setup_roots_iterator (xcb_get_setup (QX11Info::connection())).data;
}

IdlePlatform::~IdlePlatform()
{

}

bool IdlePlatform::init()
{
	return true;
}

int IdlePlatform::secondsIdle()
{
	xcb_screensaver_query_info_cookie_t cookie = xcb_screensaver_query_info (QX11Info::connection(), screen->root);
	xcb_screensaver_query_info_reply_t *info = xcb_screensaver_query_info_reply (QX11Info::connection(), cookie, NULL);

	uint idle = info->ms_since_user_input;
	free (info);

	return idle/1000;
}

#endif
