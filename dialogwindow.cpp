#include "dialogwindow.h"
#include "ui_dialogwindow.h"

DialogWindow::DialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWindow)
{
    ui->setupUi(this);

    _answer = false;
    QRegExp int_exp("[0-9]\\d*");
    QRegExpValidator *validator = new QRegExpValidator(int_exp, this);

    ui->lnBet->setValidator(validator);
    ui->btnOk->setEnabled(false);
    ui->lnName->setFocus();
}

DialogWindow::~DialogWindow()
{
    delete ui;
}

void DialogWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogWindow::on_btnOk_clicked()
{

    _name = ui->lnName->text();
    _bet = ui->lnBet->text();
    _answer = true;
    close();

}

void DialogWindow::on_lnName_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){
        ui->btnOk->setEnabled(false);
    }
    else{
        if (ui->lnBet->text()!="")
        ui->btnOk->setEnabled(true);
    }
}

void DialogWindow::on_lnBet_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){
        ui->btnOk->setEnabled(false);
    }
    else{
        if (ui->lnName->text()!="")
        ui->btnOk->setEnabled(true);
    }
}

void DialogWindow::on_btnCancel_clicked()
{
    _answer = false;
    close();
}

