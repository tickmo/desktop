#include "idle.h"

#ifdef Q_OS_WIN32

#include <QLibrary>
#include <windows.h>

#ifndef tagLASTINPUTINFO
typedef struct __tagLASTINPUTINFO {
	UINT cbSize;
	DWORD dwTime;
} __LASTINPUTINFO, *__PLASTINPUTINFO;
#endif

class IdlePlatform::Private
{
public:
	Private()
	{
		GetLastInputInfo = NULL;
		lib = 0;
	}

	BOOL (__stdcall * GetLastInputInfo)(__PLASTINPUTINFO);
	DWORD (__stdcall * IdleUIGetLastInputTime)(void);
	QLibrary *lib;
};

IdlePlatform::IdlePlatform()
{
	d = new Private;
}

IdlePlatform::~IdlePlatform()
{
	delete d->lib;
	delete d;
}

bool IdlePlatform::init()
{
	if(d->lib)
		return true;
	QFunctionPointer p;

	// try to find the built-in Windows 2000 function
	d->lib = new QLibrary("user32");
	if(d->lib->load() && (p = d->lib->resolve("GetLastInputInfo"))) {
		d->GetLastInputInfo = (BOOL (__stdcall *)(__PLASTINPUTINFO))p;
		return true;
	} else {
		delete d->lib;
		d->lib = 0;
	}

	// fall back on idleui
	d->lib = new QLibrary("idleui");
	if(d->lib->load() && (p = d->lib->resolve("IdleUIGetLastInputTime"))) {
		d->IdleUIGetLastInputTime = (DWORD (__stdcall *)(void))p;
		return true;
	} else {
		delete d->lib;
		d->lib = 0;
	}

	return false;
}

int IdlePlatform::secondsIdle()
{
	int i;
	if(d->GetLastInputInfo) {
		__LASTINPUTINFO li;
		li.cbSize = sizeof(__LASTINPUTINFO);
		bool ok = d->GetLastInputInfo(&li);
		if(!ok)
			return 0;
		i = li.dwTime;
	} else if (d->IdleUIGetLastInputTime) {
		i = d->IdleUIGetLastInputTime();
	} else
		return 0;

	return (GetTickCount() - i) / 1000;
}

#endif
