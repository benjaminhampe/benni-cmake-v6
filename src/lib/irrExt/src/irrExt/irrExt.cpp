#include <irrExt/irrExt.hpp>

irr::gui::IGUIButton*
createButton(  irr::gui::IGUIEnvironment* env,
               irr::gui::IGUIElement* parent,
               irr::core::recti const & pos,
               std::string name,
               irr::video::ITexture* tex  )
{
   assert( env );
   std::cout << __FUNCTION__ << "(" << pos << ")\n";
   if ( !parent ) parent = env->getRootGUIElement();

   irr::gui::IGUIButton* button = env->addButton( pos, parent, -1, L"", irr::core::stringw( name.c_str() ).c_str() );
   button->setImage( tex );
   //button->setScaleImage( true );
   return button;
}

CGUIEditorSceneNode::CGUIEditorSceneNode(
        irr::gui::IGUIEnvironment* env,
        irr::gui::IGUIElement* parent,
        int id,
        irr::core::recti const & pos )
   : irr::gui::IGUIElement( irr::gui::EGUIET_ELEMENT, env, parent, id, pos )
{
   std::cout << "[Info] " << __FUNCTION__ << "\n";

   // this->Environment->addButton( )
}

CGUIEditorSceneNode::~CGUIEditorSceneNode()
{
   std::cout << "[Info] " << __FUNCTION__ << "\n";
}

bool
CGUIEditorSceneNode::OnEvent( irr::SEvent const & event )
{
   return irr::gui::IGUIElement::OnEvent( event );
}

void
CGUIEditorSceneNode::draw()
{
   std::cout << "[Info] " << __FUNCTION__ << "\n";

   if ( Environment && isVisible() )
   {
      irr::video::IVideoDriver* driver = Environment->getVideoDriver();
      if ( driver )
      {
//         int b = 5;
//         int x = AbsoluteRect.UpperLeftCorner.X;
//         int y = AbsoluteRect.UpperLeftCorner.Y;
//         int w = AbsoluteRect.getWidth();
//         int h = AbsoluteRect.getHeight();
//         int lineHeight = 15;
//         irr::core::recti r_name  = mkRect( x+b, y+b, w-2*b, lineHeight);
//         irr::core::recti r_image = mkRect( x+b, y+b+lineHeight, w-2*b, h - 3*b - 2*lineHeight );
//         irr::core::recti r_value = mkRect( x+b, y+h-b-lineHeight, w-2*b, lineHeight );

//         Font_draw( m_Title.Font, m_Title.Text, r_name, m_Title.Color );
//         if ( m_Tex )
//         {
//            driver->draw2DImage( m_Tex, r_image, mkRect(0,0,m_Tex->getOriginalSize()), nullptr, nullptr, true );
//         }
//         Font_draw( m_Value.Font, m_Value.Text, r_name, m_Value.Color );

      }

   }

    irr::gui::IGUIElement::draw();
}
