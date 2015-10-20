#include "view.h"

View::View(QWidget *parent)
    : QGraphicsView(parent)
{

}

View::~View()
{

}

void View::wheelEvent(QWheelEvent *event)
{
    emit wheelEventOnView(event);
    QGraphicsView::wheelEvent(event);
}
