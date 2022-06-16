#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QSqlQueryModel>
#include <login.h>
namespace Ui {
class editor;
}

class editor : public QWidget
{
    Q_OBJECT

public:
    explicit editor(QWidget *parent = nullptr);
    ~editor();

private slots:

    void on_view_prog_b_clicked();

    void on_view_dir_b_clicked();

    void on_view_file_b_clicked();

    void on_view_user_b_clicked();

    void on_p_add_b_clicked();

    void AllComboBoxes();
    void on_tableView_clicked(const QModelIndex &index);

    void on_d_add_b_clicked();

    void on_f_add_b_clicked();

    void on_u_add_b_clicked();


    void CustomMenuReq(QPoint);

    void del_zap();


    void on_p_change_b_clicked();

    void on_d_change_b_clicked();

    void on_f_change_b_clicked();

    void on_u_change_b_clicked();

private:
    Ui::editor *ui;
    QSqlQueryModel* model;
    int table_viewed;
    int temp_nom;
    int glob_id;
};

#endif // EDITOR_H
