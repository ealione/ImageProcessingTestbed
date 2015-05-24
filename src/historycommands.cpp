#include "historycommands.h"
#include <QDebug>
HistoryCommands::HistoryCommands(QObject *parent) :
    QObject(parent)
{
}

void HistoryCommands::alg_newCommand(QString command, QString filename,
                                     QImage *image1, QImage *image2, QListWidget &list)
{
    QString uuid = QUuid::createUuid().toString();

    if (filename.isEmpty())
        filename = "Unsaved";
    else
    {
        QFileInfo baseName(filename);
        filename = baseName.completeBaseName();
    }

    // Current size of the history list.
    int curSize = list.count();

    // Currently selected index from the history list.
    int curInd = list.currentRow();
    if (curInd == -1)
        curInd = curSize - 1;

    if (curInd < curSize - 1)
    {
        for (int i = curInd; i <= curSize; i++)
        {
            delete list.item(curInd);
            m_historyList.remove(curInd);
        }
        list.addItem(QString(command + "~" + filename));
        m_historyList[curInd] = uuid;

        list.setCurrentRow(curInd);

        image1->save(QString(QDir::currentPath() + "/frames/" + uuid + "original"), "PNG", -1);
        image2->save(QString(QDir::currentPath() + "/frames/" + uuid + "edited"), "PNG", -1);
    }
    else
    {
        if (curSize == 0)
        {
            list.addItem(QString(command + "~" + filename));
            m_historyList[0] = uuid;

            list.setCurrentRow(0);

            image1->save(QString(QDir::currentPath() + "/frames/" + uuid + "original"), "PNG", -1);
            image2->save(QString(QDir::currentPath() + "/frames/" + uuid + "edited"), "PNG", -1);
        }
        else if (curSize > 0 && curSize < maxItems())
        {
            list.addItem(QString(command + "~" + filename));
            m_historyList[curInd + 1] = uuid;

            list.setCurrentRow(curInd + 1);

            image1->save(QString(QDir::currentPath() + "/frames/" + uuid + "original"), "PNG", -1);
            image2->save(QString(QDir::currentPath() + "/frames/" + uuid + "edited"), "PNG", -1);
        }
        else if (curSize >= maxItems())
        {
            delete list.item(0);
            for (int i = 0; i < maxItems() - 1; ++i)
            {
                m_historyList[i] = m_historyList[i + 1];
            }

            list.addItem(QString(command + "~" + filename));
            m_historyList[curInd] = uuid;

            list.setCurrentRow(curInd);

            image1->save(QString(QDir::currentPath() + "/frames/" + uuid + "original"), "PNG", -1);
            image2->save(QString(QDir::currentPath() + "/frames/" + uuid + "edited"), "PNG", -1);
        }
    }
}

QList<QImage> HistoryCommands::alg_goHere(int index, QListWidget &list)
{
    QList<QImage> imagesAtPoint;
    imagesAtPoint.append(QImage(QString(QDir::currentPath() + "/frames/" + m_historyList[index] + "original")));
    imagesAtPoint.append(QImage(QString(QDir::currentPath() + "/frames/" + m_historyList[index] + "edited")));

    list.setCurrentRow(index);

    return imagesAtPoint;
}

void HistoryCommands::setMaxItems(const int &maxItems_)  // Set and get how many image
{
    m_maxItems = maxItems_;
}

int HistoryCommands::maxItems() const                    // frames can be stored.
{
    return m_maxItems;
}
