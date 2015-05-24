#include <QtWidgets>
#ifndef QT_NO_PRINTDIALOG
#include <QtPrintSupport>
#endif

#include "mainwindow.h"
#include "alg_defaulteffects.h"

//-Main Window Constructor-------------------------------------------------------------------------------
MainWindow::MainWindow()
{
    // Add a script manager
    m_manager = new ScriptManager(this);
    connect(m_manager, SIGNAL(scriptLoad(QString*)), this, SLOT(openScriptFile(QString*)));

    // Initialise double viewport
    doubleViewport = new DoubleViewport();
    doubleViewport->setParent(this);
    doubleViewport->loadImage(":/images/transparent_background.png");
    doubleViewport->setWindowSize(5);

    // Use double viewport as our central widget.
    setCentralWidget(doubleViewport);

    // Initialise all menus, toolbars and dockable widgets.
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    initialState(false);

    // This is the title of the main window.
    setWindowTitle(tr("Image Processing Testbed v0.0.1"));

    setUnifiedTitleAndToolBarOnMac(true);

    // Filters suggesting save options.
    fileFilters = tr("JPG, JPEG (*.jpg *.jpeg)\n"
                     "J2K, JPG2000 (*.j2k)\n"
                     "Portable Network Graphics (PNG) (*.png)\n"
                     "Bitmap Image File (BMP) (*.png)\n"
                     "Targa bitmap (TGA) (*.tga)\n"
                     "Tagged Image File Format (TIFF) (*.tiff)\n"
                     "BINARY (*.bin)\n"
                     "Index 2D Data Compression (I2DC) (*.i2dc)\n"
                     "Line 2D Data Compression (L2DC) (*.l2dc)\n"
                     "ALL (*.*)");

    // Start with a simple pointer.
    doubleViewport->setUseInteractivePointer(false);

    // Initialise images.
    m_image1 = new QImage(8, 8, QImage::Format_ARGB32);
    m_image2 = new QImage(8, 8, QImage::Format_ARGB32);

    // Detect any previous saved frames and remove them or create a frames folder if does not exist.
    QString path = QCoreApplication::applicationDirPath() + "/frames";
    QDir dir(path);
    if (!dir.exists())
    {
        QDir().mkdir(QCoreApplication::applicationDirPath() + "/frames");
    }
    else
    {
        dir.setNameFilters(QStringList() << "*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }

    // Initialise undo/redo framework.
    historyCommands = new HistoryCommands();

    setWindowFilePath(QString());

    // Get previously saved state.
    QSettings settings(QCoreApplication::applicationDirPath() + "/state", "ImageProcessingTestbed");
    settings.beginGroup("MainWindow");
    this->restoreState(settings.value("dockState").toByteArray());
    setGeometry(settings.value("position").toRect());
    doubleViewport->restoreState(settings.value("workspaceState").toByteArray());
    m_windowSize = settings.value("detailWinSize", 3).toInt();
    m_pointerDimX = settings.value("pointerSizeX", 40).toInt();
    m_pointerDimY = settings.value("pointerSizeY", 40).toInt();
    m_pointerWindowSize = settings.value("pointerWinSize", 3).toInt();
    m_maxFrames = settings.value("maxFrames", 10).toInt();
    doubleViewport->setWindowSize(m_windowSize);
    doubleViewport->setPointerSize(m_pointerDimX, m_pointerDimY, m_pointerWindowSize);
    historyCommands->setMaxItems(m_maxFrames);
    settings.endGroup();

    // Initialise settings dialog.
    settingsDialog = new SettingsDialog(this);
    connect(settingsDialog, SIGNAL(settingsUpdate(int, int, int, int, int)),
            this, SLOT(updateSettings(int, int, int, int, int)));

    // Track scripts directory changes
    watcher = new QFileSystemWatcher(this);
    // Refresh menu in case of directory change.
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
    // Reload script in case of edit
    //connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

    // Add scripts directory to the file whatcher.
    m_defaultDirectory = m_manager->defaultDirectory();
    if (!m_defaultDirectory.isEmpty())
    {
        watcher->addPath(m_defaultDirectory);
        directoryChanged(m_defaultDirectory, true);
    }

    // Initialise script saver.
    scriptSaver = new ScriptSaver(this);
}

MainWindow::~MainWindow()
{
    // Save program state.
    QSettings settings(QCoreApplication::applicationDirPath() + "/state", "ImageProcessingTestbed");
    settings.beginGroup("MainWindow");
    settings.setValue("dockState", this->saveState());
    settings.setValue("position", this->geometry());
    settings.setValue("workspaceState", doubleViewport->saveState());
    settings.setValue("pointerSizeX", m_pointerDimX);
    settings.setValue("pointerSizeY", m_pointerDimY);
    settings.setValue("pointerWinSize", m_pointerWindowSize);
    settings.setValue("detailWinSize", m_windowSize);
    settings.setValue("maxFrames", m_maxFrames);
    settings.endGroup();
}

//-Load Image Functions----------------------------------------------------------------------------------
void MainWindow::load()
{
    m_fileName1 = QFileDialog::getOpenFileName(
                       this,
                       tr("Open File"),
                       "",
                       fileFilters);

    if (m_fileName1.isEmpty())
        return;

    loadData(m_fileName1, fileFormat(m_fileName1));

    statusBar()->showMessage(tr("Loaded '%1'").arg(m_fileName1), 2000);

    initialState(true);    

    doubleViewport->updateFilenames(m_fileName1, m_fileName2);

    historyCommands->alg_newCommand("Load ", m_fileName1, m_image1, m_image2, *HistoryList);

    setCurrentFile(m_fileName1);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        loadData(action->data().toString(), fileFormat(action->data().toString()));

        statusBar()->showMessage(tr("Loaded '%1'").arg(action->data().toString()), 2000);

        initialState(true);

        doubleViewport->updateFilenames(action->data().toString(), m_fileName2);

        historyCommands->alg_newCommand("Load ", action->data().toString(), m_image1, m_image2, *HistoryList);

        setCurrentFile(action->data().toString());
    }
}

//-Save Image Functions----------------------------------------------------------------------------------
void MainWindow::save()
{
    if (m_fileName2.isEmpty())
        saveAs();
    else
    {
        if (saveData(m_fileName2, fileFormat(m_fileName2)))
        {
            QMessageBox::warning(this, tr("Image Processing Testbed"),
                                 tr("Cannot write file %1: Algorithm Specific Error.")
                                 .arg(m_fileName2));
            return;
        }

        statusBar()->showMessage(tr("Saved '%1'").arg(m_fileName2), 2000);

        setCurrentFile(m_fileName2);
    }
}

void MainWindow::saveAs()
{
    m_fileName2 = QFileDialog::getSaveFileName(this,
                        tr("Choose a file name"), ".",
                        fileFilters);
    if (m_fileName2.isEmpty())
        return;
    QFile file(m_fileName2);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Image Processing Testbed"),
                             tr("Cannot write file %1:/n%2.")
                             .arg(m_fileName2)
                             .arg(file.errorString()));
        return;
    }

    if (saveData(m_fileName2, fileFormat(m_fileName2)))
    {
        QMessageBox::warning(this, tr("Image Processing Testbed"),
                             tr("Cannot write file %1: Algorithm Specific Error.")
                             .arg(m_fileName2));
         return;
     }

    statusBar()->showMessage(tr("Saved '%1'").arg(m_fileName2), 2000);

    doubleViewport->updateFilenames(m_fileName1, m_fileName2);
}

void MainWindow::saveScript()
{
    scriptSaver->setSourceCode(scriptEditor->toPlainText());

    scriptSaver->show();

    statusBar()->showMessage(tr("Saved Script '%1'").arg(m_fileNameSc), 2000);
}

//-Print Image Functions---------------------------------------------------------------------------------
void MainWindow::print()
{
    #ifndef QT_NO_PRINTDIALOG
        QPrinter printer;

        QPrintDialog dlg(&printer, this);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        QPainter painter(&printer);
        painter.drawImage(QPoint(0,0), *m_image2);
        painter.end();

        statusBar()->showMessage(tr("Ready"), 2000);
    #endif
}

void MainWindow::printAs()
{
    #ifndef QT_NO_PRINTDIALOG
        QPrinter printer;

        QPrintDialog dlg(&printer, this);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        QPixmap pxm = QWidget::grab(QRect( QPoint( 0, 0 ), QSize( -1, -1 ) ));

        QPainter painter(&printer);
        painter.drawPixmap(QPoint(0,0), pxm);
        painter.end();

        statusBar()->showMessage(tr("Ready"), 2000);
    #endif
}

//-Edit Image Functions----------------------------------------------------------------------------------
void MainWindow::goStart()
{
    QList<QImage> tempList;
    tempList = historyCommands->alg_goHere(0, *HistoryList);

    doubleViewport->loadImage(tempList.at(0), tempList.at(1));
}

void MainWindow::undo()
{
    int curInd = HistoryList->currentRow();
    if (curInd > 0)
    {
        QList<QImage> tempList;
        tempList = historyCommands->alg_goHere(curInd - 1, *HistoryList);

        doubleViewport->loadImage(tempList.at(0), tempList.at(1));
    }
}

void MainWindow::redo()
{
    int curInd = HistoryList->currentRow();
    if (curInd < HistoryList->count() - 1)
    {
        QList<QImage> tempList;
        tempList = historyCommands->alg_goHere(curInd + 1, *HistoryList);

        doubleViewport->loadImage(tempList.at(0), tempList.at(1));
    }
}

void MainWindow::goEnd()
{
    QList<QImage> tempList;
    tempList = historyCommands->alg_goHere(HistoryList->count() - 1, *HistoryList);

    doubleViewport->loadImage(tempList.at(0), tempList.at(1));
}

//-Zoom Functions----------------------------------------------------------------------------------------
void MainWindow::zoomIn()
{
    doubleViewport->zoomControl(1);
}

void MainWindow::zoomOut()
{
    doubleViewport->zoomControl(-1);
}

void MainWindow::zoomFit()
{
    doubleViewport->zoomControl(0);
}

//-Pointer Functions-------------------------------------------------------------------------------------
void MainWindow::pixelPointer()
{
    doubleViewport->setUseInteractivePointer(true);
    doubleViewport->setInteractivePointerMode(0);
}

void MainWindow::differencePointer()
{
    doubleViewport->setUseInteractivePointer(true);
    doubleViewport->setInteractivePointerMode(1);
}

void MainWindow::textPointer()
{
    doubleViewport->setUseInteractivePointer(true);
    doubleViewport->setInteractivePointerMode(2);
}

void MainWindow::noInteractive()
{
    doubleViewport->setUseInteractivePointer(false);
}

//-Switch Image Function---------------------------------------------------------------------------------
void MainWindow::keepImage()
{
    *m_image1 = *m_image2;
    doubleViewport->loadImage(*m_image1, *m_image2);

    m_fileName1 = m_fileName2;
    doubleViewport->updateFilenames(m_fileName1, m_fileName2);

    historyCommands->alg_newCommand("Swap ", m_fileName1, m_image1, m_image2, *HistoryList);
}

//-Algorithm Functions-----------------------------------------------------------------------------------
void MainWindow::algorithm_negativeEffect()
{
    Alg_DefaultEffects::applyNegative(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Negative ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_contrastEffect()
{
    Alg_DefaultEffects::applyContrast(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Contrast ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_redEffect()
{
    Alg_DefaultEffects::applyRed(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Red ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_grayEffect()
{
    Alg_DefaultEffects::applyGreyScale(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Gray ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_closingEffect()
{
    Alg_DefaultEffects::applyClosing(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Closing ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_erosionEffect()
{
    Alg_DefaultEffects::applyErosion(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Erosion ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_sharpenEffect()
{
    Alg_DefaultEffects::applySharpen(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Sharpen ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_saturationEffect()
{
    Alg_DefaultEffects::applySaturation(m_image1, m_image2, 30);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Saturation ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_coolEffect()
{
    Alg_DefaultEffects::applyCool(m_image1, m_image2, 30);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Cool ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_warmEffect()
{
    Alg_DefaultEffects::applyWarm(m_image1, m_image2, 30);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Warm ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_edgesEffect()
{
    Alg_DefaultEffects::applyEdges(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Edges ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_sepiaEffect()
{
    Alg_DefaultEffects::applySepia(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Sepia ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_pixelateEffect()
{
    Alg_DefaultEffects::applyPixelate(m_image1, m_image2, 30);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Pixelate ", m_fileName1, m_image1, m_image2, *HistoryList);
}

void MainWindow::algorithm_blurEffect()
{
    Alg_DefaultEffects::applyBlur(m_image1, m_image2);

    doubleViewport->loadImage(*m_image1, *m_image2);

    historyCommands->alg_newCommand("Blur ", m_fileName1, m_image1, m_image2, *HistoryList);
}

//-History Functions-------------------------------------------------------------------------------------
void MainWindow::historyItemClicked(QListWidgetItem *)
{
    QList<QImage> tempList;
    tempList = historyCommands->alg_goHere(HistoryList->currentRow(), *HistoryList);

    doubleViewport->loadImage(tempList.at(0), tempList.at(1));

    doubleViewport->updateFilenames(m_fileName1, m_fileName2);

    setCurrentFile(m_fileName1);
}

//-App Functions-----------------------------------------------------------------------------------------
void MainWindow::about()
{
   QMessageBox::about(this, tr("About Image Processing Testbed"),
            tr("<b>Image Processing Testbed</b> is designed in order to help "
               "researchers and students test their image processing algorithms. "
               "Load an image and see in real time the effects of your algorithm. "
               "You can either develop your algorithm as an addon, or by using the "
               "embedded script editor."));
}

void MainWindow::goFullscreen(bool state)
{
   if (state)
       this->showFullScreen();
   else
       this->showNormal();
}

void MainWindow::openSettingsDialog()
{
   settingsDialog->show();
}


void MainWindow::updateSettings(int _windowSize, int _pointerDimX, int _pointerDimY, int _pointerWindowSize, int _maxFrames)
{
    m_windowSize = _windowSize;
    m_pointerDimX = _pointerDimX;
    m_pointerDimY = _pointerDimY;
    m_pointerWindowSize = _pointerWindowSize;
    m_maxFrames = _maxFrames;

    doubleViewport->setWindowSize(_windowSize);
    doubleViewport->setPointerSize(_pointerDimX, _pointerDimY, _pointerWindowSize);
    historyCommands->setMaxItems(_maxFrames);
}

void MainWindow::createActions()
{
    //-Load Image Actions--------------------------------------------------------------------------------
    loadImageAct = new QAction(QIcon(":/images/new.png"), tr("&Load Image"), this);
    loadImageAct->setShortcuts(QKeySequence::Open);
    loadImageAct->setStatusTip(tr("Load a new image file from your computer"));
    connect(loadImageAct, SIGNAL(triggered()), this, SLOT(load()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    //-Save Image Actions--------------------------------------------------------------------------------
    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current edited image in it's existing file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/saveas.png"), tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current edited image in a new file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    saveScriptAct = new QAction(QIcon(":/images/savescriptas.png"), tr("&Save Script..."), this);
    saveScriptAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
    saveScriptAct->setStatusTip(tr("Save the current script to a new file"));
    connect(saveScriptAct, SIGNAL(triggered()), this, SLOT(saveScript()));

    //-Print Image Actions-------------------------------------------------------------------------------
    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current edited image"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    printAsAct = new QAction(QIcon(":/images/printas.png"), tr("&Print As..."), this);
    printAsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
    printAsAct->setStatusTip(tr("Print both images"));
    connect(printAsAct, SIGNAL(triggered()), this, SLOT(printAs()));

    //-Edit Image Actions--------------------------------------------------------------------------------
    goStartAct = new QAction(QIcon(":/images/gostart.png"), tr("&GoStart"), this);
    goStartAct->setShortcuts(QKeySequence::Back);
    goStartAct->setStatusTip(tr("Go at the start of the history stack"));
    connect(goStartAct, SIGNAL(triggered()), this, SLOT(goStart()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last editing action"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    goEndAct = new QAction(QIcon(":/images/goend.png"), tr("&GoEnd"), this);
    goEndAct->setShortcuts(QKeySequence::Forward);
    goEndAct->setStatusTip(tr("Go at the end of the history stack"));
    connect(goEndAct, SIGNAL(triggered()), this, SLOT(goEnd()));

    //-Zoom Actions--------------------------------------------------------------------------------------
    zoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("&ZoomIn"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom in image for both workspaces"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("&ZoomOut"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom out image for both workspaces"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    zoomFitAct = new QAction(QIcon(":/images/zoomfit.png"), tr("&ZoomFit"), this);
    zoomFitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Asterisk));
    zoomFitAct->setStatusTip(tr("Fit both images to current viewport"));
    connect(zoomFitAct, SIGNAL(triggered()), this, SLOT(zoomFit()));

    //-Pointer Actions-----------------------------------------------------------------------------------
    pixelPointerAct = new QAction(QIcon(":/images/pixelpointer.png"), tr("&Pixel Pointer"), this);
    pixelPointerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    pixelPointerAct->setStatusTip(tr("Interactive pointer that displays a window with the pixels below it"));
    connect(pixelPointerAct, SIGNAL(triggered()), this, SLOT(pixelPointer()));

    diffPointerAct = new QAction(QIcon(":/images/diffpointer.png"), tr("&Difference Pointer"), this);
    diffPointerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_9));
    diffPointerAct->setStatusTip(tr("Interactive pointer that displays a window with the difference of both images at that area"));
    connect(diffPointerAct, SIGNAL(triggered()), this, SLOT(differencePointer()));

    textPointerAct = new QAction(QIcon(":/images/textpointer.png"), tr("&Text Pointer"), this);
    textPointerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_8));
    textPointerAct->setStatusTip(tr("Interactive pointer that displays a window with the grayscale value of the pixels below it"));
    connect(textPointerAct, SIGNAL(triggered()), this, SLOT(textPointer()));

    noInteractivePointerAct = new QAction(QIcon(":/images/nointeractive.png"), tr("&Simple Pointer"), this);
    noInteractivePointerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    noInteractivePointerAct->setStatusTip(tr("Use a simple pointer."));
    connect(noInteractivePointerAct, SIGNAL(triggered()), this, SLOT(noInteractive()));

    //-Switch Image Action-------------------------------------------------------------------------------
    keepImageAct = new QAction(QIcon(":/images/keepimage.png"), tr("&Keep Image"), this);
    keepImageAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Backspace));
    keepImageAct->setStatusTip(tr("Keep the edited image in place of the original."));
    connect(keepImageAct, SIGNAL(triggered()), this, SLOT(keepImage()));

    //-Algorithm Actions---------------------------------------------------------------------------------
    alg_negativeEffectAct = new QAction(QIcon(":/images/algnegative.png"), tr("&Negative Effect"), this);
    //alg_negativeEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_negativeEffectAct->setStatusTip(tr("Apply negative effect to current image."));
    connect(alg_negativeEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_negativeEffect()));

    alg_contrastEffectAct = new QAction(QIcon(":/images/algcontrast.png"), tr("&Contrast Effect"), this);
    //alg_contrastEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_contrastEffectAct->setStatusTip(tr("Apply constrast effect to current image."));
    connect(alg_contrastEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_contrastEffect()));

    alg_redEffectAct = new QAction(QIcon(":/images/algred.png"), tr("&Red Effect"), this);
    //alg_redEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_redEffectAct->setStatusTip(tr("Apply red effect to current image."));
    connect(alg_redEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_redEffect()));

    alg_grayEffectAct = new QAction(QIcon(":/images/alggray.png"), tr("&Gray Effect"), this);
    //alg_grayEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_grayEffectAct->setStatusTip(tr("Apply gray effect to current image."));
    connect(alg_grayEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_grayEffect()));

    alg_closingEffectAct = new QAction(QIcon(":/images/algclosing.png"), tr("&Closing Effect"), this);
    //alg_closingEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_closingEffectAct->setStatusTip(tr("Apply closing effect to current image."));
    connect(alg_closingEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_closingEffect()));

    alg_erosionEffectAct = new QAction(QIcon(":/images/algerosion.png"), tr("&Erosion Effect"), this);
    //alg_erosionEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_erosionEffectAct->setStatusTip(tr("Apply erosion effect to current image."));
    connect(alg_erosionEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_erosionEffect()));

    alg_sharpenEffectAct = new QAction(QIcon(":/images/algsharpen.png"), tr("&Sharpen Effect"), this);
    //alg_sharpenEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_sharpenEffectAct->setStatusTip(tr("Apply sharpen effect to current image."));
    connect(alg_sharpenEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_sharpenEffect()));

    alg_saturationEffectAct = new QAction(QIcon(":/images/algsaturation.png"), tr("&Saturation Effect"), this);
    //alg_saturationEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_saturationEffectAct->setStatusTip(tr("Apply saturation effect to current image."));
    connect(alg_saturationEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_saturationEffect()));

    alg_coolEffectAct = new QAction(QIcon(":/images/algcool.png"), tr("&Cool Effect"), this);
    //alg_coolEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_coolEffectAct->setStatusTip(tr("Apply cool color effect to current image."));
    connect(alg_coolEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_coolEffect()));

    alg_warmEffectAct = new QAction(QIcon(":/images/algwarm.png"), tr("&Warm Effect"), this);
    //alg_warmEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_warmEffectAct->setStatusTip(tr("Apply warm color effect to current image."));
    connect(alg_warmEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_warmEffect()));

    alg_edgesEffectAct = new QAction(QIcon(":/images/algedges.png"), tr("&Edges Effect"), this);
    //alg_edgesEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_edgesEffectAct->setStatusTip(tr("Trace edges for current image."));
    connect(alg_edgesEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_edgesEffect()));

    alg_sepiaEffectAct = new QAction(QIcon(":/images/algsepia.png"), tr("&Sepia Effect"), this);
    //alg_sepiaEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_sepiaEffectAct->setStatusTip(tr("Apply sepia effect to current image."));
    connect(alg_sepiaEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_sepiaEffect()));

    alg_pixelateEffectAct = new QAction(QIcon(":/images/algpixelate.png"), tr("&Pixelate Effect"), this);
    //alg_pixelateEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_pixelateEffectAct->setStatusTip(tr("Pixelate current image."));
    connect(alg_pixelateEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_pixelateEffect()));

    alg_blurEffectAct = new QAction(QIcon(":/images/algblur.png"), tr("&Blur Effect"), this);
    //alg_blurEffectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    alg_blurEffectAct->setStatusTip(tr("Apply blur effect to current image."));
    connect(alg_blurEffectAct, SIGNAL(triggered()), this, SLOT(algorithm_blurEffect()));

    //-Scripting Actions---------------------------------------------------------------------------------
    runScriptAct = new QAction(QIcon(":/images/ok.png"), tr("&Run Script"), this);
    runScriptAct->setShortcuts(QKeySequence::Refresh);
    runScriptAct->setStatusTip(tr("Run currently selected script"));
    connect(runScriptAct, SIGNAL(triggered()), this, SLOT(runScript()));

    //-App Actions---------------------------------------------------------------------------------------
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    fullscreenAct = new QAction(tr("&Fullscreen"), this);
    fullscreenAct->setShortcuts(QKeySequence::FullScreen);
    fullscreenAct->setStatusTip(tr("Toggle fullscreen"));
    fullscreenAct->setCheckable(true);
    fullscreenAct->setChecked(false);
    connect(fullscreenAct, SIGNAL(toggled(bool)), this, SLOT(goFullscreen(bool)));

    settingsAct = new QAction(QIcon(":/images/settings.png"), tr("&Settings"), this);
    settingsAct->setStatusTip(tr("Open settings dialog"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));
}

void MainWindow::createMenus()
{
    //-File Menu-----------------------------------------------------------------------------------------
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadImageAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveScriptAct);
    fileMenu->addAction(printAct);
    fileMenu->addAction(printAsAct);    
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    updateRecentFileActions();

    //-Edit Menu-----------------------------------------------------------------------------------------
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(goStartAct);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(goEndAct);

    //-Tools Menu----------------------------------------------------------------------------------------
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(runScriptAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(zoomInAct);
    toolsMenu->addAction(zoomOutAct);
    toolsMenu->addAction(zoomFitAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(noInteractivePointerAct);
    toolsMenu->addAction(pixelPointerAct);
    toolsMenu->addAction(diffPointerAct);
    toolsMenu->addAction(textPointerAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(keepImageAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(settingsAct);

    //-Algorithms Menu-----------------------------------------------------------------------------------
    algorithmsMenu = menuBar()->addMenu(tr("&Algorithms"));
    algorithmsMenu->addAction(alg_negativeEffectAct);
    algorithmsMenu->addAction(alg_contrastEffectAct);
    algorithmsMenu->addAction(alg_redEffectAct);
    algorithmsMenu->addAction(alg_grayEffectAct);
    algorithmsMenu->addAction(alg_closingEffectAct);
    algorithmsMenu->addAction(alg_erosionEffectAct);
    algorithmsMenu->addAction(alg_sharpenEffectAct);
    algorithmsMenu->addAction(alg_saturationEffectAct);
    algorithmsMenu->addAction(alg_coolEffectAct);
    algorithmsMenu->addAction(alg_warmEffectAct);
    algorithmsMenu->addAction(alg_edgesEffectAct);
    algorithmsMenu->addAction(alg_sepiaEffectAct);
    algorithmsMenu->addAction(alg_pixelateEffectAct);
    algorithmsMenu->addAction(alg_blurEffectAct);

    //-Scripts Menu--------------------------------------------------------------------------------------
    scriptsMenu = menuBar()->addMenu(tr("&Scripts"));
    scriptsMenu->setTearOffEnabled(true);               // Make the menu tearable for quick access.
    createScriptMenus();

    //-View Menu-----------------------------------------------------------------------------------------
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(fullscreenAct);
    viewMenu->addSeparator();
    viewMenu->addAction(doubleViewport->detailAct);
    viewMenu->addAction(doubleViewport->infoAct);
    viewMenu->addSeparator();

    menuBar()->addSeparator();

    //-Help Menu-----------------------------------------------------------------------------------------
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

//-Create Toolbars---------------------------------------------------------------------------------------
void MainWindow::createToolBars()
{
    //-File Toolbar--------------------------------------------------------------------------------------
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("File");
    fileToolBar->addAction(loadImageAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addAction(saveScriptAct);
    fileToolBar->addAction(printAct);
    fileToolBar->addAction(printAsAct);

    viewMenu->addAction(fileToolBar->toggleViewAction());

    //-Edit Toolbar--------------------------------------------------------------------------------------
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("Edit");
    editToolBar->addAction(goStartAct);
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addAction(goEndAct);

    viewMenu->addAction(editToolBar->toggleViewAction());

    //-Tools Toolbar-------------------------------------------------------------------------------------
    toolsToolBar = addToolBar(tr("Tools"));
    toolsToolBar->setObjectName("Tools");
    toolsToolBar->addAction(zoomInAct);
    toolsToolBar->addAction(zoomOutAct);
    toolsToolBar->addAction(zoomFitAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(noInteractivePointerAct);
    toolsToolBar->addAction(pixelPointerAct);
    toolsToolBar->addAction(diffPointerAct);
    toolsToolBar->addAction(textPointerAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(runScriptAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(settingsAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(keepImageAct);

    viewMenu->addAction(toolsToolBar->toggleViewAction());

    //-Algorithms Toolbar--------------------------------------------------------------------------------
    algorithmsToolBar = addToolBar(tr("Algorithms"));
    algorithmsToolBar->setObjectName("Algorithms");
    algorithmsToolBar->addAction(alg_negativeEffectAct);
    algorithmsToolBar->addAction(alg_contrastEffectAct);
    algorithmsToolBar->addAction(alg_redEffectAct);
    algorithmsToolBar->addAction(alg_grayEffectAct);
    algorithmsToolBar->addAction(alg_closingEffectAct);
    algorithmsToolBar->addAction(alg_erosionEffectAct);
    algorithmsToolBar->addAction(alg_sharpenEffectAct);
    algorithmsToolBar->addAction(alg_saturationEffectAct);
    algorithmsToolBar->addAction(alg_coolEffectAct);
    algorithmsToolBar->addAction(alg_warmEffectAct);
    algorithmsToolBar->addAction(alg_edgesEffectAct);
    algorithmsToolBar->addAction(alg_sepiaEffectAct);
    algorithmsToolBar->addAction(alg_pixelateEffectAct);
    algorithmsToolBar->addAction(alg_blurEffectAct);

    viewMenu->addAction(algorithmsToolBar->toggleViewAction());
    viewMenu->addSeparator();
}

//-Create Status Bar-------------------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//-Create Dockable Widgets-------------------------------------------------------------------------------
void MainWindow::createDockWindows()
{
    //-History Stack Widget------------------------------------------------------------------------------
    QDockWidget *dock = new QDockWidget(tr("History"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setObjectName("History");
    HistoryList = new QListWidget(dock);
    HistoryList->setMinimumWidth(200);
    dock->setWidget(HistoryList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    connect(HistoryList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(historyItemClicked(QListWidgetItem*)));
    viewMenu->addAction(dock->toggleViewAction());

    //-Script Editor Widget------------------------------------------------------------------------------
    dock = new QDockWidget(tr("Script"), this);
    dock->setObjectName("Script");
    scriptEditor = new ScriptEditor(dock);
    scriptEditor->setMinimumWidth(200);
    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/etc/completer/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    scriptEditor->setCompleter(completer);
    dock->setWidget(scriptEditor);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

//-Initial Actions State---------------------------------------------------------------------------------
void MainWindow::initialState(bool state)
{
    saveAct->setEnabled(state);
    saveAsAct->setEnabled(state);
    printAct->setEnabled(state);
    printAsAct->setEnabled(state);

    editToolBar->setEnabled(state);
    toolsToolBar->setEnabled(state);
    algorithmsToolBar->setEnabled(state);

    editMenu->setEnabled(state);
    toolsMenu->setEnabled(state);
    algorithmsMenu->setEnabled(state);
}

//-Helper Functions--------------------------------------------------------------------------------------
QList<int> MainWindow::getPixel(int x, int y)
{
    QList<int> pixelValues;
    pixelValues = doubleViewport->pixelValueAt(x, y);
    return pixelValues;
}

void MainWindow::setPixel(int x, int y, QList<int> pVal)
{
    doubleViewport->pixelValueAt(x, y, pVal);
}

int MainWindow::getWidth()
{
    return doubleViewport->imageWidth();
}

int MainWindow::getHeight()
{
    return doubleViewport->imageHeight();
}

void MainWindow::updateImage()
{
    doubleViewport->updatePixelMap();
}

//-File Format-------------------------------------------------------------------------------------------
FileFormats MainWindow::fileFormat(const QString &fileName)
{
    if (fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
            fileName.endsWith(".jpeg", Qt::CaseInsensitive))
    {
        return JPG;
    }
    else if (fileName.endsWith(".j2k", Qt::CaseInsensitive) ||
             fileName.endsWith(".jpg2000", Qt::CaseInsensitive))
    {
        return J2K;
    }
    else if (fileName.endsWith(".png", Qt::CaseInsensitive))
    {
        return PNG;
    }
    else if (fileName.endsWith(".bmp", Qt::CaseInsensitive))
    {
        return BMP;
    }
    else if (fileName.endsWith(".tga", Qt::CaseInsensitive))
    {
        return TGA;
    }
    else if (fileName.endsWith(".tiff", Qt::CaseInsensitive))
    {
        return TIFF;
    }
    else if (fileName.endsWith(".bin", Qt::CaseInsensitive))
    {
        return BINARY;
    }
    else if (fileName.endsWith(".i2dc", Qt::CaseInsensitive))
    {
        return I2DC;
    }
    else if (fileName.endsWith(".l2dc", Qt::CaseInsensitive))
    {
        return L2DC;
    }
}

//-Saving Intermediate-----------------------------------------------------------------------------------
bool MainWindow::saveData(const QString &fileName, FileFormats format)
{
    if (format == BINARY)
    {
        return saveDataInBINARYFormat(fileName);
    }
    else if (format == I2DC)
    {
        return saveDataInI2DCFormat(fileName);
    }
    else if (format == L2DC)
    {
        return saveDataInL2DCFormat(fileName);
    }
    else
    {
        return saveDataInDefaultFormat(fileName);
    }
}

//-------------------------------------------------------------------------------------------------------

//-Saving Functions--------------------------------------------------------------------------------------
bool MainWindow::saveDataInDefaultFormat(const QString &fileName)
{
    return m_image2->save(fileName, 0, -1);
}

bool MainWindow::saveDataInBINARYFormat(const QString &fileName)
{
    return true;
}

bool MainWindow::saveDataInI2DCFormat(const QString &fileName)
{
    return true;
}

bool MainWindow::saveDataInL2DCFormat(const QString &fileName)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------

//-Loading Intermediate----------------------------------------------------------------------------------
bool MainWindow::loadData(const QString &fileName, FileFormats format)
{
    if (format == BINARY)
    {
        return loadDataInBINARYFormat(fileName);
    }
    else if (format == I2DC)
    {
        return loadDataInI2DCFormat(fileName);
    }
    else if (format == L2DC)
    {
        return loadDataInL2DCFormat(fileName);
    }
    else
    {
        return loadDataInDefaultFormat(fileName);
    }
}

//-Loading Functions-------------------------------------------------------------------------------------
bool MainWindow::loadDataInDefaultFormat(const QString &fileName)
{
    *m_image1 = QImage(fileName);
    *m_image2 = QImage(fileName);

    return doubleViewport->loadImage(*m_image1, *m_image2);
}

bool MainWindow::loadDataInBINARYFormat(const QString &fileName)
{
    return true;
}

bool MainWindow::loadDataInI2DCFormat(const QString &fileName)
{
    return true;
}

bool MainWindow::loadDataInL2DCFormat(const QString &fileName)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------

//-Recent Files Functions--------------------------------------------------------------------------------
void MainWindow::setCurrentFile(const QString &fileName)
{
    if (!fileName.isEmpty())
    {
        curFile = fileName;
        setWindowFilePath(curFile);

        QSettings settings(QCoreApplication::applicationDirPath() + "/state", "ImageProcessingTestbed");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
            files.removeLast();
        settings.setValue("recentFileList", files);
        foreach (QWidget *widget, QApplication::topLevelWidgets())
        {
            MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
            if (mainWin)
                mainWin->updateRecentFileActions();
        }
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/state", "ImageProcessingTestbed");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//-Completer Functions-----------------------------------------------------------------------------------
QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}

//-Scripting Functions-----------------------------------------------------------------------------------
void MainWindow::openScriptFile(QString* sourcecode)
{
    scriptEditor->setPlainText(*sourcecode);
}

void MainWindow::createScriptMenus()
{
    m_manager->returnScriptActions(*scriptsMenu);
}

void MainWindow::deleteScriptMenus()
{
   m_manager->removeScriptActions(*scriptsMenu);
}

void MainWindow::directoryChanged(const QString &path, bool initialize)
{
    //qDebug() << tr("Scripts directory change detected.");

    QStringList files = m_manager->listScriptsinDir(path);

    if (!initialize)
    {
        // Remove deleted scripts
        QMutableListIterator<ScriptManager::Script> i(m_manager->m_scriptList);
        while (i.hasNext())
        {
            ScriptManager::Script script = i.next();
            if (files.contains(script.fileName))
            {
                files.removeAll(script.fileName);
            }
            else
            {
                i.remove();
            }
        }
    }

    // Add new scripts
    foreach (const QString &fileName, files)
    {
        QFileInfo fi(fileName);
        // init.sconf, if needed, is only to be used for one-time configurations
        // usually at start-up
        if ((fi.fileName() != QLatin1String("init.sconf")))
        {
           QSettings reader((m_manager->defaultDirectory() + QLatin1Char('/') + fileName), QSettings::IniFormat);
           ScriptManager::Script script;
           script.fileName = fileName;
           script.name = fi.baseName();
           m_manager->fillScriptInfo(&script, &reader);
           QString scriptState;
           scriptState = m_manager->checkScriptValid(&script);
           if(scriptState == "OK")
           {
               m_manager->m_scriptList.append(script);
           }
           else
           {
               qWarning() << scriptState;
           }
        }
    }

    // Refresh scripts menu
    deleteScriptMenus();
    createScriptMenus();
}

void MainWindow::runScript()
{
   m_manager->runScriptFile(scriptEditor->toPlainText());
}
