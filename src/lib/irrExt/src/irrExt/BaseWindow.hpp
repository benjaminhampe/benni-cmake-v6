// Copyright (C) 2002-2018 Benjamin Hampe

#ifndef IRREXT_BASEWINDOW_HPP
#define IRREXT_BASEWINDOW_HPP

#include <irrExt/Types.hpp>

namespace irrExt {
	
class BaseWindow : public irr::gui::CGUIWindow
{
public:
    BaseWindow( irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id, irr::core::recti const & pos );

    virtual ~BaseWindow();

    bool OnEvent( irr::SEvent const & event ) override;
};

BaseWindow*
createBaseWindow( std::string const & title, irr::core::recti const & pos, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id );

} // end namespace irrExt


#endif // IRREXT_BASEWINDOW_HPP

