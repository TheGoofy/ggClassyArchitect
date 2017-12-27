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

FORMS += \
    ClassyMain/ggClassyMainWindow.ui \
    ClassyMain/ggClassyDataBrowserDockWidget.ui \
    ClassyMain/ggClassyDataPropertiesDockWidget.ui

DISTFILES += \
    README.md

HEADERS += \
    Base/ggBehaviorBlocking.h \
    Base/ggBehaviorLazy.h \
    Base/ggBehaviorT.h \
    Base/ggItemLinked.h \
    Base/ggObserver.h \
    Base/ggString.h \
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
    ClassyDataSet/ggClassyClass.h \
    ClassyDataSet/ggClassyClassBox.h \
    ClassyDataSet/ggClassyClassContainer.h \
    ClassyDataSet/ggClassyClassMember.h \
    ClassyDataSet/ggClassyCollection.h \
    ClassyDataSet/ggClassyCollectionContainer.h \
    ClassyDataSet/ggClassyDataSet.h \
    ClassyDataSet/ggClassyDescription.h \
    ClassyDataSet/ggClassyFrame.h \
    ClassyGraphics/ggClassyGraphicsBoxItem.h \
    ClassyGraphics/ggClassyGraphicsScene.h \
    ClassyMain/ggClassyApplication.h \
    ClassyMain/ggClassyMainWindow.h \
    BaseGraphics/ggGraphicsView.h \
    ClassyGraphics/ggClassyClassBoxPoints.h \
    ClassyDataSet/ggClassyClassBoxContainer.h \
    ClassyMain/ggClassyDataBrowserDockWidget.h \
    ClassyMain/ggClassyDataPropertiesDockWidget.h \
    BaseWidgets/ggColorWheelWidget.h \
    BaseWidgets/ggUtilityQt.h \
    BaseWidgets/ggColorChannelWidget.h \
    Base/ggVectorSetT.h \
    ClassyMain/ggClassyTreeItem.h \
    ClassyGraphics/ggClassyAutoConnectPathItem.h \
    BaseGraphics/ggGraphicsRectShadowItem.h

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
    ClassyDataSet/ggClassyClass.cxx \
    ClassyDataSet/ggClassyClassBox.cxx \
    ClassyDataSet/ggClassyClassContainer.cxx \
    ClassyDataSet/ggClassyClassMember.cxx \
    ClassyDataSet/ggClassyCollection.cxx \
    ClassyDataSet/ggClassyCollectionContainer.cxx \
    ClassyDataSet/ggClassyDataSet.cxx \
    ClassyDataSet/ggClassyDescription.cxx \
    ClassyDataSet/ggClassyFrame.cxx \
    ClassyGraphics/ggClassyGraphicsBoxItem.cxx \
    ClassyGraphics/ggClassyGraphicsScene.cxx \
    ClassyMain/ggClassyApplication.cxx \
    ClassyMain/ggClassyMainWindow.cxx \
    ClassyMain/main.cxx \
    BaseGraphics/ggGraphicsView.cxx \
    ClassyGraphics/ggClassyClassBoxPoints.cxx \
    ClassyDataSet/ggClassyClassBoxContainer.cxx \
    ClassyMain/ggClassyDataBrowserDockWidget.cxx \
    ClassyMain/ggClassyDataPropertiesDockWidget.cxx \
    BaseWidgets/ggColorWheelWidget.cxx \
    BaseWidgets/ggUtilityQt.cxx \
    BaseWidgets/ggColorChannelWidget.cxx \
    ClassyMain/ggClassyTreeItem.cxx \
    ClassyGraphics/ggClassyAutoConnectPathItem.cxx \
    BaseGraphics/ggGraphicsRectShadowItem.cxx

