#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>
#include <QString>

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
    const QString& text()    const {return _text;}
    const QString& formula() const {return _formula;}
    void  setFormula(const QString& formula) {_formula = formula;}


private:
    QString _text    {};
    QString _formula {};

};

} // namespace spr_sht

#endif // CELL_H
