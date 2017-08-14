#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_openPortBtn_clicked();

    void on_closePortBtn_clicked();

    void on_sendBtn_clicked();

private:
    void initctrls();
private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
