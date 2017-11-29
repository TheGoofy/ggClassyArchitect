#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T17:31:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ggClassyArchitect
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cxx \
    ggClassyMainWindow.cxx \
    ggClassyGraphicsView.cxx \
    ggClassyApplication.cxx \
    ggItemLinked.cxx \
    ggItemLazy.cxx \
    ggObserver.cxx \
    ggSubject.cxx \
    ggGraphicsTextItem.cxx \
    ggClassyDataSet.cxx \
    ggClassyGraphicsBoxItem.cxx \
    ggItemBlockable.cxx \
    ggClassyGraphicsBoxItems.cxx \
    ggClassyGraphicsPathItem.cxx \
    ggClassyGraphicsScene.cxx

HEADERS += \
    ggClassyMainWindow.h \
    ggClassyGraphicsView.h \
    ggSubject.h \
    ggClassyApplication.h \
    ggItemLinked.h \
    ggItemLazy.h \
    ggObserver.h \
    ggSubjectT.h \
    ggWalkerT.h \
    ggGraphicsHandleItemT.h \
    ggGraphicsTextItem.h \
    ggClassyDataSet.h \
    ggGraphicsManipulatorBarItemT.h \
    ggGraphicsManipulatorRectItemT.h \
    ggClassyGraphicsBoxItem.h \
    ggItemBlockable.h \
    ggClassyGraphicsBoxItems.h \
    ggSubjectValueT.h \
    ggClassyGraphicsPathItem.h \
    ggClassyGraphicsScene.h

FORMS += \
    ggClassyMainWindow.ui
