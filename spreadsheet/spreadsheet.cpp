#include "spreadsheet.hpp"
#include "./cell.h"

#include <QString>

namespace spr_sht {

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{
    setAutoRecalculate(true);

    setItemPrototype(new Cell);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    connect(this, &Spreadsheet::itemChanged,
            this, &Spreadsheet::somethingChanged);

    clear();
}

Spreadsheet::~Spreadsheet() = default;

QString Spreadsheet::currentLocation() const
{
    return "";
}

QString Spreadsheet::currentFormula() const
{
    return "";
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
    return QTableWidgetSelectionRange();
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
    Q_UNUSED(fileName)
    return true;
}

bool Spreadsheet::writeFile(const QString& fileName)
{
    Q_UNUSED(fileName)
    return true;
}

void Spreadsheet::sort(const SpreadsheetCompare& compare)
{
    Q_UNUSED(compare)
}

void Spreadsheet::cut(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::copy(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::paste(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::del(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::selectCurrentRow(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::selectCurrentColumn(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::recalculate(bool checked)
{
    Q_UNUSED(checked)
}

void Spreadsheet::setAutoRecalculate(bool recalc)
{
    Q_UNUSED(recalc)
}

void Spreadsheet::findNext(const QString& str, Qt::CaseSensitivity cs)
{
    Q_UNUSED(str) Q_UNUSED(cs)
}

void Spreadsheet::findPrevious(const QString& str, Qt::CaseSensitivity cs)
{
    Q_UNUSED(str) Q_UNUSED(cs)
}

void Spreadsheet::somethingChanged()
{}

Cell* Spreadsheet::cell(int row, int column) const
{
    Q_UNUSED(row)
    Q_UNUSED(column)

    return nullptr;
}

QString Spreadsheet::text(int row, int column) const
{
    QString str = QString("%1, %2").arg(row).arg(column);

    return str;

}

QString Spreadsheet::formula(int row, int column) const
{
    QString str = QString("%1, %2").arg(row).arg(column);

    return str;
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(formula)
}

bool SpreadsheetCompare::operator()(const QStringList& row1,
                                    const QStringList& row2) const
{
    Q_UNUSED(row1)
    Q_UNUSED(row2)
    return true;
}

} // namespace spr_sht
