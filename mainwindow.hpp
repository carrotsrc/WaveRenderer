#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void keyPressEvent(QKeyEvent *e);
public slots:
	void cursorForward();
private:
	Ui::MainWindow *ui;
	QTimer* mTimer;
};

#endif // MAINWINDOW_HPP
