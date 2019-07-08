# // ============================================================================
# QPushButton* WIDGET_createSymbolButton( const QChar symbol, int size )
# {
#     QPushButton* button = new QPushButton( symbol );
#     QFont font("FontAwesome", 20, QFont::Normal, false );
#     font.setHintingPreference( QFont::PreferFullHinting );
#     font.setKerning( true );
#     font.setStyleStrategy( QFont::PreferAntialias );
#     button->setFont( font );
#     return button;
# }

# #include <FontAwesome.hpp>

# QFontDatabase::addApplicationFont( ":/font/awesome" );  

INCLUDEPATH += $$PWD
HEADERS += $$PWD/FontAwesome.hpp
RESOURCES += $$PWD/FontAwesome.qrc

