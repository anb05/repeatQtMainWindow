#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace spr_sht {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

	explicit MainWindow(const MainWindow&)    = delete;
	explicit MainWindow(MainWindow&&)         = delete;

	MainWindow& operator= (const MainWindow&) = delete;
	MainWindow& operator= (MainWindow&&)      = delete;
};

} // namespace spr_sht

#endif // MAINWINDOW_H
