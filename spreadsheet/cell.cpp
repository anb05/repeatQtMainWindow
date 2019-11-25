#include "cell.h"

#include <QChar>
#include <QString>

namespace spr_sht {

Cell::Cell()
{
	setDirty();
}

Cell::Cell(const Cell& rhs)
	: QTableWidgetItem(rhs)
{
	_cachedValue = rhs._cachedValue;
	_cacheIsDirty = rhs._cacheIsDirty;
}

Cell& Cell::operator =(const Cell& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	QTableWidgetItem::operator=(rhs);
	_cachedValue = rhs._cachedValue;
	_cacheIsDirty = rhs._cacheIsDirty;
	return *this;
}

Cell::~Cell() = default;

QTableWidgetItem* Cell::clone() const
{
	return new Cell(*this);
}

void Cell::setData(int role, const QVariant& value)
{
	QTableWidgetItem::setData(role, value);
	if (Qt::EditRole == role) {
		setDirty();
	}
}

QVariant Cell::data(int role) const
{
	if (Qt::DisplayRole == role) {
		if (value().isValid()) {
			return value().toString();
		}
		else {
			return "####";
		}
	}
	else if (Qt::TextAlignmentRole == role) {
		if (value().type() == QVariant::String) {
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		}
		else {
			return int(Qt::AlignRight | Qt::AlignVCenter);
		}
	}
	return QTableWidgetItem::data(role);
}

void Cell::setFormula(const QString& formula)
{
	setData(Qt::EditRole, formula);
}

QString Cell::formula() const
{
	return data(Qt::EditRole).toString();
}

void Cell::setDirty()
{
	_cacheIsDirty = true;
}

static const QVariant INVALID;
QVariant Cell::value() const
{
	if (_cacheIsDirty) {
		_cacheIsDirty = false;
		QString formulaStr = formula();

		if (formula().startsWith('\'')) {
			_cachedValue = formulaStr.mid(1);
		}
		else if(formulaStr.startsWith('=')) {
			_cachedValue = INVALID;
			QString expr = formulaStr.mid(1);
			expr.replace(" ", "");
			expr.append(QChar::Null);

			int pos {0};
			_cachedValue = evalExpression(expr, pos);
			if (QChar::Null != expr[pos]) {
				_cachedValue = INVALID;
			}
		}
		else {
			bool ok;
			double d = formulaStr.toDouble(&ok);

			if (ok) {
				_cachedValue = d;
			}
			else {
				_cachedValue = formulaStr;
			}
		}
	}

	return _cachedValue;
}

QVariant Cell::evalExpression(const QString& str, int& pos) const
{
	QVariant result = evalTerm(str, pos);
	while (str[pos] != QChar::Null) {
		QChar op = str[pos];
		if (op != '+' && op != '-')
			return result;
		++pos;

		QVariant term = evalTerm(str, pos);
		if (result.type() == QVariant::Double
				&& term.type() == QVariant::Double) {
			if (op == '+') {
				result = result.toDouble() + term.toDouble();
			} else {
				result = result.toDouble() - term.toDouble();
			}
		} else {
			result = INVALID;
		}
	}
	return result;
}

QVariant Cell::evalTerm(const QString& str, int& pos) const
{
	QVariant result = evalFactor(str, pos);
	while (str[pos] != QChar::Null) {
		QChar op = str[pos];
		if (op != '*' && op != '/')
			return result;
		++pos;

		QVariant factor = evalFactor(str, pos);
		if (result.type() == QVariant::Double
				&& factor.type() == QVariant::Double) {
			if (op == '*') {
				result = result.toDouble() * factor.toDouble();
			} else {
				if (factor.toDouble() == 0.0) {
					result = INVALID;
				} else {
					result = result.toDouble() / factor.toDouble();
				}
			}
		} else {
			result = INVALID;
		}
	}
	return result;
}

QVariant Cell::evalFactor(const QString& str, int& pos) const
{
	QVariant result;
	bool negative = false;

	if (str[pos] == '-') {
		negative = true;
		++pos;
	}

	if (str[pos] == '(') {
		++pos;
		result = evalExpression(str, pos);
		if (str[pos] != ')')
			result = INVALID;
		++pos;
	} else {
		QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
		QString token;

		while (str[pos].isLetterOrNumber() || str[pos] == '.') {
			token += str[pos];
			++pos;
		}

		if (regExp.exactMatch(token)) {
			int column = token[0].toUpper().unicode() - 'A';
			int row = token.mid(1).toInt() - 1;

			Cell *c = static_cast<Cell *>(
							  tableWidget()->item(row, column));
			if (c) {
				result = c->value();
			} else {
				result = 0.0;
			}
		} else {
			bool ok;
			result = token.toDouble(&ok);
			if (!ok)
				result = INVALID;
		}
	}

	if (negative) {
		if (result.type() == QVariant::Double) {
			result = -result.toDouble();
		} else {
			result = INVALID;
		}
	}
	return result;
}

} // namespace spr_sht
