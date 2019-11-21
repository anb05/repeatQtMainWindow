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
};

} // namespace spr_sht

#endif // SPREADSHEET_HPP
