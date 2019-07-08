#include "FontGlyphInfoWidget.hpp"
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
// #include <FontAwesome.hpp>

QPushButton* createButton( QString text, uint16_t value, QWidget* parent )
{
   // max = 0xf300, 3*2^8 = 3*256
   // min = 0xf000,
   QPushButton* btn = new QPushButton( text, parent );
   btn->setToolTip( QString("Unicode char: ") + QString::number( value, 16 ) );
   btn->setMinimumSize( 42,42 );
   btn->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
   QFont font = btn->font();
   font.setPixelSize( 32 );
   btn->setFont( font );
   return btn;
}

FontGlyphInfoWidget::FontGlyphInfoWidget( QWidget* parent )
   : QWidget( parent )
{
   setAcceptDrops( true );
   setContentsMargins( 5, 5, 5, 5 );

   QHBoxLayout* h = new QHBoxLayout;
   h->setContentsMargins( 0,0,0,0 );

   for ( int k = 0; k < 3; ++k )
   {
      QGridLayout* grid = new QGridLayout;
      grid->setContentsMargins( 0,0,0,0 );
      grid->setSpacing( 4 );
      for ( int y = 0; y < 16; ++y )
      {
         for ( int x = 0; x < 16; ++x )
         {
            uint16_t const value = 0xf000 + ( k * 256 + ( y * 16 + x ) );
            grid->addWidget( createButton( QChar( value ), value, this ), y, x, 1, 1 );
         }
      }
      h->addLayout( grid );
   }

   setLayout( h );
}

FontGlyphInfoWidget::~FontGlyphInfoWidget()
{

}
