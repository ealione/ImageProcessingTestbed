#include "workspace.h"

Workspace::Workspace()
{
    this->setMouseTracking(true);                               // We need to track the mouse position
                                                                // inside a workspace.
    this->setInteractive(true);                                 // Workspace should be able to accept
                                                                // mouse, key events.
    this->installEventFilter(this);                             // Install a custom event filter.
    this->acceptDrops();                                        // Accept drag and Drop events.
    this->setDragMode(QGraphicsView::ScrollHandDrag);           // Drag content using your mouse.
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // No horizontal scrollbar.
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // No vertical scrollbar.
    this->setRenderHint(QPainter::Antialiasing);                // Try antialising the scene.
    this->setCacheMode(QGraphicsView::CacheBackground);         // Cache contents to improve performance.
    // Remove white background for consistency.
    this->setBackgroundBrush(QBrush(QColor(235, 236, 235), Qt::SolidPattern));
    // Set when the viewport should update.
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

//--Mouse Events-----------------------------------------------------------------------------------------
void Workspace::mousePressEvent(QMouseEvent *event)
{
    // Start a drag if needed.
    // http://doc.qt.io/qt-5/dnd.html
    /*if (event->button() == Qt::LeftButton
        && this->geometry().contains(event->pos())) {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText("tete");
        drag->setMimeData(mimeData);
        drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
    }*/

    if (event->button() == Qt::LeftButton) {
        //qDebug() << "Mouse1 Clicked";
        setMouseClicked(true);
    }

    mPixmap = new QPixmap(":/images/clicked.png");
    mCurrentCursor = new QCursor(*mPixmap);
    QApplication::changeOverrideCursor(*mCurrentCursor);
    delete mCurrentCursor;
    delete mPixmap;

    QGraphicsView::mousePressEvent(event); // Allow the event to also be handled by base component.
}

void Workspace::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse1 Released";
    setMouseClicked(false);

    mPixmap = new QPixmap(":/images/unclicked.png");
    mCurrentCursor = new QCursor(*mPixmap);
    QApplication::changeOverrideCursor(*mCurrentCursor);
    delete mCurrentCursor;
    delete mPixmap;

    /* Application cursors are stored on an internal stack. setOverrideCursor() pushes the
     * cursor onto the stack, and restoreOverrideCursor() pops the active cursor off the stack.
     * changeOverrideCursor() changes the curently active application override cursor.
     * Every setOverrideCursor() must eventually be followed by a corresponding
     * restoreOverrideCursor(), otherwise the stack will never be emptied.
     */
    //QApplication::setOverrideCursor(*mCurrentCursor);

    QGraphicsView::mouseReleaseEvent(event); // Allow the event to also be handled by base component.
}

void Workspace::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << (QString("Mouse move (%1,%2)").arg(event->x()).arg(event->y()));

    setMousePosX(event->x());
    setMousePosY(event->y());

    if(m_mouseClicked)
    {
        //qDebug() << "Panning";
        emit mustUpdate();  // Notify that we should possibly synchronise views.

        //mPixmap = new QPixmap(":/images/move.png");
        //mCurrentCursor = new QCursor(*mPixmap);
        //QApplication::setOverrideCursor(*mCurrentCursor);

        // Call this so we dont see the various artefacts left behind by the interactive pointers.
        this->scene()->update();
    }

    emit mouseMove(name()); // Notify parent that he needs to update his detail view.

    QGraphicsView::mouseMoveEvent(event); // Allow the event to also be handled by base component.
}

void Workspace::mouseDoubleClickEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse1 Double Click";

    QGraphicsView::mouseMoveEvent(event);
}
//-------------------------------------------------------------------------------------------------------

//--Repaint Event----------------------------------------------------------------------------------------
void Workspace::paintEvent(QPaintEvent *event)
{
    //qDebug() << "Repainting...";

    QGraphicsView::paintEvent(event);
}

//-------------------------------------------------------------------------------------------------------

//--Wheel Event------------------------------------------------------------------------------------------
void Workspace::wheelEvent(QWheelEvent *event)
{
    //qDebug() << "Repainting...";
    emit mustUpdate();      // Notify that we should possibly synchronise views.
    emit zoomEvent(*event);  // Notify that zoom is taking place

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    zoomUpdate(*event);

    QGraphicsView::wheelEvent(event);
}

//-------------------------------------------------------------------------------------------------------

//--Drag and Drop Events---------------------------------------------------------------------------------
void Workspace::dragEnterEvent(QDragEnterEvent *event)
{
    // Avoid re-implementing QGroupbox by hacking the drop event to only fire if above the widget.
    if (this->geometry().contains(this->mapFromGlobal(QCursor::pos())))
    {
        qDebug() << "Drag entered workspace1";
    }
}

void Workspace::dragLeaveEvent(QDragEnterEvent * event)
{
     qDebug() << "Drag ended";
}

void Workspace::dragMoveEvent(QDragEnterEvent * event)
{
     qDebug() << "Drag moving";
}

//-------------------------------------------------------------------------------------------------------

//--Event Filter-----------------------------------------------------------------------------------------
bool Workspace::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::Enter)
    {
        //qDebug() << "Entered Workspace";
        setContainsMouse(true);

        //emit mouseHover(true);

        mPixmap = new QPixmap(":/images/unclicked.png");
        mCurrentCursor = new QCursor(*mPixmap);
        QApplication::setOverrideCursor(*mCurrentCursor);
        delete mCurrentCursor;
        delete mPixmap;
    }
    if (e->type() == QEvent::Leave)
    {
        //qDebug() << "Left Workspace";
        setContainsMouse(false);

        //emit mouseHover(false);

        Qt::CursorShape shape = Qt::ArrowCursor;
        QApplication::changeOverrideCursor(QCursor(shape));
        //QApplication::restoreOverrideCursor();
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------

//--Global Variables Setters/Getters---------------------------------------------------------------------

// Set-Get contains mouse.
void Workspace::setContainsMouse(const bool &containsMouse_)
{
    m_containsMouse = containsMouse_;
}

bool Workspace::containsMouse() const
{
    return m_containsMouse;
}

// Set-Get mouse clicked.
void Workspace::setMouseClicked(const bool &mouseClicked_)
{
    m_mouseClicked = mouseClicked_;
}

bool Workspace::mouseClicked() const
{
    return m_mouseClicked;
}

// Set-Get mouse position X.
void Workspace::setMousePosX(const int &mousePosX_)
{
    m_mousePosX = mousePosX_;
}

int Workspace::mousePosX() const
{
    return m_mousePosX;
}

// Set-Get mouse position Y.
void Workspace::setMousePosY(const int &mousePosY_)
{
    m_mousePosY = mousePosY_;
}

int Workspace::mousePosY() const
{
    return m_mousePosY;
}

// Set-Get instance name.
void Workspace::setName(const QString &name_)
{
    m_name = name_;
}

QString Workspace::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------------

//--Helper Functions-------------------------------------------------------------------------------------
void Workspace::zoomUpdate(QWheelEvent event)
{
    // Scale the view.
    double scaleFactor = 1.15;
    if(event.delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
