#include "ggClassyGraphicsScene.h"

#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyGraphicsPathItem.h"


#include <math.h>
#include <set>
#include <QVector2D>
#include <QCursor>
#include <QGraphicsSceneWheelEvent>
#include "ggSubjectT.h"
#include "ggSubjectValueT.h"
#include "ggSubjectConnectionPoint.h"
#include "ggGraphicsConnectionPointItem.h"
#include "ggDecoration.h"
#include "ggGraphicsDecoratedPathItem.h"
#include "ggGraphicsAutoConnectPathItem.h"




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
  vPath1->SetDecorationSrc(ggDecoration::cType::eRhomb, 20.0f, false);
  vPath1->SetDecorationDst(ggDecoration::cType::eCircle, 10.0f, false);
  ggGraphicsAutoConnectPathItem* vPath2 = new ggGraphicsAutoConnectPathItem();
  vPath2->InsertPointSrc(vPointC->GetSubjectConnectionPoint());
  vPath2->InsertPointSrc(vPointD->GetSubjectConnectionPoint());
  vPath2->InsertPointDst(vPointA->GetSubjectConnectionPoint());
  vPath2->InsertPointDst(vPointB->GetSubjectConnectionPoint());
  vPath2->SetDecorationSrc(ggDecoration::cType::eArrow, 13.0f, false);
  vPath2->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f, false);
  QGraphicsScene::addItem(vPointA);
  QGraphicsScene::addItem(vPointB);
  QGraphicsScene::addItem(vPointC);
  QGraphicsScene::addItem(vPointD);
  QGraphicsScene::addItem(vPath1);
  QGraphicsScene::addItem(vPath2);


  // only notify box items change, when all boxes are added
  ggBehaviorLazy::Executor vLazy(mBoxItems);

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
  // only notify box items, when all are added
  ggBehaviorLazy::Executor vLazy(mBoxItems);

  // loop over classes
  typedef std::set<ggClassyClass*> tClasses;
  ggWalkerT<tClasses::const_iterator> vClassesWalker(aDataSet->mClasses);
  while (vClassesWalker) {
    const ggClassyClass* vClass = *vClassesWalker;

    // loop over base classes
    ggWalkerT<ggStringSet::const_iterator> vBaseClassNameWalker(vClass->mBaseNames);
    while (vBaseClassNameWalker) {

      // lines between derived class and base class(es)
      const QString& vClassName = vClass->mName;
      const QString& vBaseClassNames = *vBaseClassNameWalker;
      ggClassyGraphicsPathItem* vLine = new ggClassyGraphicsPathItem();
      vLine->SetBoxItems(mBoxItems);
      vLine->SetConnection(vClassName, vBaseClassNames);
      QGraphicsScene::addItem(vLine);
    }
  }
}
