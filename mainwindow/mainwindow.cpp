#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QSettings>
#include <QFileInfo>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeySequence>

#include "./mainwindow.h"
#include "../app/application.h"
#include "../sort_dialog/sortdialog.h"
#include "../find_dialog/finddialog.hpp"
#include "../spreadsheet/spreadsheet.hpp"
#include "../go_to_cell_dialog/gotocelldialog.h"

namespace spr_sht {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _pSpreadsheet {new Spreadsheet}
{
    setCentralWidget(_pSpreadsheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    setWindowIcon(QIcon(":/images/icon"));
    setCurrentFile("");

    connections();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    // для работы с одним окном
    /*
    if (okToContinue()) {
        _pSpreadsheet->clear();
        setCurrentFile("");
    }
    */

    // для работы с несколькими окнами (добавить возможность удаления)
    MainWindow* mainWin = new MainWindow;
    mainWin->show();
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    tr("Open Spreadsheet"),
                    ".",
                    tr("Spreadsheet files (*.sp)")
                    );

        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::save()
{
    if (_curFile.isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile(_curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Spreadsheet"),
                ".",
                tr("Spreadsheet files (*.sp)")
                );

    if (fileName.isEmpty()) {
        return false;
    }

    return saveFile(fileName);
}

void MainWindow::find()
{
    if (!_pFindDialog) {
        _pFindDialog = new FindDialog(this);
    }
    _pFindDialog->show();
    _pFindDialog->raise();
    _pFindDialog->activateWindow();
}

void MainWindow::goToCell()
{
    GoToCellDialog dialog(this);
    if (dialog.exec()) {
        QString str = dialog._edt->text().toUpper();
        _pSpreadsheet->setCurrentCell(str.mid(1).toInt() - 1,
                                      str[0].unicode() - 'A');
    }
}

void MainWindow::sort()
{
    SortDialog dialog(this);
    QTableWidgetSelectionRange range = _pSpreadsheet->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(),
                          'A' + range.rightColumn());

    if (dialog.exec()) {
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spreadsheet"),
                       tr("<h2>Spreadsheet 1.1</h2>"
                          "<p>Copyright &copy; 2008 Software Inc."
                          "<p>Spreadsheet is a small application that "
                          "demonstrates QAction, QMainWindow, QMenuBar, "
                          "QStatusBar, QTableWidget, QToolBar, and many other "
                          "Qt classes."));
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction* act = qobject_cast<QAction*>(sender());
        if (act != nullptr) {
            loadFile(act->data().toString());
        }
    }
}

void MainWindow::updateStatusBar()
{
    _pLocationLbl->setText(_pSpreadsheet->currentLocation());
    _pFormulaLbl->setText(_pSpreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::createActions()
{
    QAction* pAction {nullptr};
    {
        pAction = _pNewAction = new QAction(this);
        pAction->setText(tr("&New"));
        pAction->setIcon(QIcon(":/images/new"));
        pAction->setShortcut(QKeySequence::New);
        pAction->setStatusTip(tr("Create a new spreadsheet file"));
    }
    {
        pAction = _pOpenAction = new QAction(this);
        pAction->setText(tr("&Open"));
        pAction->setIcon(QIcon(":/images/open"));
        pAction->setShortcut(QKeySequence::Open);
        pAction->setStatusTip(tr("Open an existing spreadsheet file"));
    }
    {
        pAction = _pSaveAction = new QAction(this);
        pAction->setText(tr("&Save"));
        pAction->setIcon(QIcon(":/images/save"));
        pAction->setShortcut(QKeySequence::Save);
        pAction->setStatusTip(tr("Save the spreadsheet to disk"));
    }
    {
        pAction = _pSaveAsAction = new QAction(this);
        pAction->setText(tr("Save &As ..."));
        pAction->setStatusTip(tr("Save the spreadsheet under a new name"));
    }
    {
        for (int cnt = 0; cnt < MAX_RECENT_FILES ; ++cnt) {
            pAction = new QAction(this);
            pAction->setVisible(false);
            _aRecentFileActions[cnt] = pAction;

            connect(pAction, &QAction::triggered,
                    this,    &MainWindow::openRecentFile);
        }
    }
    {
        pAction = _pExitAction = new QAction(this);
        pAction->setText(tr("E&xit"));
        pAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
        pAction->setStatusTip(tr("Exit the application"));
    }
    {
        pAction = _pSelectAllAction = new QAction(this);
        pAction->setText(tr("&All"));
        pAction->setShortcut(QKeySequence::SelectAll);
        pAction->setStatusTip(tr("Select all the cells in the spreadsheet"));
    }
    {
        pAction = _pShowGridAction = new QAction(this);
        pAction->setText(tr("&Show Grid"));
        pAction->setCheckable(_pSpreadsheet->showGrid());
        pAction->setStatusTip(tr("Show or hide the spreadsheet's grid"));
    }
    {
        pAction = _pAutoRecalcAction = new QAction(this);
        pAction->setText(tr("&Auto-Recalculate"));
        pAction->setCheckable(true);
        pAction->setChecked(_pSpreadsheet->autoRecalculate());
        pAction->setStatusTip(tr("Switch auto-recalculation on or off"));
    }
    {
        pAction = _pCutAction = new QAction(this);
        pAction->setText(tr("Cu&t"));
        pAction->setIcon(QIcon(":/images/cut"));
        pAction->setShortcut(QKeySequence::Cut);
        pAction->setStatusTip(tr("Cut the current selection's contents "
                                 "to the clipboard"));
    }
    {
        pAction = _pCopyAction = new QAction(this);
        pAction->setText(tr("&Copy"));
        pAction->setIcon(QIcon(":/images/copy"));
        pAction->setShortcut(QKeySequence::Copy);
        pAction->setStatusTip(tr("Copy the current selection's contents "
                                 "to the clipboard"));
    }
    {
        pAction = _pPasteAction = new QAction(this);
        pAction->setText(tr("&Paste"));
        pAction->setIcon(QIcon(":/images/paste"));
        pAction->setShortcut(QKeySequence::Paste);
        pAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));
    }
    {
        pAction = _pDeleteAction = new QAction(this);
        pAction->setText(tr("&Delete"));
        pAction->setShortcut(QKeySequence::Delete);
        pAction->setStatusTip(tr("Delete the current selection's contents"));
    }
    {
        pAction = _pSelectRowAction = new QAction(this);
        pAction->setText(tr("&Row"));
        pAction->setStatusTip(tr("Select all the cells in the "
                                 "current row"));
    }
    {
        pAction = _pSelectColumnAction = new QAction(this);
        pAction->setText(tr("&Column"));
        pAction->setStatusTip(tr("Select all the cells in the "
                                 "current column"));
    }
    {
        pAction = _pFindAction = new QAction(this);
        pAction->setText(tr("&Find..."));
        pAction->setIcon(QIcon(":/images/find"));
        pAction->setShortcut(QKeySequence::Find);
        pAction->setStatusTip(tr("Find a matching cell"));
    }
    {
        pAction = _pGoToCellAction = new QAction(this);
        pAction->setText(tr("&Go to Cell..."));
        pAction->setIcon(QIcon(":/images/gotocell"));
        pAction->setShortcut(QKeySequence(tr("Ctrl+G")));
        pAction->setStatusTip(tr("Go to the specified cell"));
    }
    {
        pAction = _pRecalculateAction = new QAction(this);
        pAction->setText(tr("&Recalculate"));
        pAction->setShortcut(QKeySequence(tr("F9")));
        pAction->setStatusTip(tr("Recalculate all the "
                                 "spreadsheet's formulas"));
    }
    {
        pAction = _pSortAction = new QAction(this);
        pAction->setText(tr("&Sort..."));
        pAction->setStatusTip(tr("Sort the selected cells or all the cells"));
    }
    {
        pAction = _pAboutQtAction = new QAction(this);
        pAction->setText(tr("About &Qt"));
        pAction->setStatusTip(tr("Show the Qt libraty's About box"));
    }
    {
        pAction = _pAboutAction = new QAction(this);
        pAction->setText(tr("&About"));
        pAction->setStatusTip(tr("Show the application's About box"));
    }
}

void MainWindow::createMenus()
{
    QMenu* pMenu {nullptr};
    {
        _pFileMenu = pMenu = menuBar()->addMenu(tr("&File"));
        pMenu->addAction(_pNewAction);
        pMenu->addAction(_pOpenAction);
        pMenu->addAction(_pSaveAction);
        pMenu->addAction(_pSaveAsAction);
        _pSeparatorAction = pMenu->addSeparator();
        for (int cnt = 0; cnt < MAX_RECENT_FILES; ++cnt) {
            pMenu->addAction(_aRecentFileActions[cnt]);
        }
        pMenu->addSeparator();
        pMenu->addAction(_pExitAction);
    }
    {
        _pFileMenu = pMenu = menuBar()->addMenu(tr("&Edit"));
        pMenu->addAction(_pCutAction);
        pMenu->addAction(_pCopyAction);
        pMenu->addAction(_pPasteAction);
        pMenu->addAction(_pDeleteAction);
        {
            _pSelectSubmenu = pMenu->addMenu(tr("&Select"));
            _pSelectSubmenu->addAction(_pSelectRowAction);
            _pSelectSubmenu->addAction(_pSelectColumnAction);
            _pSelectSubmenu->addAction(_pSelectAllAction);
        }
        pMenu->addSeparator();
        pMenu->addAction(_pFindAction);
        pMenu->addAction(_pGoToCellAction);

    }
    {
        _pToolsMenu = pMenu = menuBar()->addMenu(tr("&Tools"));
        pMenu->addAction(_pRecalculateAction);
        pMenu->addAction(_pSortAction);
    }
    {
        _pOptionsMenu = pMenu = menuBar()->addMenu(tr("&Options"));
        pMenu->addAction(_pShowGridAction);
        pMenu->addAction(_pAutoRecalcAction);
    }
    menuBar()->addSeparator();
    {
        _pHelpMenu = pMenu = menuBar()->addMenu(tr("&Help"));
        pMenu->addAction(_pAboutAction);
        pMenu->addAction(_pAboutQtAction);
    }
}

void MainWindow::createContextMenu()
{
    _pSpreadsheet->addAction(_pCutAction);
    _pSpreadsheet->addAction(_pCopyAction);
    _pSpreadsheet->addAction(_pPasteAction);

    _pSpreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    QToolBar* pToolBar {nullptr};
    {
        _pFileToolBar = pToolBar = addToolBar(tr("&File"));
        pToolBar->addAction(_pNewAction);
        pToolBar->addAction(_pOpenAction);
        pToolBar->addAction(_pSaveAction);
    }
    {
        _pEditToolBar = pToolBar = addToolBar(tr("&Edit"));
        pToolBar->addAction(_pCutAction);
        pToolBar->addAction(_pCopyAction);
        pToolBar->addAction(_pPasteAction);
        pToolBar->addSeparator();
        pToolBar->addAction(_pFindAction);
        pToolBar->addAction(_pGoToCellAction);
    }
}

void MainWindow::createStatusBar()
{
    QLabel* pLbl {nullptr};
    {
        _pLocationLbl = pLbl = new QLabel(this);
        pLbl->setText(" W999 ");
        pLbl->setAlignment(Qt::AlignCenter);
        pLbl->setMinimumSize(pLbl->sizeHint());
        statusBar()->addWidget(pLbl);
    }
    {
        _pFormulaLbl = pLbl = new QLabel(this);
        pLbl->setIndent(3);
        pLbl->setAlignment(Qt::AlignLeft);
        statusBar()->addWidget(pLbl, 1);
    }
    updateStatusBar();
}

void MainWindow::readSettings()
{
    QSettings settings("Home Studio", "Spreadsheet");

    restoreGeometry(settings.value("geometry").toByteArray());

    _recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    _pShowGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    _pAutoRecalcAction->setChecked(autoRecalc);
}

void MainWindow::writeSettings()
{
    QSettings settings("Home Studio", "Spreadsheet");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", _recentFiles);
    settings.setValue("showGrid", _pShowGridAction->isChecked());
    settings.setValue("autoRecalc", _pAutoRecalcAction->isChecked());
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(
                    this,
                    tr("Spreadsheet"),
                    tr("The document has been modified.\n"
                       "Do you want to save your changes?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                    );
        if (QMessageBox::Yes == r) {
            return save();
        }
        else if (QMessageBox::Cancel == r) {
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    if (!_pSpreadsheet->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!_pSpreadsheet->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"));
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    _curFile = fileName;
    setWindowModified(false);

    QString shownName {"Untitled"};
    if (!_curFile.isEmpty()) {
        shownName = strippedName(_curFile);
        _recentFiles.removeAll(_curFile);
        _recentFiles.prepend(_curFile);
        updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*]-%2").arg(shownName).arg(tr("Spreadsheet")));
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(_recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next())) {
            i.remove();
        }
    }
    for (int j = 0; j < MAX_RECENT_FILES; ++j) {
        if (j < _recentFiles.count()) {
            QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(_recentFiles[j]));
            _aRecentFileActions[j]->setText(text);
            _aRecentFileActions[j]->setData(_recentFiles[j]);
            _aRecentFileActions[j]->setVisible(true);
        }
        else {
            _aRecentFileActions[j]->setVisible(false);
        }
    }
    _pSeparatorAction->setVisible(!_recentFiles.isEmpty());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::connections()
{
    connect(_pNewAction, &QAction::triggered,
            this,        &MainWindow::newFile);

    connect(_pOpenAction, &QAction::triggered,
            this,         &MainWindow::open);

    connect(_pSaveAction, &QAction::triggered,
            this,         &MainWindow::save);

    connect(_pSaveAsAction, &QAction::triggered,
            this,           &MainWindow::saveAs);

    connect(_pExitAction, &QAction::triggered,
            this,         &MainWindow::close);

    connect(_pSelectAllAction, &QAction::triggered,
            _pSpreadsheet,     &Spreadsheet::selectAll);

    connect(_pShowGridAction, &QAction::toggled,
            _pSpreadsheet,    &Spreadsheet::setShowGrid);

    connect(_pAutoRecalcAction, &QAction::toggled,
            _pSpreadsheet,      &Spreadsheet::setAutoRecalculate);

    connect(_pCutAction,   &QAction::triggered,
            _pSpreadsheet, &Spreadsheet::cut);

    connect(_pCopyAction,  &QAction::triggered,
            _pSpreadsheet, &Spreadsheet::copy);

    connect(_pPasteAction, &QAction::triggered,
            _pSpreadsheet, &Spreadsheet::paste);

    connect(_pDeleteAction, &QAction::triggered,
            _pSpreadsheet, &Spreadsheet::del);

    connect(_pSelectRowAction, &QAction::triggered,
            _pSpreadsheet,     &Spreadsheet::selectCurrentRow);

    connect(_pSelectRowAction, &QAction::triggered,
            _pSpreadsheet,     &Spreadsheet::selectCurrentColumn);

    connect(_pFindAction, &QAction::triggered,
            this,         &MainWindow::find);

    connect(_pGoToCellAction, &QAction::triggered,
            this,             &MainWindow::goToCell);

    connect(_pSelectRowAction, &QAction::triggered,
            _pSpreadsheet,     &Spreadsheet::recalculate);

    connect(_pSortAction, &QAction::triggered,
            this,         &MainWindow::sort);

    connect(_pAboutQtAction, &QAction::triggered,
            qApp,            &QApplication::aboutQt);

    connect(_pAboutAction, &QAction::triggered,
            this,          &MainWindow::about);

    connect(_pSpreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this,          SLOT(updateStatusBar()));

    connect(_pSpreadsheet, SIGNAL(modified()),
            this,          SLOT(preadsheetModified()));

    connect(_pFindDialog,  &FindDialog::findNext,
            _pSpreadsheet, &Spreadsheet::findNext);

    connect(_pFindDialog,  &FindDialog::findNext,
            _pSpreadsheet, &Spreadsheet::findPrevious);
}

} // namespace spr_sht
