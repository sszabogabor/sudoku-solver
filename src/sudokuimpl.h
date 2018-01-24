#ifndef SUDOKUIMPL_H
#define SUDOKUIMPL_H
//
#include <QMainWindow>
#include <QVector>
#include "ui_sudoku.h"
//
class SudokuImpl : public QMainWindow, public Ui::Sudoku
{
Q_OBJECT
public:
	SudokuImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void openFile();
	void saveFile();
	void closeWindow();
	void writeToList(QVector<QString>);
	void solve();
	void writeToProgressDialog(QVector<QString>);
	void prBar();

private:
	Ui::Sudoku ui;
	
protected:
	void closeEvent(QCloseEvent *event);
};
#endif




