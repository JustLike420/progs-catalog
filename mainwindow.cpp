#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editor.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    fl = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    logwin = new login();
    logwin->show();
}

void MainWindow::on_pushButton_4_clicked()
{
//    edi = new editor();

    ediwin = new editor();
    connect(this, SIGNAL(download_cb()), ediwin, SLOT(AllComboBoxes()));
    ediwin->show();
}


void MainWindow::on_pushButton_clicked()
{

    model = new QSqlQueryModel();
    model->setQuery("SELECT name AS 'Название', platform AS 'Платформа', appointment AS 'Назначение', "
                    "CASE WHEN released_at > DATEADD(year, -1, GETDATE()) THEN 'Менее года назад' "
                    "WHEN released_at < DATEADD(year, -10, GETDATE()) THEN 'Более 10 лет назад' "
                    "WHEN released_at < DATEADD(year, -5, GETDATE()) THEN 'Более 5 лет назад' "
                    "WHEN released_at < DATEADD(year, -1, GETDATE()) THEN 'Более года назад' "
                    "ELSE 'Очень давно' end as 'Дата выхода', "
                    "ISNULL((SELECT full_name FROM users WHERE id_prog=prog.id), 'Владелец не указан') as 'Владелец' FROM prog");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
}
void MainWindow::on_pushButton_2_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT f.name AS 'Название файла', p.name AS 'Название программы', (SELECT d.name FROM dirs d WHERE d.id_prog=p.id) AS 'Название директории' FROM (SELECT * FROM prog) p, files f WHERE f.id_dir in (SELECT id_dir FROM dirs WHERE id_prog=p.id)");

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT d.name AS 'Название директории' FROM dirs d WHERE d.id_prog= ANY(SELECT id FROM prog)");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
}
void MainWindow::on_pushButton_5_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT p.platform, COUNT(*) AS 'Кол-во', ISNULL(fi.extension, 'Другое или нету.') as 'Расширение' FROM prog p LEFT JOIN dirs d on d.id_prog=p.id  LEFT JOIN (SELECT * FROM files f WHERE f.extension='.exe') fi on fi.id_dir=d.id_dir GROUP BY platform, fi.extension HAVING COUNT(*) > 1");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
}
void MainWindow::on_pushButton_6_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM usersProgs");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
}


void MainWindow::obr_ref_tab(){
    on_pushButton_clicked();
}













