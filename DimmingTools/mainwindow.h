#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PutWave(double *buffer[2],uint16_t Index);
private slots:
    void on_WritePoint_clicked();

    void on_Revocation_clicked();

    void on_Save_clicked();


//    void on_Fit_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
