#ifndef DOUBLEVIEWPORT_H
#define DOUBLEVIEWPORT_H

#include "workspace.h"
#include "detailviewport.h"
#include "interactivepointer.h"
#include "currentinfoviewport.h"

#include <QMainWindow>

class DoubleViewport : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(float imageViewingPercentage READ imageViewingPercentage WRITE setImageViewingPercentage)
    Q_PROPERTY(int windowSize READ windowSize WRITE setWindowSize)
    Q_PROPERTY(bool useInteractivePointer READ useInteractivePointer WRITE setUseInteractivePointer)
    Q_PROPERTY(int interactivePointerMode READ interactivePointerMode WRITE setInteractivePointerMode)

public:
    //--Variables-----------------------------------------------------------------------------------------
    int m_windowSize;

    //--Constructors/Destructors--------------------------------------------------------------------------
    DoubleViewport(QWidget *parent = 0);
    ~DoubleViewport();

    // Actions for dock toggles.
    QAction *detailAct;
    QAction *infoAct;

    //--Setters/Getters-----------------------------------------------------------------------------------
    void setImageViewingPercentage(const float &imageViewingPercentage_);  // Set and get how much of the
    float imageViewingPercentage() const;                                  // image is visible.
    void setWindowSize(const int &windowSize_);                            // Set and get how much of the
    int windowSize() const;                                                // image is visible.
    void setUseInteractivePointer(const bool &useInteractivePointer_);     // Set and get interactive
    bool useInteractivePointer() const;                                    // pointer activation state.
    void setInteractivePointerMode(const int &interactivePointerMode_);    // Set and get interactive
    int interactivePointerMode() const;                                    // pointer mode.

    //--Helper Functions----------------------------------------------------------------------------------
    void computeViewingPercentage();                        // Displays how much of the image is visible.
    bool loadImage(QImage filename1, QImage filename2);     // Displays the image in both workspaces.
    void loadImage(QString filename);                       // Overloaded method used when user loads new
                                                            // image.
    void initInteractivePointers();                         // Add pointers to workspace if user has
                                                            // activated them.
    void zoomControl(int delta);
    void updateFilenames(QString file1, QString file2);
    void setPointerSize(int _pointerDimX, int _pointerDimY, int _pointerWindowSize);

    QList<int> pixelValueAt(int x, int y);
    void pixelValueAt(int x, int y, QList<int> pVal);
    int imageWidth();
    int imageHeight();
    void updatePixelMap();

private:
    //--Variables-----------------------------------------------------------------------------------------
    DetailViewport *m_workspaced1;              // Workspace for the image detail view.
    Workspace *m_workspace1;                    // Workspace for the original image.
    Workspace *m_workspace2;                    // Workspace for the edited image.
    InteractivePointer *m_interactivepointer1;  // Interactive pointer for the original image.
    InteractivePointer *m_interactivepointer2;  // Interactive pointer for the edited image.
    QGraphicsProxyWidget * m_labelProxy1;       // Proxy object to embed label in workspace1.
    QGraphicsProxyWidget * m_labelProxy2;       // Proxy object to embed label in workspace2.
    QHBoxLayout *m_mainLayout;                  // Main layout used for this object.
    QWidget *m_widget;                          // A widget that holds the workspaces.
    QImage * m_image1;                          // The original image.
    QImage * m_image2;                          // A copy of the original image.
    QGraphicsPixmapItem* m_item1;               // Pixelmap of the original image.
    QGraphicsPixmapItem* m_item2;               // Pixelmap of the edited image.
    QGraphicsScene* m_scene1;                   // The scene that holds our original image.
    QGraphicsScene* m_scene2;                   // The scene that holds the edited image.
    float m_imageViewingPercentage;             // Holds the percentage of the image currently visible.
    QPixmap *m_Pixmap;                          // Pixmap used here for cursor paint.
    bool m_useInteractivePointer;               // Whether interactive pointer should be enabled.
    int m_interactivePointerMode;               // What should the interactive pointer display.
    QCursor *m_CurrentCursor;                   // Cursor object holding the current cursor shape.
    CurrentInfoViewport *m_currentInfo;         // Labels displaying general info.
    bool m_freeze;                              // Desides if detail values get updated or not.
    bool m_firstTime;                           // Variable that tells us if this is the furst time a
                                                // function was called.
    QDockWidget *dock;                          // Dock widget.

    //--Functions-----------------------------------------------------------------------------------------
    void resizeEvent(QResizeEvent* event);      // Event fired up on image resize.
    void keyPressEvent(QKeyEvent *event);       // Key Press event.

private slots:
    void updateS();
    void detailValues(QString);

signals:
    void windowResized();                       // Notify that the main window was resized.

};

#endif // DOUBLEVIEWPORT_H
