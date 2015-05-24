#include "variablesettings.h"

#include <QtCore/QSettings>

VariableSettings* VariableSettings::m_pInstance = 0;

VariableSettings::VariableSettings()
{
    mTextFont = QFont("Times", 12);
    mCurrentAlgorithm = NO_ALGORITHM;
    mPreviousAlgorithm = NO_ALGORITHM;
    mIsInitialized = false;
    readSetting();
    readState();
}

VariableSettings* VariableSettings::Instance()
{
    if(!m_pInstance)
        m_pInstance = new VariableSettings;

    return m_pInstance;
}

void VariableSettings::readSetting()
{
    QSettings settings;
    mBaseSize = settings.value("/Settings/BaseSize", QSize(400, 300)).toSize();
    mIsAutoSave = settings.value("/Settings/IsAutoSave", false).toBool();
    mAutoSaveInterval = settings.value("/Settings/AutoSaveInterval", 300).toInt();
    mHistoryDepth = settings.value("/Settings/HistoryDepth", 40).toInt();
    mAppLanguage = settings.value("/Settings/AppLanguage", "system").toString();
    mIsRestoreWindowSize = settings.value("/Settings/IsRestoreWindowSize", true).toBool();

    //read shortcuts for file menu
    mFileShortcuts.insert("Open", settings.value("/Shortcuts/File/Open", QKeySequence(QKeySequence::Open)).value<QKeySequence>());
    mFileShortcuts.insert("Save", settings.value("/Shortcuts/File/Save", QKeySequence(QKeySequence::Save)).value<QKeySequence>());
    mFileShortcuts.insert("SaveAs", settings.value("/Shortcuts/File/SaveAs", QKeySequence(QKeySequence::SaveAs)).value<QKeySequence>());
    //mFileShortcuts.insert("Close", settings.value("/Shortcuts/File/Close", QKeySequence(QKeySequence::Close)).value<QKeySequence>());
    mFileShortcuts.insert("Print", settings.value("/Shortcuts/File/Print", QKeySequence(QKeySequence::Print)).value<QKeySequence>());
    mFileShortcuts.insert("PrintAs", settings.value("/Shortcuts/File/PrintAs", "Ctrl+Alt+P").value<QKeySequence>());
    mFileShortcuts.insert("Exit", settings.value("/Shortcuts/File/Exit", QKeySequence(QKeySequence::Quit)).value<QKeySequence>());

    //read shortcuts for edit menu
    mEditShortcuts.insert("Undo", settings.value("/Shortcuts/Edit/Undo", QKeySequence(QKeySequence::Undo)).value<QKeySequence>());
    mEditShortcuts.insert("Redo", settings.value("/Shortcuts/Edit/Redo", QKeySequence(QKeySequence::Redo)).value<QKeySequence>());
    mEditShortcuts.insert("GoStart", settings.value("/Shortcuts/Edit/GoStart", QKeySequence(QKeySequence::Back)).value<QKeySequence>());
    mEditShortcuts.insert("GoEnd", settings.value("/Shortcuts/Edit/GoEnd", QKeySequence(QKeySequence::Forward)).value<QKeySequence>());

    //read shortcuts for Algorithms menu
    /*mAlgorithmsShortcuts.insert("Cursor", settings.value("/Shortcuts/Algorithms/Cursor", "Ctrl+1").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Lastic", settings.value("/Shortcuts/Algorithms/Lastic", "Ctrl+2").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Pipette", settings.value("/Shortcuts/Algorithms/Pipette", "Ctrl+3").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Loupe", settings.value("/Shortcuts/Algorithms/Loupe", "Ctrl+4").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Pen", settings.value("/Shortcuts/Algorithms/Pen", "Ctrl+5").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Line", settings.value("/Shortcuts/Algorithms/Line", "Ctrl+6").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Spray", settings.value("/Shortcuts/Algorithms/Spray", "Ctrl+7").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Fill", settings.value("/Shortcuts/Algorithms/Fill", "Ctrl+8").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Rect", settings.value("/Shortcuts/Algorithms/Rect", "Ctrl+9").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Ellipse", settings.value("/Shortcuts/Algorithms/Ellipse", "Ctrl+0").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Curve", settings.value("/Shortcuts/Algorithms/Curve", "").value<QKeySequence>());
    mAlgorithmsShortcuts.insert("Text", settings.value("/Shortcuts/Algorithms/Text", "").value<QKeySequence>());*/
    // TODO: Add shortcuts for new Algorithms here

    //read shortcuts for tools menu
    mToolsShortcuts.insert("SimplePointer", settings.value("/Shortcuts/Tools/Pointer/SimplePointer", "Ctrl+7").value<QKeySequence>());
    mToolsShortcuts.insert("TextPointer", settings.value("/Shortcuts/Tools/Pointer/TextPointer", "Ctrl+8").value<QKeySequence>());
    mToolsShortcuts.insert("DiffPointer", settings.value("/Shortcuts/Tools/Pointer/DiffPointer", "Ctrl+9").value<QKeySequence>());
    mToolsShortcuts.insert("PixelPointer", settings.value("/Shortcuts/Tools/Pointer/PixelPointer", "Ctrl+0").value<QKeySequence>());

    mToolsShortcuts.insert("ZoomIn", settings.value("/Shortcuts/Tools/Zoom/ZoomIn", QKeySequence(QKeySequence::ZoomIn)).value<QKeySequence>());
    mToolsShortcuts.insert("ZoomOut", settings.value("/Shortcuts/Tools/Zoom/ZoomOut", QKeySequence(QKeySequence::ZoomOut)).value<QKeySequence>());
    mToolsShortcuts.insert("ZoomFit", settings.value("/Shortcuts/Tools/Zoom/ZoomFit", "Ctrl+*").value<QKeySequence>());

    mToolsShortcuts.insert("Switch", settings.value("/Shortcuts/Tools/Image/Switch", QKeySequence(Qt::CTRL + Qt::Key_Backspace)).value<QKeySequence>());
}

void VariableSettings::writeSettings()
{
    QSettings settings;
    settings.setValue("/Settings/BaseSize", mBaseSize);
    settings.setValue("/Settings/IsAutoSave", mIsAutoSave);
    settings.setValue("/Settings/AutoSaveInterval", mAutoSaveInterval);
    settings.setValue("/Settings/HistoryDepth", mHistoryDepth);
    settings.setValue("/Settings/AppLanguage", mAppLanguage);
    settings.setValue("/Settings/IsRestoreWindowSize", mIsRestoreWindowSize);

    //write shortcuts for file menu
    settings.setValue("/Shortcuts/File/Open", mFileShortcuts["Open"]);
    settings.setValue("/Shortcuts/File/Save", mFileShortcuts["Save"]);
    settings.setValue("/Shortcuts/File/SaveAs", mFileShortcuts["SaveAs"]);
    //settings.setValue("/Shortcuts/File/Close", mFileShortcuts["Close"]);
    settings.setValue("/Shortcuts/File/Print", mFileShortcuts["Print"]);
    settings.setValue("/Shortcuts/File/Print", mFileShortcuts["PrintAs"]);
    settings.setValue("/Shortcuts/File/Exit", mFileShortcuts["Exit"]);

    //write shortcuts for edit menu
    settings.setValue("/Shortcuts/Edit/Undo", mEditShortcuts["Undo"]);
    settings.setValue("/Shortcuts/Edit/Redo", mEditShortcuts["Redo"]);
    settings.setValue("/Shortcuts/Edit/GoStart", mEditShortcuts["GoStart"]);
    settings.setValue("/Shortcuts/Edit/GoEnd", mEditShortcuts["GoStart"]);

    //write shortcuts for Algorithms menu
    /*settings.setValue("/Shortcuts/Algorithms/Cursor", mAlgorithmsShortcuts["Cursor"]);
    settings.setValue("/Shortcuts/Algorithms/Lastic", mAlgorithmsShortcuts["Lastic"]);
    settings.setValue("/Shortcuts/Algorithms/Pipette", mAlgorithmsShortcuts["Pipette"]);
    settings.setValue("/Shortcuts/Algorithms/Loupe", mAlgorithmsShortcuts["Loupe"]);
    settings.setValue("/Shortcuts/Algorithms/Pen", mAlgorithmsShortcuts["Pen"]);
    settings.setValue("/Shortcuts/Algorithms/Line", mAlgorithmsShortcuts["Line"]);
    settings.setValue("/Shortcuts/Algorithms/Spray", mAlgorithmsShortcuts["Spray"]);
    settings.setValue("/Shortcuts/Algorithms/Fill", mAlgorithmsShortcuts["Fill"]);
    settings.setValue("/Shortcuts/Algorithms/Rect", mAlgorithmsShortcuts["Rect"]);
    settings.setValue("/Shortcuts/Algorithms/Ellipse", mAlgorithmsShortcuts["Ellipse"]);*/

    //write shortcuts for tools menu
    settings.setValue("/Shortcuts/Tools/Zoom/ZoomIn", mToolsShortcuts["ZoomIn"]);
    settings.setValue("/Shortcuts/Tools/Zoom/ZoomOut", mToolsShortcuts["ZoomOut"]);
    settings.setValue("/Shortcuts/Tools/Zoom/ZoomFit", mToolsShortcuts["ZoomFit"]);

    settings.setValue("/Shortcuts/Tools/Pointer/SimplePointer", mToolsShortcuts["SimplePointer"]);
    settings.setValue("/Shortcuts/Tools/Pointer/TextPointer", mToolsShortcuts["TextPointer"]);
    settings.setValue("/Shortcuts/Tools/Pointer/DiffPointer", mToolsShortcuts["DiffPointer"]);
    settings.setValue("/Shortcuts/Tools/Pointer/PixelPointer", mToolsShortcuts["PixelPointer"]);

    settings.setValue("/Shortcuts/Tools/Image/Switch", mToolsShortcuts["Switch"]);
}

void VariableSettings::readState()
{
    QSettings settings;
    mWindowSize = settings.value("/State/WindowSize", QSize()).toSize();
}

void VariableSettings::writeState()
{
    QSettings settings;
    if (mWindowSize.isValid())
    {
        settings.setValue("/State/WindowSize", mWindowSize);
    }
}
