#include <QApplication>
#include <QCommandLineParser>
#include <QWidget>
#include <QMainWindow>

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "FontGlyphInfoWidget.hpp"

#include <iostream>

int main( int argc, char * argv[] )
{
   int retCode = 0;
	
   char const * appName = "test.qt (c) 2019 by Benjamin Hampe <benjaminhampe@gmx.de>";
	
   srand( (unsigned int)time( nullptr ) );

   std::cout << "// ==========================================================\n";
   std::cout << "// " << appName << "\n";
   std::cout << "// ==========================================================\n";

   QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );
   QString const appPath = qApp->applicationDirPath() + "/";
   QString const styleFilePath( appPath + "themes/darcula.css" );
   app.setStyleSheet( "file:///" + styleFilePath );
   FontGlyphInfoWidget* mainWindow = new FontGlyphInfoWidget( nullptr );
   mainWindow->setWindowTitle( appName );
   mainWindow->show();
   retCode = app.exec();
   delete mainWindow;
   return retCode;
}