/****************************************************************************
**
** Name:            ScriptRunner
** Version Major:   0
** Version Minor:   1
** Author:          Esmerald Aliaj <e.aliaj@growideas.eu>.
** History:         04-01-15    File Created.
** Description:     Loads and runs a script file while providing access
**                  to the script engine for all classes using it.
**
****************************************************************************/
#include "scriptrunner.h"
#include "scriptingconstants.h"

using namespace Scripting;

namespace {
    QString rootDir;
}

//-----------------------------------------------------------------------------

/*
 * Create an instance of a script runner.
 * This function is automatically called by the script manager at startup.
 */
ScriptRunner* ScriptRunner::m_instance = 0;

ScriptRunner::ScriptRunner(QObject *parent) :
    QObject(parent),
    m_engine(0)
{
    m_instance = this;
}

//-----------------------------------------------------------------------------

/*
 * Returns the instance of the current ScriptRunner.
 */
ScriptRunner *ScriptRunner::instance()
{
    return m_instance;
}

//-----------------------------------------------------------------------------

/*
 * Deletes this instance of the ScriptRunner.
 */
ScriptRunner::~ScriptRunner()
{
}

//-----------------------------------------------------------------------------

static QString errorFileName;

/*
 * Evaluates the contents of a script file and returns the result.
 */
static QScriptValue run(QScriptEngine* engine, const QString& sourceCode, bool recursive)
{    
    if ( recursive )
    {
        QScriptContext *context = engine->currentContext();
        QScriptContext *parent=context->parentContext();
        context->setActivationObject(parent->activationObject());
        context->setThisObject(parent->thisObject());
    }

    QScriptValue result = engine->evaluate(sourceCode);
    if (engine->hasUncaughtException() && errorFileName.isNull() )
        errorFileName = QString("Current Script");

    return result;
}

//-----------------------------------------------------------------------------

/*
 * Function that gets calld by the scriptManager each time a new script is to
 * be executed. It will take care of loading the scripts' ui if it has one
 * as well as making it available to the script engine.
 */
ErrorMessage ScriptRunner::runScript(const QString sourceCode)
{
    errorFileName = QString();

    // Make sure we do have an active script engine before executing the script.
    ensureEngineInitialized();

    QScriptValue result = run(m_engine.data(), sourceCode, false);

    qDebug() << result.toInteger();

    const bool failed = m_engine->hasUncaughtException();

    m_engine->collectGarbage();

    if (failed)
        return ErrorMessage(errorFileName, m_engine->uncaughtExceptionLineNumber(), result.toString());

    return ErrorMessage();
}

//-----------------------------------------------------------------------------

/*
 * Takes as input a directory, and if it is relative,
 * will returns the absolute path to that directory.
 */
QString ScriptRunner::absolutePath(const QString &path)
{
    if (QFileInfo(path).isRelative() )
        return rootDir + QLatin1String("/") + path;
    else
        return path;
}

//-----------------------------------------------------------------------------

/*
 * The root directory where all script files are expected.
 */
QString ScriptRunner::currentScriptRoot()
{
    return rootDir;
}

//-----------------------------------------------------------------------------

/*
 * Makes sure that the script engine for this instance of scriptRunner
 * is initialised and returns a pointer to it.
 */
ScriptRunner::QScriptEnginePtr ScriptRunner::ensureEngineInitialized()
{
    if (!m_engine.isNull())
        return m_engine;

    m_engine = QScriptEnginePtr(new QScriptEngine);

    return m_engine;
}

//-----------------------------------------------------------------------------

/*
 * Adds a new object to the script engine under a specified name,
 * later you can use all the objects methods located under its'
 * public slots through the console or a script.
 */
void ScriptRunner::newEngineObject(QObject *object, const QString &name)
{
    ensureEngineInitialized();

    // expose this widget to the script
    QScriptValue appContext = m_engine->newQObject(object);
    m_engine->globalObject().setProperty(name, appContext);
}

//-----------------------------------------------------------------------------
