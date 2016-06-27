#include "mainimage.h"

MainImage::MainImage(QGraphicsView *graphicsView, QObject *parent) :
    QObject(parent)
{
    view = graphicsView;
    basicDimension = view->geometry();
    reset();
}

void MainImage::reset()
{
    QPixmap pixmap(QString(":/icon/default.png"));
    set(pixmap);
}

void MainImage::set(QPixmap pixmap)
{
    int x = basicDimension.x();
    int y = basicDimension.y();
    int width = basicDimension.width();
    int height = basicDimension.height();
    QPixmap scaled = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int centerX = ( ( width - scaled.width() ) / 2 ) + x;
    int centerY = ( ( height - scaled.height() ) / 2 ) + y;
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(scaled);
    view->setGeometry(centerX, centerY, scaled.width(), scaled.height());
    view->setScene(scene);
}
