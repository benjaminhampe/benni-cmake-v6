//////////////////////////////////////////////////////////////////////
/// @file MainWindow.cpp
/// @author benjaminhampe@gmx.de
//////////////////////////////////////////////////////////////////////

#ifndef TEST_QT_MAINWINDOW_HPP
#define TEST_QT_MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QTimer>

/// @brief Implementation for the main window.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
   MainWindow( QWidget * parent = nullptr );
   ~MainWindow();

protected:
   void keyPressEvent( QKeyEvent * );
   void closeEvent( QCloseEvent * );

private slots:
private:
   bool m_Closing;
   QTimer m_FrameTimer;
   QTimer m_MemoryStatisticsTimer;
};

#endif // TEST_QT_MAINWINDOW_HPP
