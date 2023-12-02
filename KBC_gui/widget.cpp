#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_openKBC_clicked()
{
    text_ = ui->textEdit_2->toPlainText().toStdString();
    key_ = ui->key_2->toPlainText().toStdString();
    iv_ = ui->iv_2->toPlainText().toStdString();

    if (text_== "") {
        ui->textBrowser_2->append("Input text.");
        return;
    }
    if (key_== "") {
        ui->textBrowser_2->append("Input key.");
        return;
    }
    if (iv_ == "") {
        ui->textBrowser_2->append("Input iv.");
        return;
    }


}

