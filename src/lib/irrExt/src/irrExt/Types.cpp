#include <irrExt/Types.hpp>

#include <assert.h>
#include <fstream>
#include <vector>
#include <array>
#include <memory>
// #include <windows.h>
#include <time.h>

namespace irrExt {

#if 0

class irrBackend
{
public:
    irrBackend();

    static void
    closeDevice( irr::IrrlichtDevice* & device )
    {
        if (device)
        {
            device->closeDevice();
            device->drop();
            device = nullptr;
        }
    }

    static irr::IrrlichtDevice*
    createNullDevice()
    {
        irr::IrrlichtDevice * device = irr::createDevice( irr::video::EDT_NULL );
        return device;
    }

    static irr::video::IImage*
    createImage( irr::video::IVideoDriver* driver, int32_t w, int32_t h, irr::video::SColor color = irr::video::SColor(255,0,200,0) )
    {
        irr::video::IImage* img = driver->createImage( irr::video::ECF_A8R8G8B8, irr::core::dimension2du(w,h) );
        img->fill( color );
        return img;
    }

    static irr::video::IImage*
    loadImage( irr::video::IVideoDriver* driver, std::string fileName )
    {
        irr::video::IImage* img = driver->createImageFromFile( fileName.c_str() );
        return img;
    }

    static bool
    saveImage( irr::video::IVideoDriver* driver, irr::video::IImage * img, std::string fileName )
    {
        return driver->writeImageToFile( img, fileName.c_str(), 0 );
    }

    static irr::IrrlichtDevice*
    createOpenGLDevice( int32_t w, int32_t h, bool fullscreen = false )
    {
        irr::SIrrlichtCreationParameters params;
        params.WindowSize = irr::core::dimension2du(
            static_cast< int32_t >( w ),
            static_cast< int32_t >( h ) );
        params.Bits = 32;
        params.AntiAlias = irr::video::EAAM_QUALITY;
        params.Doublebuffer = true;
        params.Vsync = false;
        params.DriverType = irr::video::EDT_OPENGL;
        params.HighPrecisionFPU = true;
        params.Fullscreen = fullscreen;
        params.Stencilbuffer = true;
        params.ZBufferBits = 24;
        irr::IrrlichtDevice* device = irr::createDeviceEx( params );
        if (!device)
        {
            std::cout << "ERROR: Cannot create device\n";
        }
        return device;
    }

    static void
    runDevice( irr::IrrlichtDevice * device )
    {
        if ( !device )
        {
            return;
        }

        irr::video::IVideoDriver* driver = device->getVideoDriver();
        irr::video::SColor clearColor( 255, 200, 200, 220 );
        irr::scene::ISceneManager* smgr = device->getSceneManager();
        irr::scene::ISceneNode* root = smgr->getRootSceneNode();
        irr::scene::IMesh* mesh = smgr->getGeometryCreator()->createSphereMesh( 50.f, 33, 33 );
        irr::scene::IMeshSceneNode* sphere = smgr->addMeshSceneNode( mesh, root, -1 );
        mesh->drop();
        sphere->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        sphere->setMaterialFlag( irr::video::EMF_WIREFRAME, true );

        irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS( root, 100.0f, 0.5f, -1 );
        camera->setPosition( irr::core::vector3df( 0,0,-1000) );
        camera->setTarget( irr::core::vector3df(0,0,0) );
        camera->setNearValue( 1.0f );
        camera->setFarValue( 10000.0f );

        while (device->run())
        {
//            if (device->isWindowActive())
//            {
                driver->beginScene( true, true, clearColor );
                smgr->drawAll();
                driver->endScene();
//            }
//            else
//            {
//                device->yield();
//            }
        }
    }

    static irr::scene::IMeshSceneNode *
    createEarthSceneNode( irr::IrrlichtDevice* device )
    {
        irr::video::IVideoDriver* driver = device->getVideoDriver();
        irr::scene::ISceneManager* smgr = device->getSceneManager();
        irr::scene::ISceneNode* root = smgr->getRootSceneNode();
        irr::scene::IMesh* sphere = smgr->getGeometryCreator()->createSphereMesh( 50.f, 33, 33 );
        irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode( sphere, root, -1 );
        sphere->drop();
        node->setMaterialFlag( irr::video::EMF_FOG_ENABLE, false );
        node->setMaterialFlag( irr::video::EMF_NORMALIZE_NORMALS, true );
        node->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        //node->setMaterialFlag( irr::video::EMF_WIREFRAME, true );
        irr::video::ITexture * dayTex = driver->getTexture( "../../media/earth_day_8k.jpg" );
        node->setMaterialTexture( 0, dayTex );
        return node;
    }
};

#endif


} // end namespace irrExt






/*
<?xml version="1.0" encoding="utf-8"?>
<empleados>
 <cantidad>UnaCantidad</cantidad>
 <empleado>
  <idEmpleado>1</idEmpleado>
  <nombre>UnNombre1</nombre>
  <apellidos>UnosApellidos1</apellidos>
 </empleado>
 <empleado>
  <idEmpleado>2</idEmpleado>
  <nombre>UnNombre2</nombre>
  <apellidos>UnosApellidos2</apellidos>
 </empleado>
</empleados>
*/

#if 0
std::string
SIrrlichtCreationParameters_toXML( irr::SIrrlichtCreationParameters const & cfg )
{
    std::stringstream s;
    s << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    s << "<pioneers3d>\n";
    s << "<config>\n";
    s << "\t<device>" << (int)cfg.DeviceType << "</device>\n";
    s << "\t<driver>" << (int)cfg.DriverType << "</driver>\n";
    s << "\t<loglevel>" << (int)cfg.LoggingLevel << "</loglevel>\n";
    s << "\t<width>" << (int)cfg.WindowSize.Width << "</width>\n";
    s << "\t<height>" << (int)cfg.WindowSize.Height << "</height>\n";
    s << "\t<bits>" << (int)cfg.Bits << "</bits>\n";
    s << "\t<antialias>" << (int)cfg.AntiAlias << "</antialias>\n";
    s << "\t<doublebuffer>" << (int)cfg.Doublebuffer << "</doublebuffer>\n";
    s << "\t<vsync>" << (int)cfg.Vsync << "</vsync>\n";
    s << "\t<fullscreen>" << (int)cfg.Fullscreen << "</fullscreen>\n";
    s << "\t<zbufferbits>" << (int)cfg.ZBufferBits << "</zbufferbits>\n";
    s << "\t<stencilbuffer>" << (int)cfg.Stencilbuffer << "</stencilbuffer>\n";
    s << "</config>\n";
    s << "</pioneers3d>\n";
    return s.str();
}

std::string
toXML( irr::video::IVideoModeList const & modeList )
{
    std::stringstream s;
    s << "<video-modes>\n";
    for ( uint32_t i = 0; i < modeList.getVideoModeCount(); ++i )
    {
        uint32_t w = modeList.getVideoModeResolution( i ).Width;
        uint32_t h = modeList.getVideoModeResolution( i ).Height;
        uint32_t d = modeList.getVideoModeDepth( i );
        s << "\t<mode><w>" << w << "</w><h>" << h << "</h><bpp>" << d << "</bpp></mode>\n";
    }
    s << "</video-modes>\n";
    return s.str();
}

std::string
toINI( irr::SIrrlichtCreationParameters const & cfg )
{
    std::stringstream s;
    //s << "[WindowSize]\n" << cfg.WindowSize.Width << "," << cfg.WindowSize.Height << "," << cfg.Bits << "\n";
    s << "[DeviceType]\n" << cfg.DeviceType << "\n";
    s << "[DriverType]\n" << cfg.DriverType << "\n";
    s << "[LogLevel]\n" << cfg.LoggingLevel << "\n";
    s << "[WindowWidth]\n" << cfg.WindowSize.Width << "\n";
    s << "[WindowHeight]\n" << cfg.WindowSize.Height << "\n";
    s << "[BitsPerPixel]\n" << cfg.Bits << "\n";
    s << "[AntiAliasing]\n" << cfg.AntiAlias << "\n";
    s << "[Doublebuffer]\n" << cfg.Doublebuffer << "\n";
    s << "[Vsync]\n" << cfg.Vsync << "\n";
    s << "[Fullscreen]\n" << cfg.Fullscreen << "\n";
    s << "[ZBufferBits]\n" << cfg.ZBufferBits << "\n";
    s << "[Stencilbuffer]\n" << cfg.Stencilbuffer << "\n";
}

bool
fromINI( std::string const & lines, std::vector< std::string > & keys, std::vector< std::string > & values )
{
    std::cout << __FUNCTION__ << "()\n" << lines << "\n\nEXTRACT NOW...\n";

    size_t i = 0;
    size_t line = 0;
    size_t keyStart;
    size_t keyCount;
    size_t valueStart;
    size_t valueCount;
    std::string key;
    std::string value;

    while( i < lines.size() )
    {
        i = lines.find( '[', i );

        if ( i == std::string::npos )
        {
            break;
        }

        ++i;

        if ( i >= lines.size() ) { break; } // validate 'i'

        keyStart = i;

        i = lines.find( ']', i );

        if ( i == std::string::npos )
        {
            break;
        }

        if ( i <= keyStart + 1 ) { break; } // validate 'i'

        keyCount = i - 1 - keyStart; // Is there something between '[' and ']' ?

        //assert( i >= keyStart ); // Crash program if '[' and ']' are not correctly used, remove for -Wno-dev

        if ( keyCount > 0 ) // If so, extract key
        {
            key = lines.substr( keyStart, keyCount );

            std::cout << __FUNCTION__ << " :: Found key (" << key << ")\n";
        }

        // Now search for EOL after key and extract ...
        i = lines.find( '\n', i + 1 );

        if ( i == std::string::npos )
        {
            break;
        }

        ++i;

        if ( i >= lines.size() ) { break; } // validate 'i'

        valueStart = i;

        ++line;

        // ... everything between next EOL and call it value ( not trimmed, raw )
        i = lines.find( '\n', i + 1 );

        if ( i == std::string::npos )
        {
            break;
        }

        valueCount = i - valueStart;

        ++i;

        if ( i >= lines.size() ) { break; } // validate 'i'

        ++line;

        // Is there something to extract?
        if ( valueCount > 0 )
        {
            // If so, extract the raw value
            value = lines.substr( valueStart, valueCount );

            // Trim whitespaces
            trim( value );

            // trim all whitespace before and after any alpha-numeric character
            std::cout << __FUNCTION__ << " :: [OK] Found { " << key << ": " << value << "; } at line (" << line << ")\n";
            keys.push_back( key );
            values.push_back( value );
        }
        else
        {
            std::cout << __FUNCTION__ << " :: [ERROR] Invalid value for key (" << key << ")\n";
        }
    }
    return true;
}

/*
<?xml version="1.0" encoding="utf-8"?>
<empleados>
 <cantidad>UnaCantidad</cantidad>
 <empleado>
  <idEmpleado>1</idEmpleado>
  <nombre>UnNombre1</nombre>
  <apellidos>UnosApellidos1</apellidos>
 </empleado>
 <empleado>
  <idEmpleado>2</idEmpleado>
  <nombre>UnNombre2</nombre>
  <apellidos>UnosApellidos2</apellidos>
 </empleado>
</empleados>
*/


std::string
toXML( irr::SIrrlichtCreationParameters const & cfg )
{
    std::stringstream s;
    s << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    s << "<pioneers3d>\n";
    s << "<config>\n";
    s << "\t<device>" << (int)cfg.DeviceType << "</device>\n";
    s << "\t<driver>" << (int)cfg.DriverType << "</driver>\n";
    s << "\t<loglevel>" << (int)cfg.LoggingLevel << "</loglevel>\n";
    s << "\t<width>" << (int)cfg.WindowSize.Width << "</width>\n";
    s << "\t<height>" << (int)cfg.WindowSize.Height << "</height>\n";
    s << "\t<bits>" << (int)cfg.Bits << "</bits>\n";
    s << "\t<antialias>" << (int)cfg.AntiAlias << "</antialias>\n";
    s << "\t<doublebuffer>" << (int)cfg.Doublebuffer << "</doublebuffer>\n";
    s << "\t<vsync>" << (int)cfg.Vsync << "</vsync>\n";
    s << "\t<fullscreen>" << (int)cfg.Fullscreen << "</fullscreen>\n";
    s << "\t<zbufferbits>" << (int)cfg.ZBufferBits << "</zbufferbits>\n";
    s << "\t<stencilbuffer>" << (int)cfg.Stencilbuffer << "</stencilbuffer>\n";
    s << "</config>\n";
    s << "</pioneers3d>\n";
    return s.str();
}


std::string
toXML( irr::video::IVideoModeList const & modeList )
{
    std::stringstream s;
    s << "<video-modes>\n";
    for ( uint32_t i = 0; i < modeList.getVideoModeCount(); ++i )
    {
        uint32_t w = modeList.getVideoModeResolution( i ).Width;
        uint32_t h = modeList.getVideoModeResolution( i ).Height;
        uint32_t d = modeList.getVideoModeDepth( i );
        s << "\t<mode><w>" << w << "</w><h>" << h << "</h><bpp>" << d << "</bpp></mode>\n";
    }
    s << "</video-modes>\n";
    return s.str();
}

bool
fromXML( std::string const & fileName, irr::SIrrlichtCreationParameters & params )
{
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile( fileName.c_str() );
    if (eResult != tinyxml2::XML_SUCCESS) return false;

    tinyxml2::XMLNode* pioneersNode = xml.FirstChildElement("pioneers3d");
    if (pioneersNode == nullptr) return false;

    tinyxml2::XMLNode* configRoot = pioneersNode->FirstChildElement("config");
    if (configRoot == nullptr) return false;

    int value;

    // [lambda]
    auto extractInt = [ configRoot ] ( std::string const & key, int & returnValue ) -> bool
    {
        if ( !configRoot ) return false;
        tinyxml2::XMLElement* configItem = configRoot->FirstChildElement( key.c_str() );
        if ( !configItem ) return false;
        if ( configItem->QueryIntText( &returnValue ) != tinyxml2::XML_SUCCESS ) return false;
        return true;
    };

    if ( extractInt( "device", value ) ){       params.DeviceType = static_cast< irr::E_DEVICE_TYPE >( value ); }
    if ( extractInt( "driver", value ) ){       params.DriverType = static_cast< irr::video::E_DRIVER_TYPE >( value ); }
    if ( extractInt( "width", value ) ){        params.WindowSize.Width = static_cast< unsigned int >( value ); }
    if ( extractInt( "height", value ) ){       params.WindowSize.Height = static_cast< unsigned int >( value ); }
    if ( extractInt( "loglevel", value ) ){     params.LoggingLevel = static_cast< irr::ELOG_LEVEL >( value ); }
    if ( extractInt( "bits", value ) ){         params.Bits = static_cast< unsigned char >( value ); }
    if ( extractInt( "antialias", value ) ){    params.AntiAlias = static_cast< irr::video::E_ANTI_ALIASING_MODE >( value ); }
    if ( extractInt( "doublebuffer", value ) ){ params.Doublebuffer = static_cast< bool >( value ); }
    if ( extractInt( "vsync", value ) ){        params.Vsync = static_cast< bool >( value ); }
    if ( extractInt( "fullscreen", value ) ){   params.Fullscreen = static_cast< bool >( value ); }
    if ( extractInt( "zbufferbits", value ) ){  params.ZBufferBits = static_cast< unsigned char >( value ); }
    if ( extractInt( "stencilbuffer", value ) ){params.Stencilbuffer = static_cast< bool >( value ); }
   return true;
}

#endif
