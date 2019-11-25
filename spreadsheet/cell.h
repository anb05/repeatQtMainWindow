#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

namespace spr_sht {

class Cell : public QTableWidgetItem
{
public:
	Cell();
	~Cell() override;

	explicit Cell(const Cell& rhs);
	explicit Cell(Cell&& rhs) = delete;

	Cell& operator =(const Cell& rhs);
	Cell& operator =(Cell&& rhs) = delete;

public:
	QTableWidgetItem* clone() const override;
	void setData(int role, const QVariant& value) override;
	QVariant data(int role) const override;
	void setFormula(const QString& formula);
	QString formula() const;
	void setDirty();

private:
	QVariant value() const;
	QVariant evalExpression(const QString& str, int& pos) const;
	QVariant evalTerm(const QString& str, int& pos) const;
	QVariant evalFactor(const QString& str, int& pos) const;

private:
	mutable QVariant _cachedValue {};
	mutable bool     _cacheIsDirty  ;
};

} // namespace spr_sht

#endif // CELL_H
