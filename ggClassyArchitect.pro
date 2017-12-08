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
    Base/ggItemLinked.cxx \
    Base/ggObserver.cxx \
    Base/ggSubject.cxx \
    Base/ggUtility.cxx \
    BaseGraphics/ggConnectionPoint.cxx \
    BaseGraphics/ggDecoration.cxx \
    BaseGraphics/ggGraphicsAutoConnectPathItem.cxx \
    BaseGraphics/ggGraphicsCheckBoxItem.cxx \
    BaseGraphics/ggGraphicsConnectionPointItem.cxx \
    BaseGraphics/ggGraphicsDecoratedPathItem.cxx \
    BaseGraphics/ggGraphicsTextItem.cxx \
    BaseGraphics/ggPainterPath.cxx \
    BaseGraphics/ggSubjectConnectionPoint.cxx \
    ClassyData/ggClassyClass.cxx \
    ClassyData/ggClassyClassBox.cxx \
    ClassyData/ggClassyClassContainer.cxx \
    ClassyData/ggClassyClassDescription.cxx \
    ClassyData/ggClassyClassMember.cxx \
    ClassyData/ggClassyCollection.cxx \
    ClassyData/ggClassyCollectionContainer.cxx \
    ClassyData/ggClassyDataSet.cxx \
    ClassyData/ggClassyFrame.cxx \
    ClassyGraphics/ggClassyGraphicsBoxItem.cxx \
    ClassyGraphics/ggClassyGraphicsBoxItems.cxx \
    ClassyGraphics/ggClassyGraphicsScene.cxx \
    ClassyGraphics/ggClassyGraphicsView.cxx \
    ClassyMain/ggClassyApplication.cxx \
    ClassyMain/ggClassyMainWindow.cxx \
    ClassyMain/main.cxx

HEADERS += \
    Base/ggBehaviorBlocking.h \
    Base/ggBehaviorLazy.h \
    Base/ggBehaviorT.h \
    Base/ggItemLinked.h \
    Base/ggObserver.h \
    Base/ggSubject.h \
    Base/ggSubjectT.h \
    Base/ggSubjectValueT.h \
    Base/ggTypes.h \
    Base/ggUtility.h \
    Base/ggWalkerT.h \
    BaseGraphics/ggConnectionPoint.h \
    BaseGraphics/ggDecoration.h \
    BaseGraphics/ggGraphicsAutoConnectPathItem.h \
    BaseGraphics/ggGraphicsCheckBoxItem.h \
    BaseGraphics/ggGraphicsConnectionPointItem.h \
    BaseGraphics/ggGraphicsDecoratedPathItem.h \
    BaseGraphics/ggGraphicsHandleItemT.h \
    BaseGraphics/ggGraphicsManipulatorBarItemT.h \
    BaseGraphics/ggGraphicsManipulatorRectItemT.h \
    BaseGraphics/ggGraphicsTextItem.h \
    BaseGraphics/ggPainterPath.h \
    BaseGraphics/ggSubjectConnectionPoint.h \
    ClassyData/ggClassyClass.h \
    ClassyData/ggClassyClassBox.h \
    ClassyData/ggClassyClassContainer.h \
    ClassyData/ggClassyClassDescription.h \
    ClassyData/ggClassyClassMember.h \
    ClassyData/ggClassyCollection.h \
    ClassyData/ggClassyCollectionContainer.h \
    ClassyData/ggClassyDataSet.h \
    ClassyData/ggClassyFrame.h \
    ClassyGraphics/ggClassyGraphicsBoxItem.h \
    ClassyGraphics/ggClassyGraphicsBoxItems.h \
    ClassyGraphics/ggClassyGraphicsScene.h \
    ClassyGraphics/ggClassyGraphicsView.h \
    ClassyMain/ggClassyApplication.h \
    ClassyMain/ggClassyMainWindow.h

FORMS += \
    ClassyMain/ggClassyMainWindow.ui

