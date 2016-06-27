#ifndef IDLE_H
#define IDLE_H

#include <QObject>
#include <QCursor>
#include <QDateTime>
#include <QTimer>

class IdlePlatform;

class Idle : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Service", "Idle")
public:
	Idle();
	~Idle();

	bool isActive() const;
	bool usingPlatform() const;
	void start();
	void stop();
	int secondsIdle();

signals:
	void secondsIdle(int);

private slots:
	void doCheck();

private:
	class Private;
	Private *d;
};

class IdlePlatform
{
public:
	IdlePlatform();
	~IdlePlatform();

	bool init();
	int secondsIdle();

private:
	class Private;
	Private *d;
};

class Idle::Private
{
public:
	Private() {}

	QPoint lastMousePos;
	QDateTime idleSince;

	bool active;
	int idleTime;
	QDateTime startTime;
	QTimer checkTimer;
};

#endif

