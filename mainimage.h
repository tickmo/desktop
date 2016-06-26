#ifndef MAINIMAGE_H
#define MAINIMAGE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QRect>
#include <QDebug>

class MainImage : public QObject
{
    Q_OBJECT
public:
    explicit MainImage(QGraphicsView *graphicsView, QObject *parent = 0);
    void set(QPixmap pixmap);
    void reset();

private:
    QGraphicsView *view;
    QRect basicDimension;

};

#endif // MAINIMAGE_H
