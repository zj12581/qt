#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ExcelTools; }
QT_END_NAMESPACE

class ExcelTools : public QMainWindow
{
    Q_OBJECT

public:
    ExcelTools(QWidget *parent = nullptr);
    ~ExcelTools();

private slots:
    void on_ChoonseFile_clicked();

    void on_CreatIni_clicked();


    void on_CreatXmlTest_clicked();


private:
    Ui::ExcelTools *ui;
};
#endif // MAINWINDOW_H
