#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "doubleviewport.h"
#include "enums.h"
#include "historycommands.h"
#include "settingsdialog.h"
#include "../ScriptEditor/scripteditor.h"
#include "scriptrunner.h"
#include "scriptmanager.h"
#include "scriptsaver.h"

#include <QMainWindow>
#include <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QAbstractItemModel;
class QComboBox;
class QCompleter;
class QLabel;
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE
class ScriptEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    enum { MaxRecentFiles = 5 };

public slots:
    void load();
    void openRecentFile();

    void save();
    void saveAs();
    void saveScript();

    void print();
    void printAs();

    void undo();
    void redo();
    void goEnd();
    void goStart();

    void zoomIn();
    void zoomOut();
    void zoomFit();

    void pixelPointer();
    void differencePointer();
    void textPointer();
    void noInteractive();

    void about();
    void goFullscreen(bool state);
    void keepImage();
    void historyItemClicked(QListWidgetItem*);

    void algorithm_negativeEffect();
    void algorithm_contrastEffect();
    void algorithm_redEffect();
    void algorithm_grayEffect();
    void algorithm_closingEffect();
    void algorithm_erosionEffect();
    void algorithm_sharpenEffect();
    void algorithm_saturationEffect();
    void algorithm_coolEffect();
    void algorithm_warmEffect();
    void algorithm_edgesEffect();
    void algorithm_sepiaEffect();
    void algorithm_pixelateEffect();
    void algorithm_blurEffect();

    bool saveData(const QString &fileName, FileFormats format);
    bool saveDataInDefaultFormat(const QString &fileName);
    bool saveDataInBINARYFormat(const QString &fileName);    // todo
    bool saveDataInI2DCFormat(const QString &fileName);    // todo
    bool saveDataInL2DCFormat(const QString &fileName);    // todo

    bool loadData(const QString &fileName, FileFormats format);
    bool loadDataInDefaultFormat(const QString &fileName);
    bool loadDataInBINARYFormat(const QString &fileName);    // todo
    bool loadDataInI2DCFormat(const QString &fileName);    // todo
    bool loadDataInL2DCFormat(const QString &fileName);    // todo

    void openSettingsDialog();

    //-Helper Functions--------------------------------------------------------------------------------------
    QList<int> getPixel(int x, int y);
    void setPixel(int x, int y, QList<int> pVal);
    int getWidth();
    int getHeight();
    void updateImage();

private slots:
    void updateSettings(int _windowSize, int _pointerDimX, int _pointerDimY, int _pointerWindowSize, int _maxFrames);
    void openScriptFile(QString *sourcecode);
    void directoryChanged(const QString &path, bool initialize = false);
    void runScript();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void initialState(bool state);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();

    QAbstractItemModel *modelFromFile(const QString& fileName);
    void createScriptMenus();
    void deleteScriptMenus();

    int m_windowSize;
    int m_pointerDimX;
    int m_pointerDimY;
    int m_pointerWindowSize;
    int m_maxFrames;

    QString strippedName(const QString &fullFileName);
    QString curFile;

    FileFormats fileFormat(const QString &fileName);

    DoubleViewport *doubleViewport;
    ScriptEditor *scriptEditor;
    QListWidget *HistoryList;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *algorithmsMenu;
    QMenu *scriptsMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *toolsToolBar;
    QToolBar *algorithmsToolBar;

    QAction *loadImageAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *saveScriptAct;
    QAction *printAct;
    QAction *printAsAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *goEndAct;
    QAction *goStartAct;
    QAction *rotateRightAct;
    QAction *rotateLeftAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomFitAct;
    QAction *pixelPointerAct;
    QAction *diffPointerAct;
    QAction *textPointerAct;
    QAction *noInteractivePointerAct;
    QAction *aboutAct;
    QAction *quitAct;
    QAction *fullscreenAct;
    QAction *keepImageAct;
    QAction *alg_negativeEffectAct;
    QAction *alg_contrastEffectAct;
    QAction *alg_redEffectAct;
    QAction *alg_grayEffectAct;
    QAction *alg_closingEffectAct;
    QAction *alg_erosionEffectAct;
    QAction *alg_sharpenEffectAct;
    QAction *alg_saturationEffectAct;
    QAction *alg_coolEffectAct;
    QAction *alg_warmEffectAct;
    QAction *alg_edgesEffectAct;
    QAction *alg_sepiaEffectAct;
    QAction *alg_pixelateEffectAct;
    QAction *alg_blurEffectAct;
    QAction *recentFileActs[MaxRecentFiles];
    QAction *separatorAct;
    QAction *settingsAct;
    QAction *runScriptAct;

    QString fileFilters;
    QString m_fileName1;
    QString m_fileName2;
    QString m_fileNameSc;

    QImage * m_image1;                          // The original image.
    QImage * m_image2;                          // Image holding user edits.

    HistoryCommands* historyCommands;
    SettingsDialog* settingsDialog;
    ScriptSaver* scriptSaver;

    QCompleter *completer;
    QFileSystemWatcher *watcher;
    QString m_defaultDirectory;
    ScriptManager *m_manager;
};

#endif
