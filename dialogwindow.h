#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QDialog>

namespace Ui {
class DialogWindow;
}

class DialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWindow(QWidget *parent = 0);
    ~DialogWindow();

    bool _answer;
    QString _name;
    QString _bet;

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_lnName_textChanged(const QString &arg1);

    void on_lnBet_textChanged(const QString &arg1);

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DialogWindow *ui;
};

#endif // DIALOGWINDOW_H
