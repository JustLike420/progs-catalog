#ifndef ADDRECORD_H
#define ADDRECORD_H

#include <QWidget>

#include <QMessageBox>
#include <QSqlQuery>

namespace Ui {
class addrecord;
}

class addrecord : public QWidget
{
    Q_OBJECT

public:
    explicit addrecord(QWidget *parent = nullptr);
    ~addrecord();

private slots:
    void on_pushButton_clicked();

signals:
    void refresh_table();

private:
    Ui::addrecord *ui;
};

#endif // ADDRECORD_H
