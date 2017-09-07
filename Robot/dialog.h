#ifndef DIALOG_H
#define DIALOG_H
#include<QtSerialPort/QSerialPort>
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void snddata(QByteArray &data);
    void sleep(unsigned int time);

private slots:
    void on_openPortBtn_clicked();
    void read_data();

    void on_sndCmdBtn_clicked();

   // void on_pushButton_clicked();

    void on_closePortBtn_clicked();

    void on_sndAngleBtn_clicked();

    void on_op_catch_clicked();

    //void on_pop_place_2_clicked();

    void on_op_Reset_clicked();

    void on_op_bend_clicked();

    void on_op_withdraw_clicked();

    void on_op_grap_clicked();

    void on_pop_place_clicked();

    void on_up_down_clicked();

    void on_sndPosiBtn_clicked();



    void on_Slider_0_sliderMoved(int position);

    void on_Slider_0_valueChanged(int value);

    void on_angle_0_textChanged(const QString &arg1);

    void on_angle_1_textChanged(const QString &arg1);

    void on_angle_2_textChanged(const QString &arg1);

    void on_angle_3_textChanged(const QString &arg1);

    void on_angle_4_textChanged(const QString &arg1);

    void on_angle_5_textChanged(const QString &arg1);

    void on_op_relax_clicked();

private:
    void init();
    void sndAngle(const QString angle,const int index);
private:
    Ui::Dialog *ui;
    QSerialPort *com = new QSerialPort(this);

};

#endif // DIALOG_H
