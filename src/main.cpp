#include <QApplication>
#include "sudokuimpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	SudokuImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
