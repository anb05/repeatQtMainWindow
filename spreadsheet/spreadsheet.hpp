#ifndef SPREADSHEET_HPP
#define SPREADSHEET_HPP

#include <QTableWidget>

namespace spr_sht {

class Spreadsheet : public QTableWidget
{
public:
    explicit Spreadsheet(QWidget* parent = nullptr);
    ~Spreadsheet() override;

    explicit Spreadsheet(const Spreadsheet& )    = delete ;
    explicit Spreadsheet(Spreadsheet&& )         = delete ;

    Spreadsheet& operator= (const Spreadsheet& ) = delete ;
    Spreadsheet& operator= (Spreadsheet&& )      = delete ;
};

} // namespace spr_sht

#endif // SPREADSHEET_HPP
