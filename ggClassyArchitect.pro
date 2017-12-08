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
    main.cxx \
    ggClassyMainWindow.cxx \
    ggClassyGraphicsView.cxx \
    ggClassyApplication.cxx \
    ggItemLinked.cxx \
    ggObserver.cxx \
    ggSubject.cxx \
    ggGraphicsTextItem.cxx \
    ggClassyDataSet.cxx \
    ggClassyGraphicsBoxItem.cxx \
    ggClassyGraphicsBoxItems.cxx \
    ggClassyGraphicsScene.cxx \
    ggGraphicsCheckBoxItem.cxx \
    ggConnectionPoint.cxx \
    ggSubjectConnectionPoint.cxx \
    ggGraphicsConnectionPointItem.cxx \
    ggDecoration.cxx \
    ggGraphicsDecoratedPathItem.cxx \
    ggGraphicsAutoConnectPathItem.cxx \
    ggUtility.cxx \
    ggPainterPath.cxx \
    ggClassyClassMember.cxx \
    ggClassyClassDescription.cxx \
    ggClassyClass.cxx \
    ggClassyClassBox.cxx \
    ggClassyClassContainer.cxx \
    ggClassyCollection.cxx \
    ggClassyCollectionContainer.cxx \
    ggClassyFrame.cxx

HEADERS += \
    ggClassyMainWindow.h \
    ggClassyGraphicsView.h \
    ggSubject.h \
    ggClassyApplication.h \
    ggItemLinked.h \
    ggObserver.h \
    ggSubjectT.h \
    ggWalkerT.h \
    ggGraphicsHandleItemT.h \
    ggGraphicsTextItem.h \
    ggClassyDataSet.h \
    ggGraphicsManipulatorBarItemT.h \
    ggGraphicsManipulatorRectItemT.h \
    ggClassyGraphicsBoxItem.h \
    ggClassyGraphicsBoxItems.h \
    ggSubjectValueT.h \
    ggClassyGraphicsScene.h \
    ggGraphicsCheckBoxItem.h \
    ggBehaviorT.h \
    ggBehaviorLazy.h \
    ggBehaviorBlocking.h \
    ggConnectionPoint.h \
    ggSubjectConnectionPoint.h \
    ggGraphicsConnectionPointItem.h \
    ggDecoration.h \
    ggGraphicsDecoratedPathItem.h \
    ggGraphicsAutoConnectPathItem.h \
    ggTypes.h \
    ggUtility.h \
    ggPainterPath.h \
    ggClassyClassMember.h \
    ggClassyClassDescription.h \
    ggClassyClass.h \
    ggClassyClassBox.h \
    ggClassyClassContainer.h \
    ggClassyCollection.h \
    ggClassyCollectionContainer.h \
    ggClassyFrame.h

FORMS += \
    ggClassyMainWindow.ui
