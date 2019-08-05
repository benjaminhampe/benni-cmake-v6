#ifndef WINDOW_H
#define WINDOW_H

#include <QIrrlichtWidget_2016.h>
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
class Window : public QWidget
// ============================================================================
{
    Q_OBJECT
public:
    Window(QWidget* parent = 0);
    virtual ~Window();
    	
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
	
    // irr::scene::ISceneCollisionManager* _collMgr = nullptr;
    // irr::scene::ISceneNode* _currNode = nullptr;
    // irr::scene::ISceneNode* _lastNode = nullptr;
};

#endif