#ifndef DETAILVALUES_H
#define DETAILVALUES_H

#include <QtWidgets>

class DetailValues : public QTableWidget
{
    Q_OBJECT

public:
    DetailValues();

    void update(QVector<int> val1, QVector<int> val2, int winSize);
};

#endif // DETAILVALUES_H
