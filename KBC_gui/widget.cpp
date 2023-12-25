#include "widget.h"
#include "ui_widget.h"
#include "qt_api.h"
#include <cstdlib>
#include <string>
#include <QDebug>

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
        QString output = QString("ciphertext: %1").arg(QString::fromUtf8((const char*)cipherText).toLatin1().toHex().constData());
        // hex 값 출력
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
        // 2바이트씩 분할한 결과를 저장할 배열
        size_t hexStringLength = strlen(text_.c_str());
        size_t byteLength = hexStringLength / 2;
        unsigned char *cipherText2 = (unsigned char*)malloc(byteLength);
        unsigned char *decPlainText = (unsigned char *)calloc(sizeof(unsigned char), byteLength);

        // 16진수 문자열을 2바이트씩 분할하고 1바이트로 변환
        hexStringToBytes(text_.c_str(), cipherText2, byteLength);

        if (ui->ECB->checkState() && !(ui->CBC->checkState())) {
            ECB(cipher, (unsigned char *)key_.c_str(), BLOCK_SIZE, byteLength, cipherText2, decPlainText);
        }else if (ui->CBC->checkState() && !(ui->ECB->checkState())){
            CBC_dec(cipher, (unsigned char *)iv_.c_str(), (unsigned char *)key_.c_str(), BLOCK_SIZE, byteLength, cipherText2, decPlainText);
        }else {
            ui->textBrowser_2->append("Check Modes of Operation.");
        }
        
        unsigned char *decPlainText2 = pkcs7_depadding(decPlainText, &byteLength);
        
        // QDebug 헤더를 이용한 값의 출력 확인
        //qDebug() << "decPlainText2: " << decPlainText2;
        // decPlainText는 QString::fromUtf8로 하니 영문이 깨져서 출력 됨. decPlainText2는 출력 자체가 안됨
        QString output2 = QString("plaintext: %1").arg(QString::fromUtf8((const char *)decPlainText2));
        //qDebug() << "output2: " << output2;
        

        // 값 출력
        ui->textBrowser_2->append(output2);

        free(cipherText2);
        free(decPlainText);
        free(decPlainText2);
    } else {
        ui->textBrowser_2->append("Check Encrypt or Decrypt (Please choose one of the two).");
    }

}

