#include "spreadsheet.hpp"

namespace spr_sht {

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{}

Spreadsheet::~Spreadsheet() = default;

} // namespace spr_sht
