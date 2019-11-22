#ifndef SPREADSHEET_HPP
#define SPREADSHEET_HPP

#include <QString>
#include <QTableWidget>

namespace spr_sht {
class Cell;
class SpreadsheetCompare;
} // namespace spr_sht

namespace spr_sht {

class Spreadsheet : public QTableWidget
{
	Q_OBJECT

public:
    explicit Spreadsheet(QWidget* parent = nullptr);
    ~Spreadsheet() override;

    explicit Spreadsheet(const Spreadsheet& )    = delete ;
    explicit Spreadsheet(Spreadsheet&& )         = delete ;

    Spreadsheet& operator= (const Spreadsheet& ) = delete ;
    Spreadsheet& operator= (Spreadsheet&& )      = delete ;

public:
    const QString& currentLocation() {return _currentLocation;}
    const QString& currentFormula()  {return _currentFormula;}
	bool  readFile(const QString& fileName) {Q_UNUSED(fileName) return true;}
	bool  writeFile(const QString& fileName){Q_UNUSED(fileName) return true;}

    QTableWidgetSelectionRange selectedRange() {return QTableWidgetSelectionRange();}

public slots:
    bool autoRecalculate() {return true;}
    void setAutoRecalculate(bool b) {Q_UNUSED(b)}
    void cut(bool checked) {Q_UNUSED(checked)}
    void copy(bool checked) {Q_UNUSED(checked)}
    void paste(bool checked) {Q_UNUSED(checked)}
    void del(bool checked) {Q_UNUSED(checked)}
    void selectCurrentRow(bool checked) {Q_UNUSED(checked)}
    void selectCurrentColumn(bool checked) {Q_UNUSED(checked)}
    void recalculate(bool checked) {Q_UNUSED(checked)}
	void findNext(const QString& str, Qt::CaseSensitivity cs) {Q_UNUSED(str) Q_UNUSED(cs)}
	void findPrevious(const QString& str, Qt::CaseSensitivity cs)  {Q_UNUSED(str) Q_UNUSED(cs)}
	void performSort(bool isMore) {Q_UNUSED(isMore)} // Rebuild in the period ahead

signals:
    void currentCellChanged(int, int, int, int);
	void modified();

private:
	enum {MAGIC_NUMBER = 0x7f51c883, ROW_COUNT = 999, COLUMN_COUNT = 26};
	Cell*   cell(int row, int column) const;
	QString text(int row, int column) const;
	QString formula(int row, int column) const;
	void    setFormula(int row, int column, const QString& formula);

private:
	bool _autoRecalc;

    QString _currentLocation {};
    QString _currentFormula {};
};

} // namespace spr_sht

#endif // SPREADSHEET_HPP
