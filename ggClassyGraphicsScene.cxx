#include "ggClassyGraphicsScene.h"

#include "ggClassyGraphicsBoxItems.h"
#include "ggGraphicsAutoConnectPathItem.h"
#include "ggGraphicsConnectionPointItem.h"


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

  // testing the auto-connect-path
  ggGraphicsConnectionPointItem* vPointA = new ggGraphicsConnectionPointItem();
  ggGraphicsConnectionPointItem* vPointB = new ggGraphicsConnectionPointItem();
  ggGraphicsConnectionPointItem* vPointC = new ggGraphicsConnectionPointItem();
  ggGraphicsConnectionPointItem* vPointD = new ggGraphicsConnectionPointItem();
  vPointA->SetPointPosition(QPointF(100.0f, -100.0f));
  vPointB->SetPointPosition(QPointF(100.0f,  100.0f));
  vPointC->SetPointPosition(QPointF(300.0f, -100.0f));
  vPointD->SetPointPosition(QPointF(300.0f,  100.0f));
  vPointA->SetPointDirectionRight();
  vPointB->SetPointDirectionRight();
  vPointC->SetPointDirectionLeft();
  vPointD->SetPointDirectionLeft();
  ggGraphicsAutoConnectPathItem* vPath1 = new ggGraphicsAutoConnectPathItem();
  vPath1->InsertPointSrc(vPointA->GetSubjectConnectionPoint());
  vPath1->InsertPointSrc(vPointB->GetSubjectConnectionPoint());
  vPath1->InsertPointDst(vPointC->GetSubjectConnectionPoint());
  vPath1->InsertPointDst(vPointD->GetSubjectConnectionPoint());
  vPath1->SetDecorationSrc(ggDecoration::cType::eDiamond, 20.0f, ggDecoration::cFill::eEmpty);
  vPath1->SetDecorationDst(ggDecoration::cType::eCircle, 10.0f, ggDecoration::cFill::eEmpty);
  ggGraphicsAutoConnectPathItem* vPath2 = new ggGraphicsAutoConnectPathItem();
  vPath2->InsertPointSrc(vPointC->GetSubjectConnectionPoint());
  vPath2->InsertPointSrc(vPointD->GetSubjectConnectionPoint());
  vPath2->InsertPointDst(vPointA->GetSubjectConnectionPoint());
  vPath2->InsertPointDst(vPointB->GetSubjectConnectionPoint());
  vPath2->SetDecorationSrc(ggDecoration::cType::eArrowBack, 13.0f, ggDecoration::cFill::eEmpty);
  vPath2->SetDecorationDst(ggDecoration::cType::eTriangleBack, 13.0f, ggDecoration::cFill::eSolid);
  QGraphicsScene::addItem(vPointA);
  QGraphicsScene::addItem(vPointB);
  QGraphicsScene::addItem(vPointC);
  QGraphicsScene::addItem(vPointD);
  QGraphicsScene::addItem(vPath1);
  QGraphicsScene::addItem(vPath2);


  // only notify box items change, when all boxes are added
  ggBehaviorLazy::cExecutor vLazy(mBoxItems);

  // loop over box items
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(aDataSet->mClassBoxes);
  while (vClassBoxesIterator) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    addItem(new ggClassyGraphicsBoxItem(vClassBox));
  }
}


void ggClassyGraphicsScene::AddLineItems(ggClassyDataSet* aDataSet)
{
  // loop over class box items
  typedef ggClassyGraphicsBoxItems::tBoxItemsSet tBoxItemsSet;
  ggWalkerT<tBoxItemsSet::const_iterator> vBoxItemsWalker(mBoxItems->GetBoxItems());
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    ggClassyClass* vClass = vBoxItem->GetClass();
    if (vClass != nullptr) {

      // loop over base classes
      ggWalkerT<ggStringSet::const_iterator> vBaseClassNamesWalker(vClass->mBaseNames);
      while (vBaseClassNamesWalker) {

        // lines between derived class and base class
        const QString& vBaseClassName = *vBaseClassNamesWalker;
        ggGraphicsAutoConnectPathItem* vAutoPath = new ggGraphicsAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eTriangle, 12.0f, ggDecoration::cFill::eSolid);
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
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eDiamond, 20.0f, ggDecoration::cFill::eEmpty);
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
























