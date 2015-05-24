#include "currentinfoviewport.h"

CurrentInfoViewport::CurrentInfoViewport(QWidget *parent)
    : QWidget(parent)
{
    // Initialise labels.
    m_fileNameLabel = new QLabel("-- / --");
    m_positionLabel = new QLabel("Position (x, y)");
    m_colorLabel1 = new QLabel("O - R:0 G:0 B:0");
    m_colorLabel2 = new QLabel("E - R:0 G:0 B:0");
    m_percentageLabel = new QLabel("000%");
    m_infoLabel = new QLabel(".:Display your info here:.");

    // Align labels content.
    m_fileNameLabel->setAlignment(Qt::AlignCenter);
    m_positionLabel->setAlignment(Qt::AlignCenter);
    m_colorLabel1->setAlignment(Qt::AlignCenter);
    m_colorLabel2->setAlignment(Qt::AlignCenter);
    m_percentageLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setAlignment(Qt::AlignCenter);

    // Hold some space for algorithm specific values.
    //m_infoLabel->setMinimumHeight(50);

    // Separator.
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // Info display layout.
    m_infoLayout = new QVBoxLayout;
    m_infoLayout->setMargin(0);

    // Use info layout..
    this->setLayout(m_infoLayout);

    // Add Widgets to info layout.
    m_infoLayout->addWidget(m_fileNameLabel);
    m_infoLayout->addWidget(m_positionLabel);
    m_infoLayout->addWidget(m_colorLabel1);
    m_infoLayout->addWidget(m_colorLabel2);
    m_infoLayout->addWidget(m_percentageLabel);
    m_infoLayout->addWidget(line);
    m_infoLayout->addStretch(1);
    m_infoLayout->addWidget(m_infoLabel);
}

void CurrentInfoViewport::update(QVector<int> val1, QVector<int> val2, int winSize, int mouseX, int mouseY)
{
    // Update label values.
    int midVal = ((winSize * winSize * 3) - 1) / 2;
    m_positionLabel->setText("Position (" + QString::number(mouseX) +
                             "," + QString::number(mouseY) + ")");
    m_colorLabel1->setText("O - R:" + QString::number(val1[midVal + 0]) +
            " G:" + QString::number(val1[midVal + 1]) +
            " B:" + QString::number(val1[midVal + 2]));
    m_colorLabel2->setText("E - R:" + QString::number(val2[midVal + 0]) +
            " G:" + QString::number(val2[midVal + 1]) +
            " B:" + QString::number(val2[midVal + 2]));
    m_infoLabel->setText(".:Specific algorithm info here:.");
}

void CurrentInfoViewport::updatePerc(float perc)
{
    m_percentageLabel->setText(QString::number(perc) + "%");
}

void CurrentInfoViewport::updateNames(QString file1, QString file2)
{
    if (file1 == "")
        file1 = "Unknown";
    if (file2 == "")
        file2 = "Unknown";

    QFileInfo baseName(file1);
    file1 = baseName.completeBaseName();
    QFileInfo baseName2(file2);
    file2 = baseName2.completeBaseName();

    m_fileNameLabel->setText("1:" + file1 + " ~ " + "2:" + file2);
}
