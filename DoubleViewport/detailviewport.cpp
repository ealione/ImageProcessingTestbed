#include "detailviewport.h"

//-Constructor-------------------------------------------------------------------------------------------
DetailViewport::DetailViewport(QWidget *parent)
    : QWidget(parent)
{
    // Initialise detail workspace for both image views.
    m_workspaced1 = new DetailWorkspace;   // Workspace used for original image loaded by the user.
    m_workspaced2 = new DetailWorkspace;   // Workspace used for displaying processed image.

    // Initialise pixel values workspace for both image views.
    m_workvalues1 = new DetailValues;      // Workspace used for original image pixel values.
    m_workvalues2 = new DetailValues;      // Workspace used for displaying pixel values.

    // Main display layout.
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);

    // Zoom display layout.
    m_zoomLayout = new QHBoxLayout;
    m_zoomLayout->setMargin(0);

    // Values display layout.
    m_valueLayout = new QHBoxLayout;
    m_valueLayout->setMargin(0);   

    // Add Widgets to zoom layout.
    m_zoomLayout->addWidget(m_workspaced1);
    m_zoomLayout->addWidget(m_workspaced2);

    // Add Widgets to values layout.
    m_valueLayout->addWidget(m_workvalues1);
    m_valueLayout->addWidget(m_workvalues2);

    // Separator.
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // Add to main layout.
    m_mainLayout->addLayout(m_zoomLayout);
    m_mainLayout->addLayout(m_valueLayout);
    //m_mainLayout->addWidget(line);

    // Use main layout in central widget.
    this->setLayout(m_mainLayout);

    // Initialise views with a random Image.
    m_imaged1 = new QImage(8, 8, QImage::Format_ARGB32);
    m_imaged2 = new QImage(8, 8, QImage::Format_ARGB32);
    m_itemd1 = new QGraphicsPixmapItem(QPixmap::fromImage(*m_imaged1));
    m_itemd2 = new QGraphicsPixmapItem(QPixmap::fromImage(*m_imaged2));

    // Create a scene using data from the image chosen.
    m_scened1 = new QGraphicsScene();
    m_scened1->addItem(m_itemd1);

    // Create a scene using data from the edited image.
    m_scened2 = new QGraphicsScene();
    m_scened2->addItem(m_itemd2);

    // Add scene to both workspaces.
    m_workspaced1->setScene(m_scened1);
    m_workspaced2->setScene(m_scened2);

    // Content alignment.
    //m_workspaced1->setAlignment(Qt::AlignCenter);
    //m_workspaced2->setAlignment(Qt::AlignCenter);

    // Make sure detail zoom view updates if resized.
    connect(m_workspaced1, SIGNAL(resized()), this, SLOT(onResize()));

    // Set size policy for zoom detail workspaces.
    QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    m_workspaced1->setSizePolicy(qsp);
    m_workspaced2->setSizePolicy(qsp);
}

//-------------------------------------------------------------------------------------------------------

//--Viewport Update--------------------------------------------------------------------------------------
void DetailViewport::update(QVector<int> val1, QVector<int> val2, int winSize,
                            int mouseX, int mouseY)
{
    //qDebug() << "window";
    //qDebug() << val1;

    int originalWinSize = qSqrt(val1.count() / 3);
    int midPoint1 = (originalWinSize - 1) / 2;
    int midPoint2 = (winSize - 1) / 2;

    int pWinFrom    = midPoint1 - midPoint2;
    int pWinTo      = midPoint1 + midPoint2;

    *m_imaged1 = QImage(winSize, winSize, QImage::Format_ARGB32);
    *m_imaged2 = QImage(winSize, winSize, QImage::Format_ARGB32);

    int counter;
    int ii = 0;
    int jj = 0;
    for (int j = pWinFrom; j <= pWinTo; j++)
    {
        ii = 0;
        for (int i = pWinFrom; i <= pWinTo; i++)
        {
            // Move to next 3 values.
            counter = ((j * originalWinSize + i) * 3);   // Steps in value vectors.
            m_imaged1->setPixel(jj, ii, qRgb(val1[counter + 0], val1[counter + 1], val1[counter + 2]));
            m_imaged2->setPixel(jj, ii, qRgb(val2[counter + 0], val2[counter + 1], val2[counter + 2]));
            ii++;
        }
        jj++;
    }

    m_itemd1->setPixmap(QPixmap::fromImage(*m_imaged1));
    m_itemd2->setPixmap(QPixmap::fromImage(*m_imaged2));

    // Add scene to both workspaces.
    m_workspaced1->setScene(m_scened1);
    m_workspaced2->setScene(m_scened2);

    // Update pixel values.
    m_workvalues1->update(val1, val2, winSize);
    m_workvalues2->update(val2, val1, winSize);

    onResize();
}

//-------------------------------------------------------------------------------------------------------

//--Resize Event-----------------------------------------------------------------------------------------
void DetailViewport::onResize()
{
   m_workspaced1->fitInView(m_workspaced1->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
   m_workspaced2->fitInView(m_workspaced2->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

//-------------------------------------------------------------------------------------------------------
