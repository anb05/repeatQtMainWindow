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
    bool autoRecalculate() {return _autoRecalc;}
    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;
    void clear();
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void sort(const SpreadsheetCompare& compare);

public slots:
//    void cut();
    void cut(bool checked);
//    void copy();
    void copy(bool checked);
//    void paste();
    void paste(bool checked);
//    void del();
    void del(bool checked);
//    void selctCurrentRow();
    void selectCurrentRow(bool checked);
//    void selctCurrentColumn();
    void selectCurrentColumn(bool checked);
//    void recalculate();
    void recalculate(bool checked);
    void setAutoRecalculate(bool recalc);
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);

    /**/
    void performSort(bool isMore) {Q_UNUSED(isMore)} // Rebuild in the period ahead

signals:
    void modified();

public slots:
    void somethingChanged();

private:
    enum {MAGIC_NUMBER = 0x7f51c883, ROW_COUNT = 999, COLUMN_COUNT = 26};
    Cell*   cell(int row, int column) const;
    QString text(int row, int column) const;
    QString formula(int row, int column) const;
    void    setFormula(int row, int column, const QString& formula);

private:
    bool _autoRecalc;
};

class SpreadsheetCompare
{
public:
    bool operator()(const QStringList &row1,
                    const QStringList &row2) const;

    enum { KeyCount = 3 };
    int keys[KeyCount];
    bool ascending[KeyCount];
};

} // namespace spr_sht

#endif // SPREADSHEET_HPP
