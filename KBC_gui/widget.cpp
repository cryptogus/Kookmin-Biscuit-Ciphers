#include "widget.h"
#include "ui_widget.h"
#include "qt_api.h"
#include <cstdlib>
#include <string>

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
        }else if (ui->AES256->checkState()) {
            cipher = AES256_Encrypt;
        }else if (ui->PIPO128->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO128_ENC;
            BLOCK_SIZE = 8;
        }else if (ui->PIPO256->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO256_ENC;
            BLOCK_SIZE = 8;
        }else if (ui->LEA128->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA128_ENC;
        }else if (ui->LEA192->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA192_ENC;
        }else if (ui->LEA256->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA256_ENC;
        }else if (ui->ARIA128->checkState()){
            cipher = ARIA128_ENC;
        }else if (ui->ARIA192->checkState()){
            cipher = ARIA192_ENC;
        }else if (ui->ARIA256->checkState()){
            cipher = ARIA256_ENC;
        }else if (ui->SEED->checkState()){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))SEED_Enc;
        }else{
            ui->textBrowser_2->append("Check Block Cipher.");
        }
        // pkcs7 padding
        size_t len = 0;
        unsigned char *pad_plainText = pkcs7_padding((unsigned char *)text_.c_str(), BLOCK_SIZE, &len);
        unsigned char *cipherText = (unsigned char *)calloc(sizeof(unsigned char), len);

        if (ui->ECB->checkState() && !(ui->CBC->checkState())) {
            ECB(cipher, (unsigned char *)key_.c_str(), BLOCK_SIZE, len, pad_plainText, cipherText);
        }else if (ui->CBC->checkState() && !(ui->ECB->checkState())){
            CBC_enc(cipher, (unsigned char *)iv_.c_str(), (unsigned char *)key_.c_str(), BLOCK_SIZE, len, pad_plainText, cipherText);
        }else {
            ui->textBrowser_2->append("Check Modes of Operation.");
        }
        // ui->textBrowser_2->append("==================== ciphertext ====================");
        QString output = QString("ciphertext: %s").arg((const char*)cipherText);
        // ui->textBrowser_2->append(str.data());
        ui->textBrowser_2->append(output);

        free(pad_plainText);
        free(cipherText);
    } else if (!(ui->Encrypt_2->checkState()) && (ui->Decrypt->checkState())) {

        if (ui->AES128->checkState()) {
            cipher = AES128_Decrypt;
        }else if (ui->AES192->checkState()) {
            cipher = AES192_Decrypt;
        }else if (ui->AES256->checkState()) {
            cipher = AES256_Decrypt;
        }else if (ui->PIPO128->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO128_DEC;
            BLOCK_SIZE = 8;
        }else if (ui->PIPO256->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO256_DEC;
            BLOCK_SIZE = 8;
        }else if (ui->LEA128->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA128_DEC;
        }else if (ui->LEA192->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA192_DEC;
        }else if (ui->LEA256->checkState()) {
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA256_DEC;
        }else if (ui->ARIA128->checkState()){
            cipher = ARIA128_DEC;
        }else if (ui->ARIA192->checkState()){
            cipher = ARIA192_DEC;
        }else if (ui->ARIA256->checkState()){
            cipher = ARIA256_DEC;
        }else if (ui->SEED->checkState()){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))SEED_Dec;
        }else{
            ui->textBrowser_2->append("Check Block Cipher.");
        }

    } else {
        ui->textBrowser_2->append("Check Encrypt or Decrypt (Please choose one of the two).");
    }

}

