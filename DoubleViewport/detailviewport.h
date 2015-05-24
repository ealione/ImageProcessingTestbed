#ifndef DETAILVIEWPORT_H
#define DETAILVIEWPORT_H

#include "detailworkspace.h"
#include "detailvalues.h"

#include <QWidget>
#include <QtMath>

class DetailViewport : public QWidget
{
    Q_OBJECT
public:
    //--Constructor---------------------------------------------------------------------------------------
    DetailViewport(QWidget *parent = 0);

    //--Variables-----------------------------------------------------------------------------------------
    int m_windowSize;

    //--Functions-----------------------------------------------------------------------------------------
    void update(QVector<int> val1, QVector<int> val2, int winSize, int mouseX, int mouseY);

private:
    //--Variables-----------------------------------------------------------------------------------------
    DetailValues *m_workvalues1;     // Workspace for the original image pixel values.
    DetailValues *m_workvalues2;     // Workspace for the edited image pixel values.
    DetailWorkspace *m_workspaced1;  // Workspace for the original image zoom view.
    DetailWorkspace *m_workspaced2;  // Workspace for the edited image zoom view.
    QImage * m_imaged1;              // The original image zoom view.
    QImage * m_imaged2;              // Edited image zoom view.
    QGraphicsPixmapItem* m_itemd1;   // Pixelmap of the original image zoom view.
    QGraphicsPixmapItem* m_itemd2;   // Pixelmap of the edited image zoom view.
    QGraphicsScene* m_scened1;       // The scene that holds our original image.
    QGraphicsScene* m_scened2;       // The scene that holds the edited image.
    QVBoxLayout *m_mainLayout;       // Main layout used for this object.
    QHBoxLayout *m_zoomLayout;       // Zoom layout used for detail view workspaces.
    QHBoxLayout *m_valueLayout;      // Value layout used for pixel values.

public slots:
    void onResize();
};

#endif // DETAILVIEWPORT_H
