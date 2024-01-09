QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flowchar_arrow_button.cpp \
    flowchar_graphics_item_base.cpp \
    flowchar_graphics_link.cpp \
    flowchar_widget.cpp \
    flowchart_graphics_item.cpp \
    flowchart_scene.cpp \
    flowchart_view.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Curve.h \
    PlottingMath.h \
    flowchar_arrow_button.h \
    flowchar_graphics_item_base.h \
    flowchar_graphics_link.h \
    flowchar_widget.h \
    flowchart_global.h \
    flowchart_graphics_item.h \
    flowchart_scene.h \
    flowchart_view.h \
    mainwindow.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MyResource.qrc
