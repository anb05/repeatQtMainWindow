#include "./sortdialog.h"

namespace spr_sht {

    SortDialog::SortDialog(QWidget* parent)
        : QDialog(parent)
    {
        setupUi(this);
        _secondaryGrBox->hide();
        _tertiaryGrBox->hide();
        layout()->setSizeConstraint(QLayout::SetFixedSize);

        setColumnRange('A', 'Z');

        // Изменения для кнопки
        _moreBtn->setText(tr("Advanced >>>"));
        connections();
    }

    void SortDialog::setColumnRange(QChar first, QChar last)
    {
        _primaryColCbx->clear();
        _secondaryColCbx->clear();
        _tertiaryColCbx->clear();

        _secondaryColCbx->addItem(tr("None"));
        _tertiaryColCbx->addItem(tr("None"));

        _primaryColCbx->setMinimumSize(_secondaryColCbx->sizeHint());

        QChar ch = first;

        while (ch <= last) {
            _primaryColCbx->addItem(ch);
            _secondaryColCbx->addItem(ch);
            _tertiaryColCbx->addItem(ch);
            ++(ch.unicode());
        }
    }

    void SortDialog::on_changedAdvanced(bool state)
    {
        if (state) {
            _moreBtn->setText(tr("Advanced <<<"));
        } else {
            _moreBtn->setText(tr("Advanced >>>"));
        }
    }

    void SortDialog::connections()
    {
        connect(_moreBtn, &QPushButton::toggled,
                this,     &SortDialog::on_changedAdvanced);
    }

} // namespace spr_sht

