#include <Window.h>

// ============================================================================
Window::Window(QWidget* parent)
// ============================================================================
    : QWidget(parent)
{
    HBox* layout = new HBox;
    
    _fileName = new QLineEdit( "Untitled.board", this);
    _boardName = new QLineEdit( "Untitled", this);

    _boardType = new QComboBox( this );
    _boardType->addItem( "Rectangular");
    _boardType->addItem( "Rhombic");
    _boardType->addItem( "Circular");

    _tileSpacing = new QSpinBox( this );
    _tileSpacing->setRange( 0, 100 );
    _tileSpacing->setValue( 0 );
    
    _tileCountX = new QSpinBox( this );
    _tileCountX->setRange( 1, 100 );
    _tileCountX->setValue( 10 );
    
    _tileCountZ = new QSpinBox( this );
    _tileCountZ->setRange( 1, 100 );
    _tileCountZ->setValue( 10 );
        
    _createBoard = new QPushButton("Create Board", this);
    
    QFormLayout* form = new QFormLayout;
    form->addRow( "File-Name:", _fileName);
    form->addRow( "Board-Name:", _boardName);
    form->addRow( "Board-Type:", _boardType);
    form->addRow( "Tile-Spacing:", _tileSpacing);
    form->addRow( "Tile-Count-X:", _tileCountX);
    form->addRow( "Tile-Count-Z:", _tileCountZ);
    form->addRow( "Create Board", _createBoard);
    
	QGroupBox* group = new QGroupBox("Board Window", this);
    group->setLayout( form );
    layout->addWidget( group );
    
	/// Create Irrlicht
	
    _irrlicht = new QIrrlichtWidget();         
	irr::scene::ISceneManager* smgr = _irrlicht->getSceneManager();
    irr::video::IVideoDriver* driver = _irrlicht->getVideoDriver();
	
	/// Add Irrlicht to QtLayoutManager
	
    layout->addWidget( _irrlicht,1 );
    setLayout( layout );
	
	/// Create some 3D object in irrlicht to show the user
	
    irr::scene::ISceneNode* rootNode = smgr->getRootSceneNode();
    irr::scene::IMeshSceneNode* cubeNode = smgr->addCubeSceneNode( 100.0f, rootNode );
    if (cubeNode)
    {
        cubeNode->setRotation( irr::core::vector3df(15,0,0) );
        cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        cubeNode->setMaterialTexture(0, driver->getTexture( "../media/cube.png" ) );
        
        irr::scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator( irr::core::vector3df(0,2.8,0) );
        cubeNode->addAnimator( anim );
        anim->drop();
    }

    // ~this code is just to create something to look at
    
	
    // connect( _createBoard, SIGNAL(released()), this, SLOT(onCreate()) );
    
    // irr::IrrlichtDevice* device = _irrlicht->getIrrlichtDevice();
    
    // _board.init( device );
    
    // _collMgr = device->getSceneManager()->getSceneCollisionManager();
        
}

// ============================================================================
Window::~Window()
// ============================================================================
{

}

// ============================================================================
void Window::onCreate()
// ============================================================================
{
//    _board.BoardType = _boardType->currentIndex();
//    _board.TileSpacing= 0.1f*_tileSpacing->value();
//    _board.TileCountX = _tileCountX->value();
//    _board.TileCountZ = _tileCountZ->value();
//    _board.createBoard();
}
