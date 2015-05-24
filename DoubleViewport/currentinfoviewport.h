#ifndef CURRENTINFOVIEWPORT_H
#define CURRENTINFOVIEWPORT_H

#include <QtWidgets>

class CurrentInfoViewport : public QWidget
{
    Q_OBJECT
public:
    CurrentInfoViewport(QWidget *parent = 0);

    void update(QVector<int> val1, QVector<int> val2, int winSize, int mouseX, int mouseY);
    void updatePerc(float perc);
    void updateNames(QString file1, QString file2);

private:
    QVBoxLayout *m_infoLayout;       // Info layout used for general data display.
    QLabel *m_fileNameLabel;         // Use this label to display your image names.
    QLabel *m_positionLabel;         // Label to display mouse position.
    QLabel *m_colorLabel1;           // Label to display color at this position for original image.
    QLabel *m_colorLabel2;           // Label to display color at this position for edited image.
    QLabel *m_percentageLabel;       // Label to display image visible part.
    QLabel *m_infoLabel;             // Use this label to display your data.
};

#endif // CURRENTINFOVIEWPORT_H
