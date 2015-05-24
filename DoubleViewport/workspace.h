#ifndef WORKSPACE_H
#define WORKSPACE_H

//#include <QtCore>
#include <QtWidgets>

// Read here
// http://doc.qt.io/qt-5/qwidget.html#dragEnterEvent

class Workspace : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(bool containsMouse READ containsMouse WRITE setContainsMouse)
    Q_PROPERTY(bool mouseClicked READ mouseClicked WRITE setMouseClicked)
    Q_PROPERTY(bool mousePosX READ mousePosX WRITE setMousePosX)
    Q_PROPERTY(bool mousePosY READ mousePosY WRITE setMousePosY)
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    //--Constructors/Destructors--------------------------------------------------------------------------
    Workspace();

    //--Setters/Getters-----------------------------------------------------------------------------------
    void setMousePosX(const int &mousePosX_);           // Set and get mouse position X.
    int mousePosX() const;
    void setMousePosY(const int &mousePosY_);           // Set and get mouse position Y.
    int mousePosY() const;
    void setName(const QString &name_);                 // Set and get instance name.
    QString name() const;

private:
    //--Private Variables---------------------------------------------------------------------------------
    bool m_containsMouse;   // True if workspace contains mouse.
    bool m_mouseClicked;    // True if the mouse is clicked right now.
    int m_mousePosX;        // Holds the X position of the mouse.
    int m_mousePosY;        // Holds the Y position of the mouse.
    QString m_name;         // Holds the name of this instance.
    QPixmap *mPixmap;       // PixMap item to hold mouse cursor.
    QCursor *mCurrentCursor;// Current cursor.

protected:
    //--Workspace Events-----------------------------------------------------------------------------------
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent * event);
    void dragLeaveEvent(QDragEnterEvent * event);
    void dragMoveEvent(QDragEnterEvent * event);
    void paintEvent(QPaintEvent  *event);
    void wheelEvent(QWheelEvent *event);

    bool eventFilter(QObject* obj, QEvent* e);          // Event filter used here to capture mouse hover.

    //--Setters/Getters-----------------------------------------------------------------------------------
    void setContainsMouse(const bool &containsMouse_);  // Set and get if the workspace contains
    bool containsMouse() const;                         // the mouse cursor.
    void setMouseClicked(const bool &mouseClicked_);    // Set and get if the mouse was clicked.
    bool mouseClicked() const;

    //--Helper Functions----------------------------------------------------------------------------------
    private slots:
    void zoomUpdate(QWheelEvent delta);

signals:
    void mustUpdate();          // Notify that we should possibly synchronise views.
    void mouseEnter();          // Notify that the mouse just entered one of two workspaces.
    void mouseExit();           // Notify that the mouse just exited one of two workspaces.
    void mouseMove(QString);    // Notify parent that he needs to update his detail view.
    void mouseHover(bool);      // Notify parent that he needs to update his pointer visibility.
    void zoomEvent(QWheelEvent); // Pass zoom event to the other workspace.

};

#endif // WORKSPACE_H
