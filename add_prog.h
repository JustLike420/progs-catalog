#ifndef ADD_PROG_H
#define ADD_PROG_H

#include <QWidget>

namespace Ui {
class add_prog;
}

class add_prog : public QWidget
{
    Q_OBJECT

public:
    explicit add_prog(QWidget *parent = nullptr);
    ~add_prog();

private:
    Ui::add_prog *ui;
};

#endif // ADD_PROG_H
