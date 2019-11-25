#include "finddialog.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QBoxLayout>

namespace spr_sht {

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    {
        _pEdtFind = new QLineEdit(this);
    }
    {
        _pLblFindWhat = new QLabel(this);
        _pLblFindWhat->setText(tr("Find &what:"));
        _pLblFindWhat->setBuddy(_pEdtFind);
    }
    {
        _pCbxMatchCase = new QCheckBox(this);
        _pCbxMatchCase->setText(tr("Match &case"));
        _pCbxMatchCase->setTristate(false);
        _pCbxMatchCase->setChecked(true);
    }
    {
        _pCbxBackward = new QCheckBox(this);
        _pCbxBackward->setText(tr("Seach Backward"));
        _pCbxBackward->setTristate(false);
        _pCbxBackward->setChecked(false);
    }
    {
        _pBtnFind = new QPushButton(this);
        _pBtnFind->setText(tr("Find"));
        _pBtnFind->setDefault(true);
        _pBtnFind->setEnabled(false);
    }
    {
        _pBtnClose = new QPushButton(this);
        _pBtnClose->setText(tr("Close"));
    }

    {
        QBoxLayout* pHTopLeft = new QBoxLayout(QBoxLayout::LeftToRight);
        pHTopLeft->addWidget(_pLblFindWhat);
        pHTopLeft->addWidget(_pEdtFind);

        QBoxLayout* pVLeft = new QBoxLayout(QBoxLayout::TopToBottom);
        pVLeft->addLayout(pHTopLeft);
        pVLeft->addWidget(_pCbxMatchCase);
        pVLeft->addWidget(_pCbxBackward);
        pVLeft->addStretch(1);

        QBoxLayout* pVRight = new QBoxLayout(QBoxLayout::TopToBottom);
        pVRight->addWidget(_pBtnFind);
        pVRight->addWidget(_pBtnClose);
        pVRight->addStretch(1);

        QBoxLayout* pHMain = new QBoxLayout(QBoxLayout::LeftToRight);
        pHMain->addLayout(pVLeft);
        pHMain->addLayout(pVRight);

        setLayout(pHMain);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
    }

    connections();
}

FindDialog::~FindDialog() = default;

void FindDialog::connections()
{
    connect(_pEdtFind, &QLineEdit::textChanged,
            this,      &FindDialog::enableFindButton);

    connect(_pBtnFind, &QPushButton::clicked,
            this,      &FindDialog::findClicked);

    connect(_pBtnClose, &QPushButton::clicked,
            this,       &QWidget::close);
}

void FindDialog::findClicked()
{
    Qt::CaseSensitivity cs = _pCbxMatchCase->isChecked() ?
                Qt::CaseSensitive : Qt::CaseInsensitive;

    if (_pCbxBackward->isChecked()) {
        emit findPrev(_pEdtFind->text(), cs);
    }
    else {
        emit findNext(_pEdtFind->text(), cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    _pBtnFind->setEnabled(!text.isEmpty());
}

} // namespace spt_sht
