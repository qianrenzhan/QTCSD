#-------------------------------------------------
#
# Project created by QtCreator 2018-11-28T10:08:51
#
#-------------------------------------------------

QT       += core gui sql multimedia charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTCSD
TEMPLATE = app

QMAKE_CXXFLAGS += /MP

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += ARM

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = stable.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ImageProcessingTool.cpp \
    account/account.cpp \
    account/accountlab.cpp \
    colorspace/colorspace.cpp \
    colorspace/colorspacelab.cpp \
    template/template.cpp \
    template/templatelab.cpp \
    helper/iconhelper.cpp \
    taskstatistics/taskstatistics.cpp \
    taskstatistics/taskstatisticslab.cpp \
    dlg/login/logindlg.cpp \
    dlg/signup/signupdialog.cpp \
    databasehelper/databasehelper.cpp \
    mywidget/qmytreewidget.cpp \
    mywidget/mymessagebox.cpp \
    templatetree/templatetree.cpp \
    templatetree/templatetreelab.cpp \
    mywidget/chart/chartwidget.cpp \
    mywidget/chart/customslice.cpp

HEADERS += \
        mainwindow.h \
    ImageProcessingTool.h \
    account/account.h \
    account/accountlab.h \
    colorspace/colorspace.h \
    colorspace/colorspacelab.h \
    template/template.h \
    template/templatelab.h \
    helper/iconhelper.h \
    helper/qbasehelper.h \
    databasehelper/databasehelper.h \
    taskstatistics/taskstatistics.h \
    taskstatistics/taskstatisticslab.h \
    dlg/login/logindlg.h \
    dlg/signup/signupdialog.h \
    mywidget/qmytreewidget.h \
    mywidget/mymessagebox.h \
    templatetree/templatetree.h \
    templatetree/templatetreelab.h \
    mywidget/chart/chartwidget.h \
    mywidget/chart/customslice.h \
    stable.h

FORMS += \
        mainwindow.ui \
    dlg/login/logindlg.ui \
    dlg/signup/signupdialog.ui \
    mywidget/mymessagebox.ui

INCLUDEPATH += account   \
    colorspace  \
    databasehelper  \
    helper  \
    mywidget    \
    mywidget/chart  \
    taskstatistics  \
    template    \
    templatetree    \
    dlg/login   \
    dlg/signup

win32{
message("compile for windows")
# opencv
INCLUDEPATH+= D:\OpenCV\opencv\build\include  \
            D:\OpenCV\opencv\build\include\opencv    \
            D:\OpenCV\opencv\build\include\opencv2
LIBS  +=  D:\OpenCV\vs2017-64build\install\lib\opencv_*2410d.lib


#DAHENG
INCLUDEPATH += $$quote(D:\Program Files\Daheng Imavision\MER-Series\Samples\VC SDK\inc)
LIBS += $$quote(D:\Program Files\Daheng Imavision\MER-Series\Samples\VC SDK\lib\x64\*.lib)
}
linux{
QMAKE_DEFAULT_INCDIRS = ...

INCLUDEPATH += /usr/local/include \
    /usr/include/opencv \
    /usr/include/opencv2

INCLUDEPATH+=/usr/include


if(contains(DEFINES,ARM)){
    message("compile for arm linux")
    LIBS += /usr/lib/aarch64-linux-gnu/libmysqlclient.so
    LIBS += /usr/lib/libopencv_*.so.3.3
}else{
    message("compile for amd linux")
    LIBS += /usr/lib/x86_64-linux-gnu/libmysqlclient.so
    LIBS += /usr/lib/x86_64-linux-gnu/libopencv_*.so.3.2
}

#LIBS += /usr/lib/libopencv_*.so.3.3

}

RESOURCES += \
    ArmsMain.qrc \
    rc.qrc


