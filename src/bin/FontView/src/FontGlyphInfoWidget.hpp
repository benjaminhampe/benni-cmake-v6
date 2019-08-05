#ifndef FONTGLYPHINFOWIDGET_HPP
#define FONTGLYPHINFOWIDGET_HPP

#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidget>

// ============================================================================
class FontGlyphInfoWidget : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   explicit FontGlyphInfoWidget( QWidget* parent = nullptr );
   virtual ~FontGlyphInfoWidget();
protected:

};

#endif // FONTGLYPHINFOWIDGET_HPP
