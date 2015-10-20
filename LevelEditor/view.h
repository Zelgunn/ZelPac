#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

#include "map.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget * parent = 0);
    ~View();

protected:
    void wheelEvent(QWheelEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void wheelEventOnView(QWheelEvent *event);
};

#endif // VIEW_H
