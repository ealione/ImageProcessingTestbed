#ifndef VARIABLESETTINGS_H
#define VARIABLESETTINGS_H

#include "enums.h"

#include <QColor>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QKeySequence>
#include <QFont>

/*
 * Singleton for variables needed for the program.
 *
 */
class VariableSettings
{
public:
    /*
     * Instance of singleton (static)
     *
     * VariableSettings Pointer of singleton
     */
    static VariableSettings* Instance();

    inline Algorithms getAlgorithm() { return mCurrentAlgorithm; }
    inline void setAlgorithm(const Algorithms &algorithm) { mCurrentAlgorithm = algorithm; }

    inline Algorithms getPreviousAlgorithm() { return mPreviousAlgorithm; }
    inline void setPreviousAlgorithm(const Algorithms &algorithm) { mPreviousAlgorithm = algorithm; }

    inline QSize getBaseSize() { return mBaseSize; }
    inline void setBaseSize(const QSize &baseSize) { mBaseSize = baseSize; }

    inline bool getIsAutoSave() { return mIsAutoSave; }
    inline void setIsAutoSave(const bool &isAutoSave) { mIsAutoSave = isAutoSave; }

    inline int getAutoSaveInterval() { return mAutoSaveInterval; }
    inline void setAutoSaveInterval(const int &interval) { mAutoSaveInterval = interval; }

    inline int getHistoryDepth() { return mHistoryDepth; }
    inline void setHistoryDepth(const int &historyDepth) { mHistoryDepth = historyDepth; }

    inline QString getAppLanguage() { return mAppLanguage; }
    inline void setAppLanguage(const QString &appLanguage) { mAppLanguage = appLanguage; }

    inline QString getLastFilePath() { return mLastFilePath; }
    inline void setLastFilePath(const QString &lastFilePath) { mLastFilePath = lastFilePath; }

    inline QSize getWindowSize() { return mWindowSize; }
    inline void setWindowSize(const QSize &winSize) { mWindowSize = winSize; }

    inline QFont getTextFont() { return mTextFont; }
    inline void setTextFont(const QFont textFont) { mTextFont = textFont; }

    inline int getWinSize() { return mWinSize; }
    inline void setWinSize(const int winSize) { mWinSize = winSize; }

    inline int getPointSizeX() { return mPointSizeX; }
    inline void setPointSizeX(const int &pointSizeX) { mPointSizeX = pointSizeX; }

    inline int getPointSizeY() { return mPointSizeY; }
    inline void setPointSizeY(const int &pointSizeY) { mPointSizeY = pointSizeY; }

    inline int getPointTextSize() { return mPointTextSize; }
    inline void setPointTextSize(const int &pointTextSize) { mPointTextSize = pointTextSize; }

    inline QByteArray getMainDockState() { return mMainDockState; }
    inline void setMainDockState(const QByteArray &mainDockState) { mMainDockState = mainDockState; }

    inline QByteArray getWorkspaceDockState() { return mWorkspaceDockState; }
    inline void setWorkspaceDockState(const QByteArray &workspaceDockState) { mWorkspaceDockState = workspaceDockState; }

    inline QMap<QString, QKeySequence> getFileShortcuts() { return mFileShortcuts; }
    inline QKeySequence getFileShortcutByKey(const QString &key) { return mFileShortcuts[key]; }

    inline void setFileShortcutByKey(const QString &key, const QKeySequence &value) { mFileShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getEditShortcuts() { return mEditShortcuts; }

    inline QKeySequence getEditShortcutByKey(const QString &key) { return mEditShortcuts[key]; }
    inline void setEditShortcutByKey(const QString &key, const QKeySequence &value) { mEditShortcuts[key] = value; }

    inline QMap<QString, QKeySequence> getAlgorithmsShortcuts() { return mAlgorithmsShortcuts; }
    inline QKeySequence getAlgorithmShortcutByKey(const QString &key) { return mAlgorithmsShortcuts[key]; }

    inline void setAlgorithmShortcutByKey(const QString &key, const QKeySequence &value) { mAlgorithmsShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getToolsShortcuts() { return mToolsShortcuts; }

    inline QKeySequence getToolShortcutByKey(const QString &key) { return mToolsShortcuts[key]; }
    inline void setToolShortcutByKey(const QString &key, const QKeySequence &value) { mToolsShortcuts[key] = value; }

    void readSetting();
    void writeSettings();
    void readState();
    void writeState();

private:
    VariableSettings();
    VariableSettings(VariableSettings const&){}

    static VariableSettings* m_pInstance;

    Algorithms mCurrentAlgorithm, mPreviousAlgorithm;
    QSize mBaseSize, mWindowSize;
    bool mIsAutoSave, mIsRestoreWindowSize, mIsAskCanvasSize, mIsInitialized;
    int mAutoSaveInterval, mHistoryDepth, mWinSize, mPointSizeX, mPointSizeY, mPointTextSize;
    QString mAppLanguage;
    QString mLastFilePath; /* last opened file */
    QFont mTextFont;
    QMap<QString, QKeySequence> mFileShortcuts, mEditShortcuts, mAlgorithmsShortcuts, mToolsShortcuts;
    QByteArray mMainDockState, mWorkspaceDockState;
};

#endif // VARIABLESETTINGS_H
