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
    text_ = ui->textEdit->toPlainText();
    key_ = ui->key->toPlainText();
    iv_ = ui->iv->toPlainText();

    if (strcmp()) {
        ui->textChat->append("이름을 입력해주세요.");
        return;
    }
    ui->openKBC->//
}


void Widget::on_dockWidget_allowedAreasChanged(const Qt::DockWidgetAreas &allowedAreas)
{

}


