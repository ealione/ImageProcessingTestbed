#include "alg_defaulteffects.h"

Alg_DefaultEffects::Alg_DefaultEffects()
{
}

void Alg_DefaultEffects::applyNegative(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = 255 - oldColor.red();
            g = 255 - oldColor.green();
            b = 255 - oldColor.blue();

            // Check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyGreyScale(QImage *image1, QImage *image2)
{
    QColor oldColor;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));
            int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
            image2->setPixel(x, y, qRgb(average, average, average));
        }
    }
}

void Alg_DefaultEffects::applyBrightness(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyWarm(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyCool(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue() + delta;

            //we check if the new value is between 0 and 255
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applySaturation(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    QColor newColor;
    int h, s, l;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            newColor = oldColor.toHsl();
            h = newColor.hue();
            s = newColor.saturation()+delta;
            l = newColor.lightness();

            //we check if the new value is between 0 and 255
            s = qBound(0, s, 255);

            newColor.setHsl(h, s, l);

            image2->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }
}

void Alg_DefaultEffects::applyBlur(QImage *image1, QImage *image2)
{
    int kernel [5][5]= {{0,0,1,0,0},
                        {0,1,3,1,0},
                        {1,3,7,3,1},
                        {0,1,3,1,0},
                        {0,0,1,0,0}};
    int kernelSize = 5;
    int sumKernel = 27;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {

            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i<= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j<= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));

        }
    }
}

void Alg_DefaultEffects::applySharpen(QImage *image1, QImage *image2)
{
    int kernel [3][3]= {{ 0,-1, 0},
                        {-1, 5,-1},
                        { 0,-1, 0}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {

            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyEdges(QImage *image1, QImage *image2)
{
    int kernel [3][3]= {{-1, -1, -1},
                        {-1,  8, -1},
                        {-1, -1, -1}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {
            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applySepia(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() * 0.393 + oldColor.green() * 0.769 + oldColor.blue() * 0.189;
            g = oldColor.red() * 0.349 + oldColor.green() * 0.686 + oldColor.blue() * 0.168;
            b = oldColor.red() * 0.272 + oldColor.green() * 0.534 + oldColor.blue() * 0.131;

            //we check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyPixelate(QImage *image1, QImage *image2, int size)
{
    int r, g, b, count, avg1, avg2, avg3;
    QColor color;

    for (int y = 0; y < image1->height(); y += size)
    {
        for (int x = 0; x < image1->width(); x += size)
        {
            r = 0;
            g = 0;
            b = 0;
            count = 0;

            for (int yi = 0; yi < size && y + yi < image1->height(); yi++)
            {
                for (int xi = 0; xi < size && x + xi < image1->width(); xi++)
                {
                    color = QColor(image1->pixel(x + xi, y + yi));
                    r += color.red();
                    g += color.green();
                    b += color.blue();
                    count++;
                }
            }

            avg1 = r / count;
            avg2 = g / count;
            avg3 = b / count;

            for (int yi = 0; yi < size && y + yi < image1->height(); yi++)
            {
                for (int xi = 0; xi < size && x + xi < image1->width(); xi++)
                {
                    image2->setPixel(x + xi, y + yi, qRgb(avg1, avg2, avg3));
                }
            }
        }
    }
}

void Alg_DefaultEffects::applyRed(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = 0;
            b = 0;

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyContrast(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue();

            r = (r > 128) ? (r * 1.2) : (r / 1.2);
            g = (g > 128) ? (g * 1.2) : (g / 1.2);
            b = (b > 128) ? (b * 1.2) : (b / 1.2);

            //we check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyErosion(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int buff[9];

    int *output = new int[image1->width() * image1->height()];

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {

            int i = 0;
            for (int ty = y - 1; ty <= y + 1; ty++){
               for (int tx = x - 1; tx <= x + 1; tx++){
                   if(ty >= 0 && ty < image1->height() && tx >= 0 && tx < image1->width()){
                       //pixel under the mask
                       oldColor = QColor(image1->pixel(tx, ty));
                       buff[i] = oldColor.red();
                       i++;
                   }
               }
            }

            int max = 0;
            for (int i = 1; i < 9; i++)
            {
                if (buff[i] > max)
                    max = buff[i];
            }
            output[x + y * image1->width()] = max;
        }
    }

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {
            int v = output[x + y * image1->width()];
            image2->setPixel(x, y, qRgb(v, v, v));
        }
    }
    delete[] output;
}

void Alg_DefaultEffects::applyDilation(QImage *image1, QImage *image2)
{
    int kernel [9] = {0, 1, 0, 1,  1, 1, 0, 1, 0};
    int kernelSize = 3;

    int width = image1->width();
    int height = image1->height();

    QColor oldColor;

    //int buff[kernelSize * kernelSize];
    //int output[image1->width() * image1->height()];

    int *buff = new int[kernelSize * kernelSize];
    int *output = new int[image1->width() * image1->height()];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int i = 0;
            for (int ty = y - kernelSize/2, mr = 0; ty <= y + kernelSize/2; ty++, mr++)
            {
                for (int tx = x - kernelSize/2, mc = 0; tx <= x + kernelSize/2; tx++, mc++)
                {

                   if (ty >= 0 && ty < height && tx >= 0 && tx < width)
                   {
                        if (kernel[mc + mr * kernelSize] != 1)
                        {
                            continue;
                        }
                        //pixel under the mask
                        oldColor = QColor(image1->pixel(tx, ty));
                        buff[i] = oldColor.red();
                        i++;
                    }
                }
            }

            int min = 400;
            for (int i = 1; i < 9; i++)
            {
                if (buff[i] < min)
                    min = buff[i];
            }

            output[x + y * image1->width()] = min;
        }
    }

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {
            int v = output[x + y * image1->width()];
            image2->setPixel(x, y, qRgb(v, v, v));
        }
    }
    delete[] buff;
    delete[] output;
}

void Alg_DefaultEffects::applyClosing(QImage *image1, QImage *image2)
{
    applyDilation(image1, image2);
    applyErosion(image1, image2);
}

// If we have a new compiler
/*#include "alg_defaulteffects.h"

Alg_DefaultEffects::Alg_DefaultEffects()
{
}

void Alg_DefaultEffects::applyNegative(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = 255 - oldColor.red();
            g = 255 - oldColor.green();
            b = 255 - oldColor.blue();

            // Check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyGreyScale(QImage *image1, QImage *image2)
{
    QColor oldColor;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));
            int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
            image2->setPixel(x, y, qRgb(average, average, average));
        }
    }
}

void Alg_DefaultEffects::applyBrightness(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyWarm(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyCool(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue() + delta;

            //we check if the new value is between 0 and 255
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applySaturation(QImage *image1, QImage *image2, int delta)
{
    QColor oldColor;
    QColor newColor;
    int h, s, l;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            newColor = oldColor.toHsl();
            h = newColor.hue();
            s = newColor.saturation()+delta;
            l = newColor.lightness();

            //we check if the new value is between 0 and 255
            s = qBound(0, s, 255);

            newColor.setHsl(h, s, l);

            image2->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }
}

void Alg_DefaultEffects::applyBlur(QImage *image1, QImage *image2)
{
    int kernel [5][5]= {{0,0,1,0,0},
                        {0,1,3,1,0},
                        {1,3,7,3,1},
                        {0,1,3,1,0},
                        {0,0,1,0,0}};
    int kernelSize = 5;
    int sumKernel = 27;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {

            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i<= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j<= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));

        }
    }
}

void Alg_DefaultEffects::applySharpen(QImage *image1, QImage *image2)
{
    int kernel [3][3]= {{ 0,-1, 0},
                        {-1, 5,-1},
                        { 0,-1, 0}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {

            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyEdges(QImage *image1, QImage *image2)
{
    int kernel [3][3]= {{-1, -1, -1},
                        {-1,  8, -1},
                        {-1, -1, -1}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r, g, b;
    QColor color;

    for (int x = kernelSize / 2; x < image1->width() - (kernelSize / 2); x++)
    {
        for (int y = kernelSize / 2; y < image1->height() - (kernelSize / 2); y++)
        {
            r = 0;
            g = 0;
            b = 0;

            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++)
                {
                    color = QColor(image1->pixel(x + i, y + j));
                    r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }

            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applySepia(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red() * 0.393 + oldColor.green() * 0.769 + oldColor.blue() * 0.189;
            g = oldColor.red() * 0.349 + oldColor.green() * 0.686 + oldColor.blue() * 0.168;
            b = oldColor.red() * 0.272 + oldColor.green() * 0.534 + oldColor.blue() * 0.131;

            //we check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyPixelate(QImage *image1, QImage *image2, int size)
{
    int r, g, b, count, avg1, avg2, avg3;
    QColor color;

    for (int y = 0; y < image1->height(); y += size)
    {
        for (int x = 0; x < image1->width(); x += size)
        {
            r = 0;
            g = 0;
            b = 0;
            count = 0;

            for (int yi = 0; yi < size && y + yi < image1->height(); yi++)
            {
                for (int xi = 0; xi < size && x + xi < image1->width(); xi++)
                {
                    color = QColor(image1->pixel(x + xi, y + yi));
                    r += color.red();
                    g += color.green();
                    b += color.blue();
                    count++;
                }
            }

            avg1 = r / count;
            avg2 = g / count;
            avg3 = b / count;

            for (int yi = 0; yi < size && y + yi < image1->height(); yi++)
            {
                for (int xi = 0; xi < size && x + xi < image1->width(); xi++)
                {
                    image2->setPixel(x + xi, y + yi, qRgb(avg1, avg2, avg3));
                }
            }
        }
    }
}

void Alg_DefaultEffects::applyRed(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = 0;
            b = 0;

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyContrast(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            oldColor = QColor(image1->pixel(x, y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue();

            r = (r > 128) ? (r * 1.2) : (r / 1.2);
            g = (g > 128) ? (g * 1.2) : (g / 1.2);
            b = (b > 128) ? (b * 1.2) : (b / 1.2);

            //we check if the new value is between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image2->setPixel(x, y, qRgb(r, g, b));
        }
    }
}

void Alg_DefaultEffects::applyErosion(QImage *image1, QImage *image2)
{
    QColor oldColor;
    int buff[9];
    int output[image1->width() * image1->height()];

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {

            int i = 0;
            for (int ty = y - 1; ty <= y + 1; ty++){
               for (int tx = x - 1; tx <= x + 1; tx++){
                   if(ty >= 0 && ty < image1->height() && tx >= 0 && tx < image1->width()){
                       //pixel under the mask
                       oldColor = QColor(image1->pixel(tx, ty));
                       buff[i] = oldColor.red();
                       i++;
                   }
               }
            }

            int max = 0;
            for (int i = 1; i < 9; i++)
            {
                if (buff[i] > max)
                    max = buff[i];
            }
            output[x + y * image1->width()] = max;
        }
    }

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {
            int v = output[x + y * image1->width()];
            image2->setPixel(x, y, qRgb(v, v, v));
        }
    }
}

void Alg_DefaultEffects::applyDilation(QImage *image1, QImage *image2)
{
    int kernel [9] = {0, 1, 0, 1,  1, 1, 0, 1, 0};
    int kernelSize = 3;

    int width = image1->width();
    int height = image1->height();

    QColor oldColor;

    int buff[kernelSize * kernelSize];
    int output[image1->width() * image1->height()];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int i = 0;
            for (int ty = y - kernelSize/2, mr = 0; ty <= y + kernelSize/2; ty++, mr++)
            {
                for (int tx = x - kernelSize/2, mc = 0; tx <= x + kernelSize/2; tx++, mc++)
                {

                   if (ty >= 0 && ty < height && tx >= 0 && tx < width)
                   {
                        if (kernel[mc + mr * kernelSize] != 1)
                        {
                            continue;
                        }
                        //pixel under the mask
                        oldColor = QColor(image1->pixel(tx, ty));
                        buff[i] = oldColor.red();
                        i++;
                    }
                }
            }

            int min = 400;
            for (int i = 1; i < 9; i++)
            {
                if (buff[i] < min)
                    min = buff[i];
            }

            output[x + y * image1->width()] = min;
        }
    }

    for (int y = 0; y < image1->height(); y++)
    {
        for (int x = 0; x < image1->width(); x++)
        {
            int v = output[x + y * image1->width()];
            image2->setPixel(x, y, qRgb(v, v, v));
        }
    }
}

void Alg_DefaultEffects::applyClosing(QImage *image1, QImage *image2)
{
    applyDilation(image1, image2);
    applyErosion(image1, image2);
}*/
