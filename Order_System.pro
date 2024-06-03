QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cashier_manage.cpp \
    dinein_ok.cpp \
    dinein_order.cpp \
    dinein_pay.cpp \
    dish_manage.cpp \
    main.cpp \
    manage.cpp \
    token_ok.cpp \
    token_order.cpp \
    token_pay.cpp \
    widget.cpp

HEADERS += \
    cashier_manage.h \
    dinein_ok.h \
    dinein_order.h \
    dinein_pay.h \
    dish_manage.h \
    manage.h \
    token_ok.h \
    token_order.h \
    token_pay.h \
    widget.h

FORMS += \
    cashier_manage.ui \
    dinein_ok.ui \
    dinein_order.ui \
    dinein_pay.ui \
    dish_manage.ui \
    manage.ui \
    token_ok.ui \
    token_order.ui \
    token_pay.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
