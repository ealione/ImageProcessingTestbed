#include "doubleviewport.h"

//--Constructor------------------------------------------------------------------------------------------
DoubleViewport::DoubleViewport(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialise first time as true.
    m_firstTime = true; 

    // Add central widget
    m_widget = new QWidget;
    setCentralWidget(m_widget);

    // Initialise detail workspace.
    m_workspaced1 = new DetailViewport;   // Workspace used for displaying zoomed view.

    // Initialise current info space.
    m_currentInfo = new CurrentInfoViewport;

    // Make detail view dockable.
    dock = new QDockWidget(tr("Detail View"), this);
    dock->setWidget(m_workspaced1);
    dock->setObjectName("DetailWorkspace");
    addDockWidget(Qt::RightDockWidgetArea, dock);
    detailAct = dock->toggleViewAction();

    // Make current info widget dockable.
    dock = new QDockWidget(tr("Current Info"), this);
    dock->setWidget(m_currentInfo);
    dock->setObjectName("InfoView");
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    infoAct = dock->toggleViewAction();

    // Initialise workspace for both image views.
    m_workspace1 = new Workspace;         // Workspace used for original image loaded by the user.
    m_workspace2 = new Workspace;         // Workspace used for displaying processed image.

    // Set instance names for the two workspaces.
    m_workspace1->setName("Workspace1");
    m_workspace2->setName("Workspace2");

    // We only accept edits in the first workspace.
    //m_workspace2->setEnabled(false);

    // Initialise scenes.
    m_scene1 = new QGraphicsScene();
    m_scene2 = new QGraphicsScene();

    // Initialise pixel maps for each workspace.
    m_item1 = new QGraphicsPixmapItem();
    m_item2 = new QGraphicsPixmapItem();    

    // Create a scene using the image chosen.
    m_scene1->addItem(m_item1);
    m_scene2->addItem(m_item2);

    // Connect signals slots from both workspaces.
    connect(m_workspace1, SIGNAL(mustUpdate()), this, SLOT(updateS()));
    connect(m_workspace1, SIGNAL(mouseMove(QString)), this, SLOT(detailValues(QString)));
    connect(m_workspace2, SIGNAL(mouseMove(QString)), this, SLOT(detailValues(QString)));
    connect(m_workspace1, SIGNAL(zoomEvent(QWheelEvent)), m_workspace2, SLOT(zoomUpdate(QWheelEvent)));
    //connect(m_workspace2, SIGNAL(zoomEvent(QWheelEvent)), m_workspace1, SLOT(zoomUpdate(QWheelEvent)));

    // Connect signals with detail view.
    connect(this, SIGNAL(windowResized()), m_workspaced1, SLOT(onResize()));

    // Main display layout.
    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setMargin(2);

    // Separator.
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    // Add Widgets to main layout.
    m_mainLayout->addWidget(m_workspace1);
    m_mainLayout->addWidget(m_workspace2);
    //m_mainLayout->addWidget(line);
    //m_mainLayout->addWidget(m_workspaced1);

    // Use main layout in central widget.
    m_widget->setLayout(m_mainLayout);

    // We do not want to start with frozen detail views.
    m_freeze = true;

    this->acceptDrops();            // We want to support drag and drop functionality.

    // Initialise interactive pointers.
    initInteractivePointers();
}

//-------------------------------------------------------------------------------------------------------

//--Destructor-------------------------------------------------------------------------------------------
DoubleViewport::~DoubleViewport()
{

}

//--Load Image-------------------------------------------------------------------------------------------
void DoubleViewport::loadImage(QString filename)
{    
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Here we initialise our images and pixel maps.
    m_image1 = new QImage(filename);
    m_image2 = new QImage(filename);
    m_item1->setPixmap(QPixmap::fromImage(*m_image1));
    m_item2->setPixmap(QPixmap::fromImage(*m_image2));

    // Add scene to both workspaces.
    m_workspace1->setScene(m_scene1);
    m_workspace2->setScene(m_scene2);

    QApplication::restoreOverrideCursor();
}

bool DoubleViewport::loadImage(QImage filename1, QImage filename2)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    *m_image1 = filename1;
    *m_image2 = filename2;
    m_item1->setPixmap(QPixmap::fromImage(*m_image1));
    m_item2->setPixmap(QPixmap::fromImage(*m_image2));

    // Add scene to both workspaces.
    m_workspace1->setScene(m_scene1);
    m_workspace2->setScene(m_scene2);

    // Update Workspaces.
    updateS();

    // Make sure everything gets applied.
    this->resize(size().width(), size().height() + 1);
    this->resize(size().width(), size().height() - 1);

    QApplication::restoreOverrideCursor();

    return true;
}

//-------------------------------------------------------------------------------------------------------

//--Interactive Pointers---------------------------------------------------------------------------------
void DoubleViewport::initInteractivePointers()
{
    if (!m_firstTime)
    {
        delete m_interactivepointer1;
        delete m_interactivepointer2;

        m_firstTime = false;
    }

    // Initialise interactive pointers.
    m_interactivepointer1 = new InteractivePointer;
    m_interactivepointer2 = new InteractivePointer;

    // Pointer visibility is automatically set when mouse enters/leaves a workspace.
    m_interactivepointer1->setVisible(false);
    m_interactivepointer2->setVisible(false);

    // Set an initial position at the center of each viewport for the pointers.
    m_interactivepointer1->updatePosition(m_scene1->width() / 2, m_scene1->height() / 2);
    m_interactivepointer2->updatePosition(m_scene1->width() / 2, m_scene1->height() / 2);

    // Add interactive pointers.
    m_labelProxy1 = m_scene1->addWidget(m_interactivepointer1);
    m_labelProxy2 = m_scene2->addWidget(m_interactivepointer2);
}

void DoubleViewport::setPointerSize(int _pointerDimX, int _pointerDimY, int _pointerWindowSize)
{
    m_interactivepointer1->setPointerSizeX(_pointerDimX);
    m_interactivepointer1->setPointerSizeY(_pointerDimY);
    m_interactivepointer1->setWinSize(_pointerWindowSize);

    m_interactivepointer2->setPointerSizeX(_pointerDimX);
    m_interactivepointer2->setPointerSizeY(_pointerDimY);
    m_interactivepointer2->setWinSize(_pointerWindowSize);
}

//-------------------------------------------------------------------------------------------------------

//--Update Visible Geometry For Workspace2---------------------------------------------------------------
void DoubleViewport::updateS()
{
    // Get visible area relative to the scene for workspace1.
    QRectF viewport1 = m_workspace1->mapToScene(m_workspace1->viewport()->geometry()).boundingRect();

    // Synchronise workspace2
    m_workspace2->setSceneRect(viewport1);

    // We also want to use the same transformation matrix in user zooms in or applys any other
    // transformation.
    m_workspace2->setMatrix(m_workspace1->matrix());

    m_workspace1->update();
    m_workspace2->update();
}

//-------------------------------------------------------------------------------------------------------

//--Resize Event-----------------------------------------------------------------------------------------
void DoubleViewport::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event); // Now that we cought the event we still want it to be processed as it
                                    // normally would.

   computeViewingPercentage();      // Compute a new value for the visible part of the image.

   updateS();                       // A resize can still change the workspace scene geometry so it is a
                                    // good idea to synchronise workspace2.

   m_workspaced1->onResize();

   emit windowResized();            // Tell whomever is listening that the window was resized.

   //QMainWindow::resizeEvent(event);
}

//-------------------------------------------------------------------------------------------------------

//--Space Press Event------------------------------------------------------------------------------------
/*
 * Freeze detail values if space pressed.
 */
void DoubleViewport::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Space)
   {
        m_freeze = !m_freeze;
   }
}

//-------------------------------------------------------------------------------------------------------

//--Set/Get Global Variables-----------------------------------------------------------------------------

// Set-Get visible image part.
void DoubleViewport::setImageViewingPercentage(const float &imageViewingPercentage_)
{
    m_imageViewingPercentage = imageViewingPercentage_;
}

float DoubleViewport::imageViewingPercentage() const
{
    return m_imageViewingPercentage;
}

// Set-Get detailet view window size.
void DoubleViewport::setWindowSize(const int &windowSize_)
{
    m_windowSize = windowSize_;
}

int DoubleViewport::windowSize() const
{
    return m_windowSize;
}

// Set-Get interactive pointer state.
void DoubleViewport::setUseInteractivePointer(const bool &useInteractivePointer_)
{
    m_useInteractivePointer = useInteractivePointer_;
}

bool DoubleViewport::useInteractivePointer() const
{
    return m_useInteractivePointer;
}

// Set-Get interactive pointer mode.
void DoubleViewport::setInteractivePointerMode(const int &interactivePointerMode_)
{
    m_interactivePointerMode = interactivePointerMode_;
}

int DoubleViewport::interactivePointerMode() const
{
    return m_interactivePointerMode;
}

//-------------------------------------------------------------------------------------------------------

//--Helper Functions-------------------------------------------------------------------------------------
void DoubleViewport::computeViewingPercentage()
{
    float sceneWidth    = m_workspace1->scene()->width();   // Image width.
    float sceneHeight   = m_workspace1->scene()->height();  // Image height.
    float visibleWidth  = m_workspace1->width();            // Visible part width.
    float visibleHeight = m_workspace1->height();           // Visible part height.

    // Get scene position relative to viewport.
    // m_workspace1->mapFromScene(m_scene1->itemsBoundingRect());

    // Compute an aproximation of how much of the image is visible.
    float tempVal = (100 * (qMin(visibleWidth, sceneWidth) * qMin(visibleHeight, sceneHeight)) / (sceneWidth * sceneHeight));

    // Update Value.
    setImageViewingPercentage(tempVal);
    m_currentInfo->updatePerc(imageViewingPercentage());
}

void DoubleViewport::zoomControl(int delta)
{
    double scaleFactor = 1.15;

    if (delta < 0)          // Zoom in.
    {
        m_workspace1->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        m_workspace2->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
    else if (delta > 0)     // Zoom out.
    {
        m_workspace1->scale(scaleFactor, scaleFactor);
        m_workspace2->scale(scaleFactor, scaleFactor);
    }
    else                    // Zoom fit.
    {
        m_workspace1->fitInView(
            m_workspace1->scene()->itemsBoundingRect(),
            Qt::KeepAspectRatio);
        m_workspace2->fitInView(
            m_workspace2->scene()->itemsBoundingRect(),
            Qt::KeepAspectRatio);
    }
    updateS();
}

QList<int> DoubleViewport::pixelValueAt(int x, int y)
{
    QColor tempColor;
    QList<int> pixelValues;

    /*QRgb *rowData = (QRgb*)m_image2->scanLine(y);
    QRgb pixelData = rowData[x];
    int red = qRed(pixelData);

    pixelValues << qRed(pixelData);
    pixelValues << qGreen(pixelData);
    pixelValues << qBlue(pixelData);*/

    tempColor = QColor(m_image2->pixel(x, y));
    pixelValues << tempColor.red();
    pixelValues << tempColor.green();
    pixelValues << tempColor.blue();

    return pixelValues;
}

void DoubleViewport::pixelValueAt(int x, int y, QList<int> pVal)
{
    m_image2->setPixel(x, y, qRgb(pVal[0], pVal[1], pVal[2]));
}

int DoubleViewport::imageWidth()
{
    return m_image2->width();
}

int DoubleViewport::imageHeight()
{
    return m_image2->height();
}

void DoubleViewport::updatePixelMap()
{
    m_item2->setPixmap(QPixmap::fromImage(*m_image2));
}

//-------------------------------------------------------------------------------------------------------

//--Detail Values----------------------------------------------------------------------------------------
void DoubleViewport::detailValues(QString source)
{
    int mouseX, mouseY, counter;
    QColor tempColor;

    // qDebug() << "Mouse over " + source;
    if (source == "Workspace1")
    {
        mouseX = m_workspace1->mousePosX();
        mouseY = m_workspace1->mousePosY();
        QPointF relativePos = m_workspace1->mapToScene(QPoint(mouseX, mouseY));
        mouseX = relativePos.x();
        mouseY = relativePos.y();
        //qDebug() << relativePos;
    }
    else
    {
        mouseX = m_workspace2->mousePosX();
        mouseY = m_workspace2->mousePosY();
        QPointF relativePos = m_workspace2->mapToScene(QPoint(mouseX, mouseY));
        mouseX = relativePos.x();
        mouseY = relativePos.y();
        //qDebug() << relativePos;
    }

    int tempWinSize;                                    // Temporarily store the window size.
    tempWinSize = (windowSize() > m_interactivepointer1->winSize()) ?
                windowSize() : m_interactivepointer1->winSize();

    QVector<int> window1(tempWinSize * tempWinSize * 3);// Vector holding original image values
                                                        // for current window.
    QVector<int> window2(tempWinSize * tempWinSize * 3);// Vector holding edited image values
                                                        // for current window.
    counter = 0;

    for (int i = mouseX - ((tempWinSize - 1) / 2); i <= mouseX + ((tempWinSize - 1) / 2); i++)
    {
        for (int j = mouseY - ((tempWinSize - 1) / 2); j <= mouseY + ((tempWinSize - 1) / 2); j++)
        {
            if (i > 0 && i < m_image1->width() && j > 0 && j < m_image1->height())
            {
                // Set values for workspace1.
                tempColor = QColor(m_image1->pixel(i, j));
                window1[counter + 0] = tempColor.red();
                window1[counter + 1] = tempColor.green();
                window1[counter + 2] = tempColor.blue();
                // Set values for workspace2.
                tempColor = QColor(m_image2->pixel(i, j));
                window2[counter + 0] = tempColor.red();
                window2[counter + 1] = tempColor.green();
                window2[counter + 2] = tempColor.blue();                
            }
            else
            {
                // Set values for workspace1.
                window1[counter + 0] = 0;
                window1[counter + 1] = 0;
                window1[counter + 2] = 0;
                // Set values for workspace1.
                window2[counter + 0] = 0;
                window2[counter + 1] = 0;
                window2[counter + 2] = 0;
            }
            counter = counter + 3;
        }
    }

    if (m_freeze)
    {
        // Update detail view with new values.
        m_workspaced1->update(window1, window2, windowSize(), mouseX, mouseY);

        // Update current info values.
        m_currentInfo->update(window1, window2, windowSize(), mouseX, mouseY);
    }

    // If user wants to use interactive pointers.
    if (m_useInteractivePointer)
    {
        // Decide if interactive pointer should be visible or not.
        if (QRect(0, 0, m_image1->width(), m_image2->height()).contains(QPoint(mouseX, mouseY)))
        {
            m_interactivepointer1->setVisible(true);
            m_interactivepointer2->setVisible(true);

            m_Pixmap = new QPixmap(":/images/empty.png");
            m_CurrentCursor = new QCursor(*m_Pixmap);
            QApplication::changeOverrideCursor(*m_CurrentCursor);
            delete m_CurrentCursor;
            delete m_Pixmap;
        }
        else
        {
            m_interactivepointer1->setVisible(false);
            m_interactivepointer2->setVisible(false);

            m_Pixmap = new QPixmap(":/images/unclicked.png");
            m_CurrentCursor = new QCursor(*m_Pixmap);
            QApplication::changeOverrideCursor(*m_CurrentCursor);
            delete m_CurrentCursor;
            delete m_Pixmap;
        }

        // Fix interactive pointer position.
        if ((mouseX - m_interactivepointer1->width() / 2) < 0)
            mouseX = (m_interactivepointer1->width() / 2);
        else if ((mouseX + m_interactivepointer1->width() / 2) > m_image1->width())
            mouseX = m_image1->width() - (m_interactivepointer1->width() / 2);
        if ((mouseY - m_interactivepointer1->height() / 2) < 0)
            mouseY = (m_interactivepointer1->height() / 2);
        else if ((mouseY + m_interactivepointer1->height() / 2) > m_image2->height())
            mouseY = m_image2->height() - (m_interactivepointer1->height() / 2);

        // Update interactive pointer.
        m_interactivepointer1->update(mouseX, mouseY, window1, window2,
                                      interactivePointerMode());
        m_interactivepointer2->update(mouseX, mouseY, window2, window1,
                                      interactivePointerMode());
    }    
    else    // Make sure interactive points are hidden.
    {
        m_interactivepointer1->setVisible(false);
        m_interactivepointer2->setVisible(false);
    }
}

void DoubleViewport::updateFilenames(QString file1, QString file2)
{
    m_currentInfo->updateNames(file1, file2);
}

//-------------------------------------------------------------------------------------------------------
