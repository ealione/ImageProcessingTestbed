#include "detailvalues.h"

DetailValues::DetailValues()
{
    this->setMaximumWidth(400);                                 // We do not need this component to get
    this->setMaximumHeight(400);                                // very big.
    this->setMinimumWidth(200);                                 // We do not need this component to get
    this->setMinimumHeight(200);                                // very small.
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff );// No Horizontal scrollbars.
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );  // No Vertical scrollbars.

    //m_table = new QTableWidget();
}

void DetailValues::update(QVector<int> val1, QVector<int> val2, int winSize)
{
    int midPoint1 = (winSize  - 1) / 2;  // Middle point of incoming data.
    int winSizeTemp = winSize;           // Store old window size.

    // It is not practical displaying values for too big windows.
    if (winSize > 9)
        winSize = 9;

    int midPoint2 = (winSize - 1) / 2;  // New middle point. Different only if
                                        // winSize changes in the above line.

    // Configure the table to cover our current needs.
    this->setColumnCount(winSize);          // Number of needed columns.
    this->setRowCount(winSize);             // Number of rows.
    this->verticalHeader()->hide();         // Do not show vertical headers.
    this->horizontalHeader()->hide();       // Do not show horizontal headers.

    int pWinFrom    = midPoint1 - midPoint2;// Pixel value window can only display so much values.
    int pWinTo      = midPoint1 + midPoint2;

    float step = this->width() / winSize;       // Vertical and horizontal step for each
                                                // new element.
    int counter;                                // Meter used to traverse vactor.

    for (int j = pWinFrom; j <= pWinTo; j++)
    {
        int jj = j - pWinFrom;                  // Fix range for table view.
        this->setRowHeight(jj, step);           // Row height so as not to go beyond available area.
        for (int i = pWinFrom; i <= pWinTo; i++)
        {
            int ii = i - pWinFrom;              // Fix range for table view.
            this->setColumnWidth(ii, step);     // Column width so as not to go beyond available area.

            // Move to next 3 values.
            counter = ((j * winSizeTemp + i) * 3);   // Steps in value vectors.

            // It is convenient and more sensible to display grayscale values.
            int grayColor1 = 0.21 * val1[counter + 0] +
                             0.72 * val1[counter + 1] +
                             0.07 * val1[counter + 2];

            int grayColor2 = 0.21 * val2[counter + 0] +
                             0.72 * val2[counter + 1] +
                             0.07 * val2[counter + 2];

            // Add new item.
            this->setItem(ii, jj, new QTableWidgetItem(QString::number(grayColor1)));

            // Align item.
            this->model()->setData(this->model()->index(ii, jj), Qt::AlignCenter,
                                   Qt::TextAlignmentRole);

            // Set text color according to pixel relative values.
            if (grayColor1 > grayColor2)
                this->item(ii, jj)->setTextColor(QColor(255, 221, 170));
            else if (grayColor1 < grayColor2)
                this->item(ii, jj)->setTextColor(QColor(45, 255, 76));
            else
                this->item(ii, jj)->setTextColor(Qt::black);

            // Edit font size so as pixel values can fit in their cells.
            if (winSize > 6)
                this->item(ii, jj)->setFont(QFont("Helvetica", 10));
            if (winSize > 7)
                this->item(ii, jj)->setFont(QFont("Helvetica", 6));
        }
    }
}
