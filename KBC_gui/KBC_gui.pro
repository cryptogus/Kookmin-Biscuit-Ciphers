QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp \
    #../src/aes/aes.c \
    #../src/lea/lea.c \
    #../src/mode/ECB.c \
    #../src/mode/CBC.c \
    #../src/padding/pkcs7.c \
    #../src/pipo/pipo.c \
    #../src/aria/aria.c \
    #../src/seed/seed.c

HEADERS += \
    widget.h \
    qt_api.h

FORMS += \
    widget.ui


# DESTDIR=./
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += \
    -L$$PWD -lKBC \
    -lcrypto
    
