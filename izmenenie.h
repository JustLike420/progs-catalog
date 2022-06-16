#ifndef IZMENENIE_H
#define IZMENENIE_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class izmenenie;
}

class izmenenie : public QWidget
{
    Q_OBJECT

public:
    explicit izmenenie(QWidget *parent = nullptr);
    ~izmenenie();
private slots:
    void obr_sendID(int);

    void on_pushButton_clicked();

private:
    Ui::izmenenie *ui;
};

#endif // IZMENENIE_H
