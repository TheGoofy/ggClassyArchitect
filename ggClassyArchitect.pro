#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T17:31:40
#
#-------------------------------------------------

QT       += core gui xml

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
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Base/ggSubject.cxx \
    Base/ggItemLinked.cxx \
    Base/ggUtility.cxx \
    Base/ggObserver.cxx \
    Data/ggClassyDataSet.cxx \
    Data/ggClassyFrame.cxx \
    Data/ggClassyCollectionContainer.cxx \
    Data/ggClassyCollection.cxx \
    Data/ggClassyClassContainer.cxx \
    Data/ggClassyClassBox.cxx \
    Data/ggClassyClass.cxx \
    Data/ggClassyClassDescription.cxx \
    Data/ggClassyClassMember.cxx \
    Graphics/ggClassyGraphicsBoxItems.cxx \
    Graphics/ggClassyGraphicsBoxItem.cxx \
    Graphics/ggGraphicsTextItem.cxx \
    Graphics/ggPainterPath.cxx \
    Graphics/ggGraphicsDecoratedPathItem.cxx \
    Graphics/ggGraphicsAutoConnectPathItem.cxx \
    Graphics/ggDecoration.cxx \
    Graphics/ggSubjectConnectionPoint.cxx \
    Graphics/ggGraphicsConnectionPointItem.cxx \
    Graphics/ggGraphicsCheckBoxItem.cxx \
    Graphics/ggConnectionPoint.cxx \
    Graphics/ggClassyGraphicsScene.cxx \
    Graphics/ggClassyGraphicsView.cxx \
    Main/ggClassyApplication.cxx \
    Main/ggClassyMainWindow.cxx \
    Main/main.cxx

HEADERS += \
    Base/ggSubject.h \
    Base/ggObserver.h \
    Base/ggTypes.h \
    Base/ggWalkerT.h \
    Base/ggUtility.h \
    Base/ggBehaviorT.h \
    Base/ggSubjectValueT.h \
    Base/ggSubjectT.h \
    Base/ggItemLinked.h \
    Base/ggBehaviorLazy.h \
    Base/ggBehaviorBlocking.h \
    Data/ggClassyDataSet.h \
    Data/ggClassyFrame.h \
    Data/ggClassyCollectionContainer.h \
    Data/ggClassyCollection.h \
    Data/ggClassyClassContainer.h \
    Data/ggClassyClassBox.h \
    Data/ggClassyClass.h \
    Data/ggClassyClassDescription.h \
    Data/ggClassyClassMember.h \
    Graphics/ggGraphicsTextItem.h \
    Graphics/ggGraphicsManipulatorBarItemT.h \
    Graphics/ggClassyGraphicsBoxItem.h \
    Graphics/ggPainterPath.h \
    Graphics/ggDecoration.h \
    Graphics/ggClassyGraphicsBoxItems.h \
    Graphics/ggGraphicsDecoratedPathItem.h \
    Graphics/ggGraphicsAutoConnectPathItem.h \
    Graphics/ggSubjectConnectionPoint.h \
    Graphics/ggGraphicsManipulatorRectItemT.h \
    Graphics/ggGraphicsHandleItemT.h \
    Graphics/ggGraphicsConnectionPointItem.h \
    Graphics/ggGraphicsCheckBoxItem.h \
    Graphics/ggConnectionPoint.h \
    Graphics/ggClassyGraphicsScene.h \
    Graphics/ggClassyGraphicsView.h \
    Main/ggClassyApplication.h \
    Main/ggClassyMainWindow.h


FORMS += \
    Main/ggClassyMainWindow.ui

