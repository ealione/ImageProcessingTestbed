#ifndef ALG_DEFAULTEFFECTS_H
#define ALG_DEFAULTEFFECTS_H

#include <QColor>
#include <QImage>

class Alg_DefaultEffects : public QObject
{
    Q_OBJECT

public:
    Alg_DefaultEffects();

    static void applyNegative(QImage *image1, QImage *image2);

    static void applyGreyScale(QImage *image1, QImage *image2);

    static void applyBrightness(QImage *image1, QImage *image2, int delta);

    static void applyWarm(QImage *image1, QImage *image2, int delta);

    static void applyCool(QImage *image1, QImage *image2, int delta);

    static void applySaturation(QImage *image1, QImage *image2, int delta);

    static void applyBlur(QImage *image1, QImage *image2);

    static void applySharpen(QImage *image1, QImage *image2);

    static void applyEdges(QImage *image1, QImage *image2);

    static void applySepia(QImage *image1, QImage *image2);

    static void applyPixelate(QImage *image1, QImage *image2, int size);

    static void applyRed(QImage *image1, QImage *image2);

    static void applyContrast(QImage *image1, QImage *image2);

    static void applyErosion(QImage *image1, QImage *image2);

    static void applyDilation(QImage *image1, QImage *image2);

    static void applyClosing(QImage *image1, QImage *image2);
};

#endif // ALG_DEFAULTEFFECTS_H
