#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

namespace spr_sht {

class Cell : public QTableWidgetItem
{
public:
	Cell();
	~Cell() override;

	explicit Cell(const Cell& )    = delete ;
	explicit Cell(Cell&& )         = delete ;

	Cell& operator =(const Cell& ) = delete ;
	Cell& operator =(Cell&& )      = delete ;

public:

private:
};

} // namespace spr_sht

#endif // CELL_H
