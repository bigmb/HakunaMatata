#ifndef POP_UP_WINDOW_H
#define POP_UP_WINDOW_H

#include <QDialog>

namespace Ui {
class Pop_up_window;
}

class Pop_up_window : public QDialog
{
    Q_OBJECT

public:
    explicit Pop_up_window(QWidget *parent = 0);
    ~Pop_up_window();

private:
    Ui::Pop_up_window *ui;
};

#endif // POP_UP_WINDOW_H
