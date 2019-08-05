// ============================================================================
/// @author Benjamin Hampe (c) 2019 <benjaminhampe@gmx.de>
// ============================================================================

#include "MainWindow.hpp"

int main(int argc, char** argv)
{   
	QApplication a(argc, argv);

	MainWindow win( nullptr );
	win.show();

	return a.exec();
}
