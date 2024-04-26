#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Image Labeling Tool");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Image Labeling Tool");
    parser.addHelpOption();
    //parser.addVersionOption();

    // source
    QCommandLineOption sourceDirectoryOption(QStringList() << "i" << "image-directory",
            QApplication::translate("main", "Image or project source: <directory>."),
            QApplication::translate("main", "directory"));
    parser.addOption(sourceDirectoryOption);

    // target
    QCommandLineOption targetDirectoryOption(QStringList() << "o" << "output-directory",
            QApplication::translate("main", "Output directory: <directory>."),
            QApplication::translate("main", "directory"));
    parser.addOption(targetDirectoryOption);

    // label mode
    QCommandLineOption showProgressOption("l", QCoreApplication::translate("main", "Enable overlaping label mode"));
    parser.addOption(showProgressOption);
    parser.process(app);

    MainWindow w;
    QString sourceDir = parser.value(sourceDirectoryOption);
    QString targetDir = parser.value(targetDirectoryOption);
    bool labelMode = parser.isSet(showProgressOption);
    if(!sourceDir.isEmpty()){
        if(targetDir.isEmpty()){
            w.openDirectory(sourceDir, sourceDir, labelMode);
        } else {
            w.openDirectory(sourceDir, targetDir, labelMode);
        }
    }
    w.show();

    return app.exec();
}
