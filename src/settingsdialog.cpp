#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings(QCoreApplication::applicationDirPath() + "/state", "ImageProcessingTestbed");
    settings.beginGroup("MainWindow");
    ui->winSize_spinBox->setValue(settings.value("detailWinSize", 3).toInt());
    ui->pointerX_spinBox->setValue(settings.value("pointerSizeX", 40).toInt());
    ui->pointerY_spinBox->setValue(settings.value("pointerSizeY", 40).toInt());
    ui->deWinSize_spinBox->setValue(settings.value("pointerWinSize", 3).toInt());
    ui->maxFrames_spinBox->setValue(settings.value("maxFrames", 10).toInt());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_ok_pushButton_clicked()
{
    m_windowSize = ui->winSize_spinBox->value();
    m_pointerDimX = ui->pointerX_spinBox->value();
    m_pointerDimY = ui->pointerY_spinBox->value();
    m_pointerWindowSize = ui->deWinSize_spinBox->value();
    m_maxFrames = ui->maxFrames_spinBox->value();

    if (m_windowSize % 2 == 0)
        m_windowSize++;
    if (m_pointerWindowSize % 2 == 0)
        m_pointerWindowSize++;

    ui->winSize_spinBox->setValue(m_windowSize);
    ui->deWinSize_spinBox->setValue(m_pointerWindowSize);

    emit settingsUpdate(m_windowSize, m_pointerDimX, m_pointerDimY, m_pointerWindowSize, m_maxFrames);

    this->hide();
}

void SettingsDialog::on_cancel_pushButton_clicked()
{
    this->hide();
}
