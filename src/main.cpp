#include "mainwindow.h"
#include <QApplication>

/*
 * Name of this application.
 */
#define APPNAME "Image Processing Testbed"

/*
 * Publisher of this application.
 */
#define ORGANIZATION "greenleaf"

/*
 * Publishers' domain.
 */
#define ORGANIZATION_DOMAIN "greenleaf@gmail.com"

/*
 * Short description of this application.
 */
#define DESCRIPTION "Test and interactively explore image processing algorithms."

/*
 * Minor and major version.
 */
#define APP_VERSION_MAJOR 0
#define APP_VERSION_MINOR 1

// ISSUES
// Incorrect synchronisation for zoom. For some reason it synchronises (or updates) only on resize.
// Drag and drop from one workspace to another.
// Drag and drop to load image.
// Scripting.
int main(int argc, char *argv[])
{
    // Initialize the core application.
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(icons);
    MainWindow mainWin;
    mainWin.show();

    app.setApplicationName(APPNAME);
    app.setOrganizationName(ORGANIZATION);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);

    return app.exec();
}
