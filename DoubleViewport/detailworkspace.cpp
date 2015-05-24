#include "detailworkspace.h"

DetailWorkspace::DetailWorkspace()
{
    this->setMouseTracking(false);                              // We dont need to track the mouse
                                                                // position inside a detail workspace.
    this->setInteractive(false);                                // Detaile workspace does not need to
                                                                // accept mouse, key events.
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // No horizontal scrollbar.
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // No vertical scrollbar.
    this->setRenderHint(QPainter::Antialiasing);                // Try antialising the scene.
    this->setCacheMode(QGraphicsView::CacheBackground);         // Cache contents to improve performance.
    this->setAlignment(Qt::AlignTop);                           // Align scene at the top.
    this->setBackgroundBrush(QBrush                             // Remove white background for consistency.
                             (QColor(235, 236, 235),
                              Qt::SolidPattern));

    this->setMaximumWidth(400);                                 // We do not need this component to get
    this->setMaximumHeight(400);                                // very big.
    this->setMinimumWidth(200);                                 // We do not need this component to get
    this->setMinimumHeight(200);                                // very small.
}

void DetailWorkspace::resizeEvent(QResizeEvent *event)
{
    emit resized();
}


