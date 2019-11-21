#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>

#include "ui_gotocelldialog.h"

namespace spr_sht {

    class GoToCellDialog : public QDialog, public Ui::GoToCellDialog
    {
        Q_OBJECT

        public:
            explicit GoToCellDialog(QWidget* parent = nullptr);
            ~GoToCellDialog() override = default;

            explicit GoToCellDialog(const GoToCellDialog&) = delete;
            explicit GoToCellDialog(GoToCellDialog&&)      = delete;

            GoToCellDialog& operator= (const GoToCellDialog&) = delete;
            GoToCellDialog& operator= (GoToCellDialog&&)      = delete;

        private slots:
            void onLineEdtTxtChanged();

        private:
        void connections();
    };

} // namespace spr_sht

#endif // GOTOCELLDIALOG_H

