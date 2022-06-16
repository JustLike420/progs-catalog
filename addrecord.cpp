#include "addrecord.h"
#include "ui_addrecord.h"

addrecord::addrecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addrecord)
{
    ui->setupUi(this);
}

addrecord::~addrecord()
{
    delete ui;
}

void addrecord::on_pushButton_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("INSERT INTO primer1 VALUES(:name, :post)");
    query->bindValue(":name", ui->lineEdit->text());
    query->bindValue(":post", ui->lineEdit_2->text());

    QMessageBox* mess = new QMessageBox();

    if (!query->exec()){
        mess->setText("Запрос составлен неверно!");
        mess->show();
    }

    emit refresh_table();
}

