//////////////////////////////////////////////////////////////////////
/// @file MainWindow.cpp
/// @author benjaminhampe@gmx.de
//////////////////////////////////////////////////////////////////////
#include "MainWindow.hpp"

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QMouseEvent>
#include <QScreen>
#include <QString>
#include <QUrl>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow( QWidget * parent )
   : QMainWindow( parent )
   , m_Closing( false )
{
}

MainWindow::~MainWindow()
{
}

void
MainWindow::keyPressEvent( QKeyEvent * ke )
{
   if ( Qt::Key_Escape == ke->key() )
   {
      this->close();
   }
}


void
MainWindow::closeEvent( QCloseEvent * )
{
   m_Closing = true;
   this->close();
}