#ifndef INTERACTIVEPOINTER_H
#define INTERACTIVEPOINTER_H

#include <QtWidgets>
#include <QtMath>

class InteractivePointer : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int pointerSizeX READ pointerSizeX WRITE setPointerSizeX)
    Q_PROPERTY(int pointerSizeY READ pointerSizeY WRITE setPointerSizeY)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(int winSize READ winSize WRITE setWinSize)
public:
    //--Constructors/Destructors--------------------------------------------------------------------------
    InteractivePointer();

    //--Update Functions----------------------------------------------------------------------------------
    void update(int posX, int posY, QVector<int> val1, QVector<int> val2, int mode);
    void updatePosition(int posX, int posY);

    //--Pointer Modes-------------------------------------------------------------------------------------
    void zoomView(QVector<int> val1, int winSize);
    void diffView(QVector<int> val1, QVector<int> val2, int winSize);
    void textView(QVector<int> valT, int winSize);

    //--Setters/Getters-----------------------------------------------------------------------------------
    void setPointerSizeX(const float &pointerSizeX_);  // Set pointer width.
    float pointerSizeX() const;                        // Get pointer width.
    void setPointerSizeY(const float &pointerSizeY_);  // Set pointer height.
    float pointerSizeY() const;                        // Get pointer height.
    void setFontSize(const int &fontSize_);            // Set pointer height.
    int fontSize() const;                              // Get pointer height.
    void setWinSize(const int &winSize_);              // Set pointer window size.
    int winSize() const;                               // Get pointer window size.

private:
    //--Variables-----------------------------------------------------------------------------------------
    QImage * m_imagep;              // Interactive pointer image zoom view.
    QPixmap m_pixmap;               // Pixelmap for the current interactive pointer.
    int m_pointerSizeX = 40;        // Width of the interactive pointer.
    int m_pointerSizeY = 40;        // Height of the interactive pointer.
    QFont m_labelFont;              // Font used by interactive pointer.
    int m_fontSize;                 // Font size.
    int m_pixelsWideT;              // This is how long each value is going to be when printed.
    int m_pixelsWideS;              // This is how long each empty space is going to be when printed.
    int m_pixelsHighT;              // This is how high each value is going to be when printed.
    int m_pixelsHighS;              // This is how high each line space is going to be when printed.
    int m_winSize;                  // Window Size for the interactive pointer.
};

#endif // INTERACTIVEPOINTER_H
