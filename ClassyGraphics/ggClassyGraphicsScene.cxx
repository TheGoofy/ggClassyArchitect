// 0) include own header
#include "ggClassyGraphicsScene.h"

// 1) include system or QT
#include <QGraphicsItem>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseGraphics/ggGraphicsAutoConnectPathItem.h"
#include "BaseGraphics/ggGraphicsConnectionPointItem.h"
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyGraphics/ggClassyGraphicsBoxItems.h"


ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mDataSet(nullptr),
  mBoxItems(nullptr)
{
  mBoxItems = new ggClassyGraphicsBoxItems();
}


ggClassyGraphicsScene::~ggClassyGraphicsScene()
{
  delete mBoxItems;
}


void ggClassyGraphicsScene::Update(const ggSubject* aSubject)
{
  if (aSubject == mDataSet) {
    ClearAll();
    UpdateBoxItems();
    UpdateLineItems();
  }
}


void ggClassyGraphicsScene::AddItem(ggClassyGraphicsBoxItem* aBoxItem)
{
  mBoxItems->AddItem(aBoxItem);
  QGraphicsScene::addItem(aBoxItem);
}


void ggClassyGraphicsScene::SetDataSet(ggClassyDataSet* aDataSet)
{
  // (re)connect subject
  Detach(mDataSet);
  mDataSet = aDataSet;
  Attach(mDataSet);

  // update all items
  ClearAll();
  UpdateBoxItems();
  UpdateLineItems();

  // goofy: development & testing ...
  AddTestConnections();
}


void ggClassyGraphicsScene::ClearAll()
{
  // collect items for removal (don't change the list of items, while iterating over it)
  std::vector<QGraphicsItem*> vItemsToRemove;
  ggWalkerT<QList<QGraphicsItem*>::const_iterator> vItemsWalker(items());
  while (vItemsWalker) {
    QGraphicsItem* vItem = *vItemsWalker;
    if (dynamic_cast<ggClassyGraphicsBoxItem*>(vItem) != nullptr ||
        dynamic_cast<ggGraphicsAutoConnectPathItem*>(vItem) != nullptr) {
      vItemsToRemove.push_back(vItem);
    }
  }

  // now remove and delete them
  ggWalkerT<std::vector<QGraphicsItem*>::iterator> vItemsToRemoveWalker(vItemsToRemove);
  while (vItemsToRemoveWalker) {
    QGraphicsItem* vItem = *vItemsToRemoveWalker;
    removeItem(vItem);
    if (vItem->parentItem() != nullptr) qDebug() << __PRETTY_FUNCTION__ << "parent item detected" << vItem;
    if (vItem->parentObject() != nullptr) qDebug() << __PRETTY_FUNCTION__ << "parent object detected" << vItem;
    delete vItem;
  }

  // clear the box-items map
  mBoxItems->Clear();
}


void ggClassyGraphicsScene::UpdateBoxItems()
{
  if (mDataSet != nullptr) {

    // only notify box items change, when all boxes are added
    ggBehaviorLazy::cExecutor vLazy(mBoxItems);

    // loop over box items
    typedef std::vector<ggClassyClassBox*> tClassBoxes;
    ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(mDataSet->mClassBoxes);
    while (vClassBoxesIterator) {
      ggClassyClassBox* vClassBox = *vClassBoxesIterator;
      ggClassyClass* vClass = mDataSet->FindClass(vClassBox->mClassName);
      AddItem(new ggClassyGraphicsBoxItem(vClass, vClassBox));
    }
  }
}


void ggClassyGraphicsScene::UpdateLineItems()
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
