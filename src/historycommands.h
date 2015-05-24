#ifndef HISTORYCOMMANDS_H
#define HISTORYCOMMANDS_H

#include <QObject>
#include <QImage>
#include <QUuid>
#include <QMap>
#include <QListWidget>
#include <QDir>

class HistoryCommands : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int maxItems READ maxItems WRITE setMaxItems)

public:
    explicit HistoryCommands(QObject *parent = 0);

    void alg_newCommand(QString command, QString filename, QImage *image1, QImage *image2, QListWidget &list);
    QList<QImage> alg_goHere(int index, QListWidget &list);

    void setMaxItems(const int &maxItems_);  // Set and get how many image
    int maxItems() const;                    // frames can be stored.

    int curId;
    QMap<int, QString> m_historyList;

signals:

public slots:

private:
    int m_maxItems;

};

#endif // HISTORYCOMMANDS_H
