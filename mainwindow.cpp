#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   _map = new HashMap<QString,int>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void MainWindow::on_btnAdd_clicked()
{
  DialogWindow *ad = new DialogWindow(this);
    ad->exec();
    if (ad->_answer){
        if (!_map->count())
            _map = new HashMap<QString,int>();
        _map->add(ad->_name, (ad->_bet).toInt());
        tblRefresh();
}
}

    void MainWindow::tblRefresh()
    {
        ui->tblBets->clear();
        ui->tblBets->setRowCount(0);
        ui->tblBets->setColumnCount(0);


        if (!_map->count()){
            ui->tblBets->clear();
            ui->btnDeleteAll->setEnabled(false);
            ui->btnDeleteSelected->setEnabled(false);
            ui->btnChoose->setEnabled(false);
            ui->btnToFile->setEnabled(false);
            return;
        }

        ui->tblBets->setColumnCount(2);

        QStringList horheaders;
        horheaders.append("Name");
        horheaders.append("Bet");
        ui->tblBets->setHorizontalHeaderLabels(horheaders);
        ui->tblBets->setColumnWidth(0, 110);

        int t=0;
            HashMap <QString, int>:: Iterator *i = new HashMap <QString, int>::Iterator(*_map);
            while (i->getIndex() != -1)
            {
            ui->tblBets->insertRow(t);
            ui->tblBets->setItem(t, 0, new QTableWidgetItem(i->getCurrentKey()));
            ui->tblBets->setItem(t, 1, new QTableWidgetItem(QString("%1").arg((i->getCurrentValue()))));
            t++;
            i->moveFwd();
            ui->btnDeleteAll->setEnabled(true);
            ui->btnDeleteSelected->setEnabled(true);
            ui->btnChoose->setEnabled(true);
            ui->btnToFile->setEnabled(true);
            }
        }



void MainWindow::on_btnDeleteSelected_clicked()
{
    int i = ui->tblBets->currentRow();
    if (i!=-1)
    _map->del(ui->tblBets->item(i, 0)->text());
    tblRefresh();
}

void MainWindow::on_btnDeleteAll_clicked()
{
    _map->delAll();
    tblRefresh();
}

void MainWindow::on_btnChoose_clicked() const
{

    int bank=0;
    int j = ui->tblBets->currentRow();
    if (j!=-1){
        HashMap <QString, int>:: Iterator *i = new HashMap <QString, int>::Iterator(*_map);
        while (i->getIndex() != -1)
        {
        if(i->getCurrentKey()!=ui->tblBets->item(j, 0)->text())
            bank+=i->getCurrentValue();
        i->moveFwd();
        }
        double cash=0.2*bank;
        QString res="Last bank is: "+QString("%1").arg(cash);
    ui->lblResult->setText(res);
    }
}

void MainWindow::on_btnToFile_clicked()
{
    string text=(ui->lnFile->text()).toStdString()+".txt";
    if (!_map->count())
        _map = new HashMap<QString,int>();
    *(_map) >> text;
}

void MainWindow::on_btnRead_clicked()
{
    string text=(ui->lnFile->text()).toStdString()+".txt";
    _map->delAll();
    if (!_map->count())
        _map = new HashMap<QString,int>();
    *(_map) << text;
    tblRefresh();
}
