#ifndef SCRIPTSAVER_H
#define SCRIPTSAVER_H

#include <QDialog>

namespace Ui {
class ScriptSaver;
}

class ScriptSaver : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptSaver(QWidget *parent = 0);
    ~ScriptSaver();

    void setSourceCode(QString code);

private slots:
    void on_cancel_pushButton_clicked();

    void on_save_pushButton_clicked();

private:
    Ui::ScriptSaver *ui;

    QString m_name;
    QString m_author;
    int m_version;
    QString m_description;
    QString m_type;
    bool m_menuEntry;
    QString m_sourceCode;
};

#endif // SCRIPTSAVER_H
