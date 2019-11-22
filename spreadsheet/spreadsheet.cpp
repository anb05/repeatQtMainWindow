#include "spreadsheet.hpp"
#include "./cell.h"

#include <QString>

namespace spr_sht {

Spreadsheet::Spreadsheet(QWidget *parent)
	: QTableWidget(parent)
{}

Cell* Spreadsheet::cell(int row, int column) const
{
	Q_UNUSED(row)
	Q_UNUSED(column)

	return nullptr;
}

QString Spreadsheet::text(int row, int column) const
{
	QString str("%1, %2").arg(row).arg(column);

	return str;

}

Spreadsheet::~Spreadsheet() = default;

} // namespace spr_sht
