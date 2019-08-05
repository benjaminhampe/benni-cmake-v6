#include "addTextBillboard.hpp"

void
addTextBillboard( irr::scene::ISceneManager* smgr,
                  float32_t const lineHeight,
                  std::string const & textMessage,
                  float32_t const x, float32_t const y, float32_t const z,
                  uint32_t const colorTop, uint32_t const colorBottom )
{
    irr::gui::IGUIEnvironment * guienv = smgr->getGUIEnvironment();
    if ( !guienv )
    {
        // Log error and abort;
        return;
    }

    irr::gui::IGUIFont* font = guienv->getBuiltInFont();
    if ( !font )
    {
        // Log error and abort;
        return;
    }

    irr::core::stringw const text( irr::core::stringc( textMessage.c_str() ) );

    irr::core::dimension2du const textSize = font->getDimension( text.c_str() );

    irr::scene::ISceneNode* rootNode = smgr->getRootSceneNode();

    float32_t const ratio = float32_t( textSize.Width ) / float32_t( textSize.Height );

    smgr->addBillboardTextSceneNode(
                    smgr->getGUIEnvironment()->getBuiltInFont(),
                    irr::core::stringw( irr::core::stringc( text.c_str() ) ).c_str(),
                    rootNode,
                    irr::core::dimension2df( ratio * lineHeight, lineHeight ),
                    irr::core::vector3df( x,y,z ), -1, colorTop, colorBottom );
}