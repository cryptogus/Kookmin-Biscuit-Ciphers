#include "widget.h"
#include "ui_widget.h"
#include "../apps/api.h"

void hexStringToBytes(const char* hexString, unsigned char* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &bytes[i]);
    }
}

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

    void (*cipher)(uint8_t *, uint8_t *, uint8_t *) = 0;

    if (ui->Encrypt_2->checkState() && !(ui->Decrypt->checkState())) {
        if (ui->AES128->checkState()) {
            cipher = AES128_Encrypt;
        }else if (ui->AES192->checkState()) {
            cipher = AES192_Encrypt;
        }else if (ui->PIPO128->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO128_ENC;
        }
    } else if (!(ui->Encrypt_2->checkState()) && (ui->Decrypt->checkState())) {

        if (ui->AES128->checkState()) {
            cipher = AES128_Decrypt;
        }else if (ui->AES192->checkState()) {
            cipher = AES192_Decrypt;
        }
    }
    cipher;

}

