#include "scriptsaver.h"
#include "ui_scriptsaver.h"
#include "scriptingconstants.h"

#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

using namespace Scripting;

ScriptSaver::ScriptSaver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptSaver)
{
    ui->setupUi(this);
}

ScriptSaver::~ScriptSaver()
{
    delete ui;
}

void ScriptSaver::on_cancel_pushButton_clicked()
{
    this->hide();
}

void ScriptSaver::on_save_pushButton_clicked()
{
    m_name = ui->name_lineEdit->text();
    m_author = ui->author_lineEdit->text();
    m_version = ui->version_spinBox->value();
    m_description = ui->description_lineEdit->text();
    m_type = ui->type_lineEdit->text();
    m_menuEntry = ui->menuEntry_radioButton->isChecked();

    if (m_name.isEmpty())
        m_name = "Tomporary_Script";
    if (m_author.isEmpty())
        m_author = "Unknown";
    if (m_description.isEmpty())
        m_description = "No Description Available";
    if (m_type.isEmpty())
        m_type = "common";

    // Save configurations file.
    QSettings settings(Constants::S_SCRIPTDIRECTORY + QLatin1String("/") + m_name + "." + Constants::C_EXTENSION, QSettings::IniFormat);
    settings.beginGroup("Basic");
    settings.setValue("Author", m_author);
    settings.setValue("Version", m_version);
    settings.setValue("Description", m_description);
    settings.setValue("Type", m_type);
    settings.setValue("MenuEntry", m_menuEntry);
    settings.setValue("Directory", Constants::S_SCRIPTDIRECTORY);
    settings.endGroup();

    // Save source code.
    QString scriptFullPath = Constants::S_SCRIPTDIRECTORY + QLatin1String("/") + m_name + "." + Constants::S_EXTENSION;
    QFile file(scriptFullPath);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Image Processing Testbed"),
                             tr("Cannot write file %1:/n%2.")
                             .arg(scriptFullPath)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);

    out << m_sourceCode;

    file.flush();
    file.close();

    this->hide();
}

void ScriptSaver::setSourceCode(QString code)
{
    m_sourceCode = code;
}
