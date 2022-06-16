#include "add_prog.h"
#include "ui_add_prog.h"

add_prog::add_prog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_prog)
{
    ui->setupUi(this);
}

add_prog::~add_prog()
{
    delete ui;
}
