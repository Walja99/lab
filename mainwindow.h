#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QTableWidgetItem>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>


#include "HashMap.h"
#include "dialogwindow.h"

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void tblRefresh();
    void updInput();
    HashMap <QString, int> *_map;


protected:
    void changeEvent(QEvent *e);

private slots:


    void on_btnAdd_clicked();

    void on_btnDeleteSelected_clicked();

    void on_btnDeleteAll_clicked();

    void on_btnChoose_clicked() const;

    void on_btnToFile_clicked();

    void on_btnRead_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
