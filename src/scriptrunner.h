#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QScriptEngine>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QScriptValueIterator>
#include <QFileDialog>
#include <QMainWindow>

struct ErrorMessage
{
    ErrorMessage() : hasError(false) {}
    ErrorMessage(const QString& fileName, int line, const QString& message) :
        hasError(true), fileName(fileName), line(line), message(message) {}

    bool hasError;
    QString fileName;
    int line;
    QString message;
};

class ScriptRunner : public QObject
{
    Q_OBJECT

public:
    explicit ScriptRunner(QObject *parent = 0);

    typedef QSharedPointer<QScriptEngine> QScriptEnginePtr;
    static ScriptRunner* instance();
    virtual ~ScriptRunner();

    ErrorMessage runScript(const QString sourceCode);
    QScriptEnginePtr scriptEngine() { return ensureEngineInitialized(); }

    static QString absolutePath(const QString& path);
    static QString currentScriptRoot();

private:
    QScriptEnginePtr ensureEngineInitialized();
    QScriptEnginePtr m_engine; // Dont get confused  (&*m_engine) gives you a pointer to this engine
    static ScriptRunner* m_instance;
    QWidget* widget;

public slots:
    void newEngineObject(QObject *object, const QString &name);

};

#endif // SCRIPTRUNNER_H
