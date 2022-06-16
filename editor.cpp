#include "editor.h"
#include "login.h"
#include "ui_editor.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#define PROG 0
#define DIR 1
#define FILE 2
#define USER 3

editor::editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editor)
{
    ui->setupUi(this);
    // разрешить вызов контекстного меню
    AllComboBoxes();
ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),SLOT(CustomMenuReq(QPoint)));
ui->p_date_de->setDate(QDate::currentDate());
ui->f_date_de->setDate(QDate::currentDate());
//connect(ui->view_prog_b,SIGNAL(clicked()),this,SLOT(AllComboBoxes()));

}

editor::~editor()
{
    delete ui;
}

void editor::AllComboBoxes(){
    QSqlQuery* query = new QSqlQuery();
    query->exec("SELECT name FROM prog");
     ui->d_prog_cb->clear();
     ui->f_dir_cb->clear();
     ui->u_prog_cb->clear();
    while(query->next()){
        ui->d_prog_cb->addItem(query->value(0).toString());
        ui->u_prog_cb->addItem(query->value(0).toString());
    }
    query->exec("SELECT name FROM dirs");

    while(query->next()){
        ui->f_dir_cb->addItem(query->value(0).toString());
    }

}
void editor::on_view_prog_b_clicked()
{
    AllComboBoxes();
    model = new QSqlQueryModel();
    model->setQuery("SELECT id, name AS 'Название', released_at AS 'Дата выхода', platform AS 'Платформа', appointment AS 'Назначение' FROM prog");
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    table_viewed = 0;
}


void editor::on_view_dir_b_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT id_dir, name AS 'Название', (SELECT p.name FROM prog p WHERE p.id=dirs.id_prog) AS 'Название программы' FROM dirs");
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    table_viewed = 1;
}


void editor::on_view_file_b_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT id_files,  name AS 'Название', extension AS 'Расширение', size AS 'Размер', created_at AS 'Дата создания', modified_at AS 'Дата изменения',"
                    "(SELECT d.name FROM dirs d WHERE d.id_dir=files.id_dir) AS 'Название директории' FROM files");
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    table_viewed = 2;
}


void editor::on_view_user_b_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT id, full_name AS 'Имя', country AS 'Страна', (SELECT p.name FROM prog p WHERE p.id=users.id_prog) AS 'Программа' FROM users");
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    table_viewed = 3;
}


void editor::on_p_add_b_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("EXEC add_prog :name, :released_at, :platform, :appointment");
    query->bindValue(":name",ui->p_name_le->text());
    query->bindValue(":released_at",ui->p_date_de->text());
    query->bindValue(":platform",ui->p_platform_le->text());
    query->bindValue(":appointment",ui->p_apppo_le->text());

    QMessageBox* mess = new QMessageBox();
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        on_view_prog_b_clicked();
    }
}

void editor::on_tableView_clicked(const QModelIndex &index)
{
    temp_nom = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
    qDebug()<<temp_nom<<table_viewed;
    QSqlQuery* query = new QSqlQuery();

    switch (table_viewed){
        case PROG:
            qDebug()<<"prog";
            query->prepare("SELECT name, released_at, platform, appointment FROM prog WHERE id=:id");
            query->bindValue(":id", temp_nom);
            if(query->exec()){
                query->first();
                ui->p_name_le->setText(query->value(0).toString());
                ui->p_date_de->setDate(query->value(1).toDate());
                ui->p_platform_le->setText(query->value(2).toString());
                ui->p_apppo_le->setText(query->value(3).toString());
            }
        break;
        case DIR:
            qDebug()<<"dir";
            query->prepare("SELECT name, id_prog FROM dirs WHERE id_dir=:id");
            query->bindValue(":id", temp_nom);
            if(query->exec()){
                query->first();
                ui->d_name_le->setText(query->value(0).toString());
                ui->d_prog_cb->setCurrentIndex(1);
            }
            break;
        case FILE:
            qDebug()<<"file";
            query->prepare("SELECT name, id_dir, extension, size, created_at  FROM files WHERE id_files=:id");
            query->bindValue(":id", temp_nom);
            if(query->exec()){
                query->first();
                ui->f_name_le->setText(query->value(0).toString());
                ui->f_ext_le->setText(query->value(2).toString());
                ui->f_size_le->setText(query->value(3).toString());
                ui->f_date_de->setDate(query->value(4).toDate());
                ui->f_dir_cb->setCurrentText(query->value(1).toString());
            }
            break;
        case USER:
            qDebug()<<"user";
            query->prepare("SELECT full_name, country, id_prog FROM users WHERE id=:id");
            query->bindValue(":id", temp_nom);
            if(query->exec()){
                query->first();
                ui->u_name_le->setText(query->value(0).toString());
                ui->u_country_le->setText(query->value(1).toString());
                ui->u_prog_cb->setCurrentText(query->value(2).toString());
            }
            break;
        }


}

void editor::on_d_add_b_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    int id_prog;
    query->prepare("SELECT id FROM prog WHERE name=:name");
    query->bindValue(":name", ui->d_prog_cb->currentText());
    query->exec();
    query->first();
    id_prog = query->value(0).toInt();

    query->prepare("EXEC add_dir :id_prog, :name");
    query->bindValue(":name",ui->d_name_le->text());
    query->bindValue(":id_prog",id_prog);

    QMessageBox* mess = new QMessageBox();
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        on_view_dir_b_clicked();
    }
}



void editor::on_f_add_b_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    int id_dir;
    query->prepare("SELECT MAX(id_dir) FROM dirs WHERE name=:name");
    query->bindValue(":name", ui->f_dir_cb->currentText());
    query->exec();
    query->first();
    id_dir = query->value(0).toInt();

    query->prepare("EXEC add_file :id_dir, :name, :extension, :size, :created");
    query->bindValue(":id_dir", id_dir);
    query->bindValue(":name", ui->f_name_le->text());
    query->bindValue(":extension", ui->f_ext_le->text());
    query->bindValue(":size", ui->f_size_le->text());
    query->bindValue(":created", ui->f_date_de->text());

    QMessageBox* mess = new QMessageBox();
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        on_view_file_b_clicked();
    }
}


void editor::on_u_add_b_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    int id_prog;
    query->prepare("SELECT id FROM prog WHERE name=:name");
    query->bindValue(":name", ui->u_prog_cb->currentText());
    query->exec();
    query->first();
    id_prog = query->value(0).toInt();

    query->prepare("EXEC add_user :id_prog, :name, :country");
    query->bindValue(":name",ui->u_name_le->text());
    query->bindValue(":id_prog",id_prog);
    query->bindValue(":country",ui->u_country_le->text());

    QMessageBox* mess = new QMessageBox();
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        on_view_user_b_clicked();
    }
}



void editor::CustomMenuReq(QPoint pos){
    QModelIndex index = ui->tableView->indexAt(pos);
    glob_id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
    QMenu* menu = new QMenu(this);
    QAction* del = new QAction("Удалить", this);

    connect(del, SIGNAL(triggered()), this, SLOT(del_zap()));

    menu->addAction(del);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void editor::del_zap(){
    QSqlQuery* query = new QSqlQuery();
    QMessageBox* mess = new QMessageBox();
    switch (table_viewed){
    case PROG:

        query->prepare("EXEC delete_prog :id");
        query->bindValue(":id", glob_id);

        if(!query->exec()){
            mess->setText("Запрос неверный");
            mess->show();
        }else{
            mess->setText("Запись удалена");
            mess->show();
            on_view_prog_b_clicked();
        }
        break;
    case DIR:
        query->prepare("EXEC delete_dir :id");
        query->bindValue(":id", glob_id);
        if(!query->exec()){
            mess->setText("Запрос неверный");
            mess->show();
        }else{
            mess->setText("Запись удалена");
            mess->show();
            on_view_dir_b_clicked();
        }
        break;
    case FILE:
        query->prepare("EXEC delete_file :id");
        query->bindValue(":id", glob_id);
        if(!query->exec()){
            mess->setText("Запрос неверный");
            mess->show();
        }else{
            mess->setText("Запись удалена");
            mess->show();
            on_view_file_b_clicked();
        }
        break;
    case USER:
        query->prepare("EXEC delete_user :id");
        query->bindValue(":id", glob_id);;
        if(!query->exec()){
            mess->setText("Запрос неверный");
            mess->show();
        }else{
            mess->setText("Запись удалена");
            mess->show();
            on_view_user_b_clicked();
        }
        break;
    }
}

void editor::on_p_change_b_clicked()
{
    QMessageBox* mess = new QMessageBox();
    QSqlQuery* query = new QSqlQuery();
    qDebug()<<temp_nom;
    query->prepare("EXEC update_prog :id, :name, :release, :platform, :appointment");
    query->bindValue(":id", temp_nom);
    query->bindValue(":name", ui->p_name_le->text());
    query->bindValue(":release", ui->p_date_de->text());
    query->bindValue(":platform", ui->p_platform_le->text());
    query->bindValue(":appointment", ui->p_apppo_le->text());
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        mess->setText("Запись изменена");
        mess->show();
        on_view_prog_b_clicked();
        AllComboBoxes();
    }
}

void editor::on_d_change_b_clicked()
{
    QMessageBox* mess = new QMessageBox();
    QSqlQuery* query = new QSqlQuery();

    int id_prog;
    query->prepare("SELECT MAX(id) FROM prog WHERE name=:name");
    query->bindValue(":name", ui->d_prog_cb->currentText());
    query->exec();
    query->first();
    id_prog = query->value(0).toInt();

    query->prepare("EXEC update_dir :id, :id_prog, :name");
    query->bindValue(":id", temp_nom);
    query->bindValue(":id_prog", id_prog);
    query->bindValue(":name", ui->d_name_le->text());

    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        mess->setText("Запись изменена");
        mess->show();
        on_view_dir_b_clicked();
        AllComboBoxes();
    }
}


void editor::on_f_change_b_clicked()
{
    QMessageBox* mess = new QMessageBox();
    QSqlQuery* query = new QSqlQuery();
    int id_dir;
    query->prepare("SELECT MAX(id_dir) FROM dirs WHERE name=:name");
    query->bindValue(":name", ui->f_dir_cb->currentText());
    query->exec();
    query->first();
    id_dir = query->value(0).toInt();


    query->prepare("EXEC update_file :id, :id_dir, :name, :extension, :size, :created");
    query->bindValue(":id", temp_nom);
    query->bindValue(":id_dir", id_dir);
    query->bindValue(":name", ui->f_name_le->text());
    query->bindValue(":extension", ui->f_ext_le->text());
    query->bindValue(":size", ui->f_size_le->text());
    query->bindValue(":created", ui->f_date_de->text());
    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        mess->setText("Запись изменена");
        mess->show();
        on_view_file_b_clicked();
        AllComboBoxes();
    }
}


void editor::on_u_change_b_clicked()
{
    QMessageBox* mess = new QMessageBox();
    QSqlQuery* query = new QSqlQuery();
    int id_prog;
    query->prepare("SELECT MAX(id) FROM prog WHERE name=:name");
    query->bindValue(":name", ui->u_prog_cb->currentText());
    query->exec();
    query->first();
    id_prog = query->value(0).toInt();

    qDebug()<<ui->u_prog_cb->currentText() << id_prog;
    query->prepare("EXEC update_user :id, :id_prog, :name, :country");
    query->bindValue(":id", temp_nom);
    query->bindValue(":id_prog",id_prog);
    query->bindValue(":name", ui->u_name_le->text());
    query->bindValue(":country", ui->u_country_le->text());

    if(!query->exec()){
        mess->setText("Запрос неверный");
        mess->show();
    }else{
        mess->setText("Запись изменена");
        mess->show();
        on_view_user_b_clicked();
        AllComboBoxes();
    }

}

