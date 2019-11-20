#include "./gotocelldialog.h"

#include <QRegExp>
#include <QRegExpValidator>

namespace ui_dlg {
    
    GoToCellDialog::GoToCellDialog(QWidget* parent)
        : QDialog(parent)
    {
        setupUi(this);

        QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
        _edt->setValidator(new QRegExpValidator(regExp, this));

        layout()->setSizeConstraint(QLayout::SetFixedSize);


        connections();
    }

    void GoToCellDialog::onLineEdtTxtChanged()
    {
        _okBtn->setEnabled(_edt->hasAcceptableInput());
    }

    void GoToCellDialog::connections()
    {
        connect(_okBtn, &QPushButton::clicked,
                this,   &QDialog::accept);

        connect(_cancelBtn, &QPushButton::clicked,
                this,       &QDialog::reject);

        connect(_edt, &QLineEdit::textChanged,
                this, &GoToCellDialog::onLineEdtTxtChanged);
    }

} // namespace ui_dlg

