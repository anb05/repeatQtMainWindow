#ifndef SORT_DIALOG_H
#define SORT_DIALOG_H

#include <QDialog>
#include <QChar>

#include "ui_SortDialog.h"

namespace Ui {
class SortDialog;
} // namespace Ui

namespace spr_sht {

    class SortDialog : public QDialog, public Ui::SortDialog
    {
        Q_OBJECT

        public:
            explicit SortDialog(QWidget* parent = nullptr);
            explicit SortDialog(const SortDialog&) = delete;
            explicit SortDialog(SortDialog&&)      = delete;

            SortDialog& operator= (const SortDialog&) = delete;
            SortDialog& operator= (SortDialog&&)      = delete;

            ~SortDialog() override = default;

        public:
            void setColumnRange(QChar first, QChar last);

        public slots:
            void on_changedAdvanced(bool state);

        private:
            void connections();
    };

} // namespace spr_sht

#endif // SORT_DIALOG_H

