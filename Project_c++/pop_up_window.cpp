#include "pop_up_window.h"
#include "ui_pop_up_window.h"

Pop_up_window::Pop_up_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pop_up_window)
{
    ui->setupUi(this);
}

Pop_up_window::~Pop_up_window()
{
    delete ui;
}
