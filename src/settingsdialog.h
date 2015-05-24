#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_cancel_pushButton_clicked();

    void on_ok_pushButton_clicked();

private:
    Ui::SettingsDialog *ui;

    int m_windowSize;
    int m_pointerDimX;
    int m_pointerDimY;
    int m_pointerWindowSize;
    int m_maxFrames;

signals:
    void settingsUpdate(int, int, int, int, int);
};

#endif // SETTINGSDIALOG_H
