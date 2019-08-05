#ifndef QIRR_MAINWINDOW_HPP
#define QIRR_MAINWINDOW_HPP

#include <irrExt/QIrrlichtWidget2019.hpp>
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

typedef QHBoxLayout HBox;
typedef QVBoxLayout VBox;

// ============================================================================
class MainWindow : public QWidget
// ============================================================================
{
Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);
	virtual ~MainWindow();

signals:
	public slots:
	virtual void onCreate();

protected:
	/// 2D GUI Stuff
	QLineEdit* _fileName = nullptr;
	QLineEdit* _boardName = nullptr;
	QComboBox* _boardType = nullptr;
	QSpinBox* _tileSpacing = nullptr;
	QSpinBox* _tileCountX = nullptr;
	QSpinBox* _tileCountZ = nullptr;
	QPushButton* _createBoard = nullptr;

	/// 3D Irrlicht stuff    
	QIrrlichtWidget* _irrlicht = nullptr;
};

#endif