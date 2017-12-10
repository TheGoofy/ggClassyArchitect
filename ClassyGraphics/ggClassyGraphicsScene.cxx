// 0) include own header
#include "ggClassyGraphicsScene.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseGraphics/ggGraphicsAutoConnectPathItem.h"
#include "BaseGraphics/ggGraphicsConnectionPointItem.h"
#include "ClassyGraphics/ggClassyGraphicsBoxItems.h"


ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mBoxItems(nullptr)
{
  mBoxItems = new ggClassyGraphicsBoxItems();
}


ggClassyGraphicsScene::~ggClassyGraphicsScene()
{
  delete mBoxItems;
}


void ggClassyGraphicsScene::addItem(ggClassyGraphicsBoxItem* aBoxItem)
{
  mBoxItems->AddItem(aBoxItem);
  QGraphicsScene::addItem(aBoxItem);
}


void ggClassyGraphicsScene::AddClassBoxItems(ggClassyDataSet* aDataSet)
{
  // only notify box items change, when all boxes are added
  ggBehaviorLazy::cExecutor vLazy(mBoxItems);

  // loop over box items
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(aDataSet->mClassBoxes);
  while (vClassBoxesIterator) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    ggClassyClass* vClass = aDataSet->mClasses.FindClass(vClassBox->mClassName);
    addItem(new ggClassyGraphicsBoxItem(vClass, vClassBox));
  }
}


void ggClassyGraphicsScene::AddLineItems()
{
  // loop over class box items
  typedef ggClassyGraphicsBoxItems::tBoxItemsSet tBoxItemsSet;
  ggWalkerT<tBoxItemsSet::const_iterator> vBoxItemsWalker(mBoxItems->GetBoxItems());
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    ggClassyClass* vClass = vBoxItem->GetClass();
    if (vClass != nullptr) {

      // loop over base classes
      ggWalkerT<ggStringSet::const_iterator> vBaseClassNamesWalker(vClass->mBaseClassNames);
      while (vBaseClassNamesWalker) {

        // lines between derived class and base class
        const QString& vBaseClassName = *vBaseClassNamesWalker;
        ggGraphicsAutoConnectPathItem* vAutoPath = new ggGraphicsAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f, ggDecoration::cFill::eSolid);
        vAutoPath->InsertPointSrc(vBoxItem->GetClassConnectionTop());
        vAutoPath->InsertPointsDst(mBoxItems->GetClassPointsBottom(vBaseClassName));
        QGraphicsScene::addItem(vAutoPath);
      }

      // loop over members
      const ggClassyClass::tMembers& vMembers = vClass->mMembers;
      for (ggUSize vMemberIndex = 0; vMemberIndex < vMembers.size(); vMemberIndex++) {

        // connect member with class
        const QString& vMemberClassName = vMembers[vMemberIndex].GetClassName();
        ggGraphicsAutoConnectPathItem* vAutoPath = new ggGraphicsAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eDiamond, 15.0f, ggDecoration::cFill::eEmpty);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionLeft(vMemberIndex));
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionRight(vMemberIndex));
        vAutoPath->InsertPointsDst(mBoxItems->GetClassPointsLeftRight(vMemberClassName));
        QPen vPen(vAutoPath->pen());
        vPen.setStyle(Qt::DashLine);
        vAutoPath->setPen(vPen);
        QGraphicsScene::addItem(vAutoPath);
      }
    }
  }
}


void ggClassyGraphicsScene::AddTestConnections()
{
  float vPositionY = 150.0f;
  ggWalkerT<ggDecoration::tTypes::const_iterator> vDecorationTypesWalker(ggDecoration::Types());
  while (vDecorationTypesWalker) {
    const ggDecoration::cType vType = *vDecorationTypesWalker;
    ggGraphicsConnectionPointItem* vPointA = new ggGraphicsConnectionPointItem();
    ggGraphicsConnectionPointItem* vPointB = new ggGraphicsConnectionPointItem();
    vPointA->SetPointPosition(QPointF(250.0f, vPositionY));
    vPointB->SetPointPosition(QPointF(400.0f, vPositionY));
    vPointA->SetPointDirectionRight();
    vPointB->SetPointDirectionLeft();
    ggGraphicsAutoConnectPathItem* vPath = new ggGraphicsAutoConnectPathItem();
    vPath->InsertPointSrc(vPointA->GetSubjectConnectionPoint());
    vPath->InsertPointDst(vPointB->GetSubjectConnectionPoint());
    vPath->SetDecorationSrc(vType, 15.0f, ggDecoration::cFill::eEmpty);
    vPath->SetDecorationDst(vType, 15.0f, ggDecoration::cFill::eSolid);
    QGraphicsScene::addItem(vPointA);
    QGraphicsScene::addItem(vPointB);
    QGraphicsScene::addItem(vPath);
    vPositionY += 20.0f;
  }
}
