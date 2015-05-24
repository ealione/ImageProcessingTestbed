#include "interactivepointer.h"

// Use http://doc.qt.io/qt-4.8/qgraphicspixmapitem.html instead of ProxyWidget.

InteractivePointer::InteractivePointer()
{
    this->setScaledContents(true);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setFixedWidth(pointerSizeX());
    this->setFixedHeight(pointerSizeY());
    //this->setTextFormat(Qt::RichText);
    this->setMargin(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setAlignment(Qt::AlignTop);

    setPointerSizeX(40);
    setPointerSizeY(40);
    setWinSize(3);

    // Initialise an image to hold pointer data.
    m_imagep = new QImage(8, 8, QImage::Format_ARGB32);
    m_pixmap = QPixmap::fromImage(*m_imagep);

    // Set pixelmap to this label.
    this->setPixmap(m_pixmap);

    // Set font size.
    setFontSize(9);
    m_labelFont.setPointSize(fontSize());

    // Font metrics in order to adjust pointer size
    QFontMetrics fm(m_labelFont);
    m_pixelsWideT = fm.width("000");
    m_pixelsWideS = fm.width(" ");
    m_pixelsHighT = fm.height();
    m_pixelsHighS = fm.lineSpacing();
}

void InteractivePointer::update(int posX, int posY, QVector<int> val1, QVector<int> val2, int mode)
{
    // Also update current position.
    updatePosition(posX, posY);

    if (mode == 0)                      // Zoom view
    {
        this->setFixedWidth(pointerSizeX());
        this->setFixedHeight(pointerSizeY());

        zoomView(val1, winSize());
    }
    else if (mode == 1)                 // Difference view
    {
        this->setFixedWidth(pointerSizeX());
        this->setFixedHeight(pointerSizeY());

        diffView(val1, val2, winSize());
    }
    else                                // Text View
    {
        // The reason we are not adjusting the font, and/or window size here, is to help users with poor eyesight
        // to avoid small letters. So let a user find his desired font size, and adjust the pointer accordingly.        
        this->setFixedWidth(winSize() * (m_pixelsWideT + m_pixelsWideS));
        this->setFixedHeight(winSize() * (m_pixelsHighS));

        this->setLineWidth(pointerSizeY());
        textView(val1, winSize());
    }
}

void InteractivePointer::updatePosition(int posX, int posY)
{
    // Move pointer to its new position.
    this->move((posX - this->width() / 2), (posY - this->height() / 2));
}

void InteractivePointer::setPointerSizeX(const float &pointerSizeX_)  // Set pointer width.
{
    m_pointerSizeX = pointerSizeX_;
}

float InteractivePointer::pointerSizeX() const                        // Get pointer width.
{
    return m_pointerSizeX;
}

void InteractivePointer::setPointerSizeY(const float &pointerSizeY_)  // Set pointer height.
{
    m_pointerSizeY = pointerSizeY_;
}

float InteractivePointer::pointerSizeY() const                        // Get pointer height.
{
    return m_pointerSizeY;
}

void InteractivePointer::setFontSize(const int &fontSize_)            // Set font size.
{
    m_fontSize = fontSize_;
}

int InteractivePointer::fontSize() const                              // Get font size.
{
    return m_fontSize;
}

void InteractivePointer::setWinSize(const int &winSize_)              // Set font size.
{
    m_winSize = winSize_;
}

int InteractivePointer::winSize() const                               // Get font size.
{
    return m_winSize;
}

void InteractivePointer::zoomView(QVector<int> val1, int winSize)
{
    int originalWinSize = qSqrt(val1.count() / 3);
    int midPoint1 = (originalWinSize - 1) / 2;
    int midPoint2 = (winSize - 1) / 2;

    int pWinFrom    = midPoint1 - midPoint2;
    int pWinTo      = midPoint1 + midPoint2;

    // Resize image.
    *m_imagep = QImage(winSize, winSize, QImage::Format_ARGB32);

    // Update current pixels.    
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
            m_imagep->setPixel(jj, ii, qRgb(val1[counter + 0], val1[counter + 1], val1[counter + 2]));
            ii++;
        }
        jj++;
    }

    // Create pixel map.
    m_pixmap = QPixmap::fromImage(*m_imagep);

    // Set pixel map to this label.
    this->setPixmap(m_pixmap);
}

void InteractivePointer::diffView(QVector<int> val1, QVector<int> val2, int winSize)
{
    int originalWinSize = qSqrt(val1.count() / 3);
    int midPoint1 = (originalWinSize - 1) / 2;
    int midPoint2 = (winSize - 1) / 2;

    int pWinFrom    = midPoint1 - midPoint2;
    int pWinTo      = midPoint1 + midPoint2;

    // Resize image.
    *m_imagep = QImage(winSize, winSize, QImage::Format_ARGB32);

    int diff1;
    int diff2;
    int diff3;

    // Update current pixels.
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
            diff1 = val1[counter + 0] - val2[counter + 0];
            if (diff1 < 0 || diff1 > 255)
                diff1 = diff1 - 256;
            diff2 = val1[counter + 1] - val2[counter + 1];
            if (diff2 < 0 || diff2 > 255)
                diff2 = diff2 - 256;
            diff3 = val1[counter + 2] - val1[counter + 2];
            if (diff3 < 0 || diff3 > 255)
                diff3 = diff3 - 256;

            m_imagep->setPixel(jj, ii, qRgb(diff1, diff2, diff3));
            ii++;
        }
        jj++;
    }

    // Create pixel map.
    m_pixmap = QPixmap::fromImage(*m_imagep);

    // Set pixel map to this label.
    this->setPixmap(m_pixmap);
}

void InteractivePointer::textView(QVector<int> valT, int winSize)
{
    // Create subvector if needed.
    QVector<int> val1(winSize * winSize * 3);
    int originalWinSize = qSqrt(valT.count() / 3);
    int midPoint1 = (originalWinSize - 1) / 2;
    int midPoint2 = (winSize - 1) / 2;

    int pWinFrom    = midPoint1 - midPoint2;
    int pWinTo      = midPoint1 + midPoint2;

    if (midPoint1 != midPoint2)
    {
        int cnt;
        int ii = 0;
        for (int j = pWinFrom; j <= pWinTo; j++)
        {
            for (int i = pWinFrom; i <= pWinTo; i++)
            {
                // Move to next 3 values.
                cnt = ((j * originalWinSize + i) * 3);   // Steps in value vectors.

                val1[ii + 0] = valT[cnt + 0];
                val1[ii + 1] = valT[cnt + 1];
                val1[ii + 2] = valT[cnt + 2];

                ii = ii + 3;
            }
        }
    }
    else
        val1 = valT;

    // Clear any previous values.
    this->clear();

    int grayColor = 0;

    /*double stepX = this->width() / winSize;
    double stepY = this->height() / winSize;

    int mid = winSize / 2;

    double curX = 0;
    double curY = 0;*/

    int counter = 0;
    for (int i = 0; i < winSize; i++)
    {
        for (int j = 0; j < winSize; j++)
        {
            counter = (j * winSize + i) * 3;
            /*curX = curX + stepX*i;
            curY = curY + stepY*j;
            QRect rec(curX-stepX, curY-stepY, curX, curY);*/

            grayColor = 0.21 * val1[counter + 0] +
                        0.72 * val1[counter + 1] +
                        0.07 * val1[counter + 2];

            QString tempVal = "---";
            if (grayColor / 10 < 1)
                tempVal = QString("00" + QString::number(grayColor));
            else if (grayColor / 100 < 1)
                tempVal = QString("0" + QString::number(grayColor));
            else
                tempVal = QString::number(grayColor);

            this->setFont(m_labelFont);

            // If we want the pointer to remain rectangular something like this must be used
            // to uniformly fill the available space. Instead of 100 dinamically set the line
            // space based on window and font height.
            //QString line = "<p style=\"line-height:100\">";
            //line += tempVal + " ";

            this->setText(this->text() + " " + tempVal);

            //counter = counter + 3; // This will rotate the image by 90 deg.
        }

        if (i < winSize - 1)
            this->setText(this->text() + "\n");
    }
}
