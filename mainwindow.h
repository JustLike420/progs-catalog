#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "login.h"
#include "editor.h"
#include "ui_editor.h"
#include "ui_login.h"

#include <QMainWindow>

//#include <QSqlTableModel>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    login* logwin;
    editor* ediwin;


    int globid;
    int fl;

private slots:
    void on_action_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void obr_ref_tab();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

signals:
    void sendID(int);
    void download_cb();
private:
    Ui::MainWindow *ui;
    QSqlQueryModel* model;
};
#endif // MAINWINDOW_H
