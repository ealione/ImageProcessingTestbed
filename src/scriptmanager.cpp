#include "scriptmanager.h"

#include <QMessageBox>

using namespace Scripting;

ScriptManager::ScriptManager(QObject *parent) :
    QObject(parent)
{    
    // Instantiate a script runner
    m_runner = new ScriptRunner(this);
    m_runner->newEngineObject(parent, "app");
}

/*
 * Check if a script is valid according to its' configuration file.
 */
QString ScriptManager::checkScriptValid(ScriptManager::Script* s)
{
    QString sc_ERROR;
    sc_ERROR = "OK";

    // Check if the script this configuration file is pointing to exists.
    QString path = s->directory + QDir::separator() + s->fileName;
    QFileInfo checkFile(path);
    if (!checkFile.exists() && !checkFile.isFile())
    {
        sc_ERROR = QString(tr("Script %1 was not found at %2/")).arg(s->name, s->directory);
        return sc_ERROR;
    }

    // These variables belonging to the RUNCONFIG section must have a valid value.
    /*QString thisExecution = s->execution.toLower();
    if (!(thisExecution == "onstart") && !(thisExecution == "oncall"))
    {
        sc_ERROR = QString(tr("%1 is not a valid value for \"Execution\" at %2. Possible values are \
                           onStart|onCall")).arg(s->execution).arg(s->name);
        return sc_ERROR;
    }*/

    return sc_ERROR;
}

/*
 * This function will make sure that the default scripts' directory
 * exists and it will return its' path.
 */
QString ScriptManager::defaultDirectory() const
{
    const QString &path =
            QCoreApplication::applicationDirPath()+ QLatin1Char('/') +
            Constants::S_SCRIPTDIRECTORY;
    if (QFile::exists(path) || QDir().mkpath(path))
        return path;
    qWarning() << tr("Could not create scripts directory...") << path;
    return QString();
}


void ScriptManager::fillScriptInfo(ScriptManager::Script* s, QSettings* d)
{
    d->beginGroup("Basic");
    s->directory    = d->value("Directory", "").toString();
    s->author       = d->value("Author", "UNKNOWN").toString();
    s->version      = d->value("Version", "000").toString();
    s->description  = d->value("Description", "UNKNOWN").toString();
    s->type         = d->value("Type", "UNKNOWN").toString();
    s->license      = d->value("License", "UNKNOWN").toString();
    s->menuentry    = d->value("MenuEntry", false).toBool();
    d->endGroup();
}

void ScriptManager::runScriptFile(QString sourceCode)
{
    ErrorMessage message = m_runner->runScript(sourceCode);
    if (message.hasError)
    {
        QMessageBox::warning(NULL, tr("Image Processing Testbed"),
                             QString(tr("Error in %1 at line %3: %4\n")
                                     .arg(message.fileName)
                                     .arg(message.line).arg(message.message)),
                             QMessageBox::Ok);
    }
}

/*
 * Will list all valid scripts that are found on the default directory.
 */
ScriptManager::ScriptList ScriptManager::listScripts() const
{
    return m_scriptList;
}

/*
 * Will list all scripts that are found on the default directory.
 */
QStringList ScriptManager::listScriptsinDir(QDir scriptPath) const
{
    scriptPath.setNameFilters(QStringList() << QLatin1String("*.") + "sconf");
    QStringList files = scriptPath.entryList(QDir::Files);
    return files;
}

void ScriptManager::returnScriptActions(QMenu &scriptsMenu)
{
    ScriptManager::ScriptList s_list = listScripts();

    //qDebug() << tr("Adding list submenus");
    QMutableListIterator<ScriptManager::Script> i(s_list);
    while (i.hasNext())
    {
        ScriptManager::Script script = i.next();
        if(script.menuentry)
        {
            //qDebug() << tr("Adding ") + script.name;

            QAction *s_action=new QAction(this);
            s_action->setText(script.name);
            scriptsMenu.addAction(s_action);

            connect(s_action, &QAction::triggered, [=]()
                {scriptSelected( script.name );});
        }
    }
}

void ScriptManager::removeScriptActions(QMenu &scriptsMenu)
{
    //qDebug() << tr("Removing list submenus");
    QList<QAction*> actions = scriptsMenu.actions();

    foreach (QAction* a, actions)
    {
        if(a->text() != "&Run Script" && a->text() != "")
        {
            //qDebug() << tr("Removing ") + a->text();
            scriptsMenu.removeAction(a);
        }
    }
}

void ScriptManager::scriptSelected(const QString &scriptName)
{
    QFile file(Constants::S_SCRIPTDIRECTORY + QLatin1String("/") + scriptName + "." + Constants::S_EXTENSION);
    QString* sourceCode;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        *sourceCode = stream.readAll();
        qDebug() <<stream.readAll();
    }

    emit scriptLoad(sourceCode);

    //qDebug() << tr("clicked ") + scriptName;
}
