#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "scriptingconstants.h"
#include "scriptrunner.h"

#include <QObject>
#include <QSettings>
#include <QMenu>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QDebug>

/*#include <QAction>
#include <QFileSystemWatcher>
#include <QSignalMapper>
#include <QShortcut>
#include <QMetaEnum>
#include <QList>*/

class ScriptManager : public QObject
{
    Q_OBJECT

public:
    ScriptManager(QObject *parent = 0);

    struct Script {
        QString name;           // This will be the name of the script as it will appear in the menu.
                                // Its' value is automatically set using the current script name.
                                // All other associated files (.script, .ui) should have adopt this name to be valid.
        QString fileName;       // This will be the full name of the script including its' extension.
                                // Its' value is automatically set using the current script name.
        QString directory;      // Relative path pointing to the scripts' source code and ui.
                                // Leave empty if they all are inside the '/scripts' directory
        QString author;         // Author of the script, only to be used internally for script maintenance and update. [Not Used]
        QString version;        // Version is a number spaning from 000 to 999.
        QString description;    // Brief text describing the script, could be used for script identification through a main menu.  [Not Used]
        QString type;           // Identifies the intended use for this script e.g. common, database, network.  [Not Used]
        QString position;       // A unique value to identify which function inside the main app the script is intendet for. [Not Used]
        bool gui;               // 0 if the script does not include a UI, any other value if it does.
        bool menuentry;         // 0 if the script should not have a menu entry, any other value if it should.
        QString execution;      // Should be either onStart for immediate execution at startup,
                                // or onCall to connect an action that calls the script from the menu.
        QString lang;           // Should be either script or python
        QString license;        // An identfier pointing at the license code. [Not Used]
        QString dependencies;   // Use this list to check whether all needed script dependencies are present. [Not Used]
    };

    typedef QList<Script> ScriptList;

    QString checkScriptValid(Script *s);

    void fillScriptInfo(Script *s, QSettings *d);

    /*
     * Will return the directory that the scripts are expected to reside.
     */
    QString defaultDirectory() const;

    ScriptList m_scriptList;

    void returnScriptActions(QMenu &scriptsMenu);
    void removeScriptActions(QMenu &scriptsMenu);

private:
    ScriptRunner *m_runner;

public slots:
    void runScriptFile(QString sourceCode);
    ScriptList listScripts() const;
    QStringList listScriptsinDir(QDir scriptPath) const;
    void scriptSelected(const QString &scriptName);

signals:
    void scriptLoad(QString*);
};

#endif // SCRIPTMANAGER_H
