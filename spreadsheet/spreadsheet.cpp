#include "spreadsheet.hpp"
#include "./cell.h"

#include <QFile>
#include <QList>
#include <QString>
#include <QCursor>
#include <QClipboard>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>

#include <utility>

namespace spr_sht {

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{
    setAutoRecalculate(true);
    setItemPrototype(new Cell);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    connect(this, &QTableWidget::itemChanged,
            this, &Spreadsheet::somethingChanged);

    clear();
}

Spreadsheet::~Spreadsheet() = default;

QString Spreadsheet::currentLocation() const
{
    return QChar('A' + currentColumn()) +
           QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula() const
{
    return formula(currentRow(), currentColumn());
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
    QList<QTableWidgetSelectionRange> ranges {selectedRanges()};
    if (ranges.isEmpty()) {
        return QTableWidgetSelectionRange();
    }
    return ranges.first();
}

void Spreadsheet::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(ROW_COUNT);
    setColumnCount(COLUMN_COUNT);

    for (int i = 0; i < COLUMN_COUNT; ++i) {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
    }
    setCurrentCell(0, 0);
}

bool Spreadsheet::readFile(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             tr("Spreadsheet"),
                             tr("Cannot read file %1:\n%2")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);

    quint32 magic  {0};

    in >> magic;
    if (MAGIC_NUMBER != magic) {
        QMessageBox::warning(
                    this,
                    tr("Spreadsheet"),
                    tr("The file %1 is not a Spreadshit file. ")
                    .arg(file.fileName())
                    );
        return false;
    }

    clear();

    quint16 row    {0};
    quint16 column {0};
    QString str    {};

    QApplication::setOverrideCursor(Qt::WaitCursor);

    while (!in.atEnd()) {
        in >> row >> column >> str;
        setFormula(row, column, std::move(str));
    }
    QApplication::restoreOverrideCursor();

    return true;
}

bool Spreadsheet::writeFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("Spreadsheet"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint32(MAGIC_NUMBER);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int row = 0; row < ROW_COUNT; ++row) {
        for (int column = 0; column < COLUMN_COUNT; ++column) {
            QString str {formula(row, column)};

            if (!str.isEmpty()) {
                out << quint16(row) << quint16(column) << str;
            }
        }
    }
    QApplication::restoreOverrideCursor();

    return true;
}

void Spreadsheet::sort(const SpreadsheetCompare& compare)
{
    Q_UNUSED(compare)
}

void Spreadsheet::cut()
{
    copy();
    del();
}

void Spreadsheet::copy()
{
    QTableWidgetSelectionRange range = selectedRange();

    QString str;

    for (int i = 0; i < range.rowCount(); ++i) {
        if (i > 0) {
            str += '\n';
        }
        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0) {
                str += '\t';
            }
            str += formula(range.topRow() + i, range.leftColumn() + j);
        }
    }
    QApplication::clipboard()->setText(str);
}

void Spreadsheet::paste()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t') + 1;

    if (
        (range.rowCount() * range.columnCount() != 1) &&
        ((range.rowCount() != numRows) ||
         (range.columnCount() != numColumns))
        ) {
        QMessageBox::warning(
                    this,
                    tr("Spreadsheet"),
                    tr("The information cannot be pasted because the "
                       "copy and paste areas aren't the same size."));
        return;
    }

    for (int i = 0; i < numRows; ++i) {
        QStringList columns = rows[i].split('\t');

        for (int j = 0; j < numColumns ; ++j) {
            int row = range.topRow() + i;
            int col = range.leftColumn() + j;

            if ((row < ROW_COUNT)&&(col < COLUMN_COUNT)) {
                setFormula(row, col, columns[j]);
            }
        }
    }
    somethingChanged();
}

void Spreadsheet::del()
{
    QList<QTableWidgetItem*> items = selectedItems();

    if (items.isEmpty()) {
        return;
    }

    foreach(QTableWidgetItem* item, items) {
        delete item;
    }
    somethingChanged();
}

void Spreadsheet::selectCurrentRow()
{
    selectRow(currentRow());
}

void Spreadsheet::selectCurrentColumn()
{
    selectColumn(currentColumn());
}

void Spreadsheet::recalculate()
{
}

void Spreadsheet::setAutoRecalculate(bool recalc)
{
    _autoRecalc = recalc;
    if (_autoRecalc == true) {
        recalculate();
    }
}

void Spreadsheet::findNext(const QString& str, Qt::CaseSensitivity cs)
{
    for (int row = currentRow(); row < ROW_COUNT ; ++row) {
        for (int col = currentColumn() + 1; col < COLUMN_COUNT; ++col){
            QString txt {text(row, col)};
            if (txt.contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, col);
                activateWindow();
                return;
            }
        }
    }
    QApplication::beep();
}

void Spreadsheet::findPrevious(const QString& str, Qt::CaseSensitivity cs)
{
    Q_UNUSED(str) Q_UNUSED(cs)
}

void Spreadsheet::somethingChanged()
{
    if (true == _autoRecalc) {
        recalculate();
    }
    emit modified();
}

Cell* Spreadsheet::cell(int row, int column) const
{
    return static_cast<Cell*>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const
{
    Cell* c = cell(row, column);
    if (nullptr != c) {
        return c->text();
    }

    return "";

}

QString Spreadsheet::formula(int row, int column) const
{
    Cell* c {cell(row, column)};
    if (nullptr != c) {
        return c->formula();
    }
    return "";
}

void Spreadsheet::setFormula(int row,
                             int column,
                             const QString &formula)
{
    Cell* c = cell(row, column);

    if (nullptr == c) {
        c = new Cell;
        setItem(row, column, c);
    }
    c->setFormula(formula);
}

bool SpreadsheetCompare::operator()(const QStringList& row1,
                                    const QStringList& row2) const
{
    Q_UNUSED(row1)
    Q_UNUSED(row2)
    return true;
}

} // namespace spr_sht
