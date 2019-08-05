
#include <Window.h>

// ============================================================================

int main(int argc, char** argv)

// ============================================================================

{   
    QApplication a(argc, argv);
    
    Window win( nullptr );
    win.show();
   
    return a.exec();
}
