#ifndef SPREADSHEET_HPP
#define SPREADSHEET_HPP

#include <QString>
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

public:
    const QString& currentLocation() {return _currentLocation;}
    const QString& currentFormula()  {return _currentFormula;}
    bool  readFile(const QString& fileName) {return true;}
    bool  writeFile(const QString& fileName) {return true;}

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
    void findNext(const QString& str, Qt::CaseSensitivity cs) {}
    void findPrevious(const QString& str, Qt::CaseSensitivity cs) {}

signals:
    void currentCellChanged(int, int, int, int);
    void modified();

private:
    QString _currentLocation {};
    QString _currentFormula {};
};

} // namespace spr_sht

#endif // SPREADSHEET_HPP
