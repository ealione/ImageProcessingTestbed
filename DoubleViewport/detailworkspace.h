#ifndef DETAILWORKSPACE_H
#define DETAILWORKSPACE_H

#include <QtWidgets>

class DetailWorkspace : public QGraphicsView
{
    Q_OBJECT
public:
    DetailWorkspace();

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void resized();
};

#endif // DETAILWORKSPACE_H
