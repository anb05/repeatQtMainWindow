#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;

namespace spr_sht {
class FindDialog;
class Spreadsheet;
} // namespace spt_sht

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

private:
    void closeEvent(QCloseEvent* event) override;

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void find();
	void goToCell();
	void sort();
	void about();
	void openRecentFile();
	void updateStatusBar();
	void spreadsheetModified();

private:
	void    createActions();
	void    createMenus();
	void    createContextMenu();
	void    createToolBars();
	void    createStatusBar();
	void    readSettings();
	void    writeSettings();
	bool    okToContinue();
	bool    loadFile(const QString& fileName);
	bool    saveFile(const QString& fileName);
	void    setCurrentFile(const QString& fileName);
	void    updateRecentFileActions();
    QString strippedName(const QString& fullFileName);
	void    connections();

private:
    Spreadsheet* _pSpreadsheet        {nullptr};
    FindDialog*  _pFindDialog         {nullptr};
    QLabel*      _pLocationLbl        {nullptr};
    QLabel*      _pFormulaLbl         {nullptr};
    QStringList  _recentFiles         {};
    QString      _curFile             {};

	QMenu*       _pFileMenu           {nullptr};
	QMenu*       _pEditMenu           {nullptr};
	QMenu*       _pSelectSubmenu      {nullptr};
	QMenu*       _pToolsMenu          {nullptr};
	QMenu*       _pOptionsMenu        {nullptr};
	QMenu*       _pHelpMenu           {nullptr};
	QToolBar*    _pFileToolBar        {nullptr};
	QToolBar*    _pEditToolBar        {nullptr};
	enum {MAX_RECENT_FILES = 5};
	QAction*     _aRecentFileActions[MAX_RECENT_FILES]; //
    QAction*     _pSeparatorAction    {nullptr};
	QAction*     _pNewAction          {nullptr}; //
	QAction*     _pOpenAction         {nullptr}; //
	QAction*     _pSaveAction         {nullptr}; //
	QAction*     _pSaveAsAction       {nullptr}; //
	QAction*     _pExitAction         {nullptr}; //
	QAction*     _pCutAction          {nullptr}; //
	QAction*     _pCopyAction         {nullptr}; //
	QAction*     _pPasteAction        {nullptr}; //
	QAction*     _pDeleteAction       {nullptr}; //
	QAction*     _pSelectRowAction    {nullptr}; //
	QAction*     _pSelectColumnAction {nullptr}; //
	QAction*     _pSelectAllAction    {nullptr}; //
	QAction*     _pFindAction         {nullptr}; //
	QAction*     _pGoToCellAction     {nullptr}; //
	QAction*     _pRecalculateAction  {nullptr}; //
	QAction*     _pSortAction         {nullptr}; //
	QAction*     _pShowGridAction     {nullptr}; //
	QAction*     _pAutoRecalcAction   {nullptr}; //
	QAction*     _pAboutAction        {nullptr}; //
	QAction*     _pAboutQtAction      {nullptr}; //
};


} // namespace spr_sht

#endif // MAINWINDOW_H
