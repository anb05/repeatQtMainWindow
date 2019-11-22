#ifndef FIND_DIALOG_HPP
#define FIND_DIALOG_HPP

#include <QDialog>

class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;

namespace spr_sht {

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog() override;

    explicit FindDialog(const FindDialog&) = delete;
    explicit FindDialog(FindDialog&&)      = delete;

    FindDialog& operator= (const FindDialog&) = delete;
    FindDialog& operator= (FindDialog&&)      = delete;

signals:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrev(const QString& str, Qt::CaseSensitivity cs);

private:
    QLabel*      _pLblFindWhat  {nullptr};
    QLineEdit*   _pEdtFind      {nullptr};
    QCheckBox*   _pCbxMatchCase {nullptr};
    QCheckBox*   _pCbxBackward  {nullptr};
    QPushButton* _pBtnFind      {nullptr};
    QPushButton* _pBtnClose     {nullptr};

private:
    void connections();

private slots:
    void findClicked();
    void enableFindButton(const QString& text);
};

} // namespace spr_sht
#endif // FIND_DIALOG_HPP
