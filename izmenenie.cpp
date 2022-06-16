#include "izmenenie.h"
#include "ui_izmenenie.h"

izmenenie::izmenenie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::izmenenie)
{
    ui->setupUi(this);
}

izmenenie::~izmenenie()
{
    delete ui;
}

void izmenenie::obr_sendID(int a)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT name, post FROM primer1 WHERE id=?");
    query->bindValue(0, a);

    if (query->exec()){
        query->next();
        ui->lineEdit->setText(query->value(0).toString());
        ui->lineEdit_2->setText(query->value(1).toString());
        ui->lineEdit_3->setText(QString::number(a));
    }
}

void izmenenie::on_pushButton_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE primer1 SET name=?, post=? WHERE id=?");
    query->bindValue(0, ui->lineEdit->text());
    query->bindValue(1, ui->lineEdit_2->text());
    query->bindValue(2, ui->lineEdit_3->text().toInt());
    if (query->exec()){

    }
}

