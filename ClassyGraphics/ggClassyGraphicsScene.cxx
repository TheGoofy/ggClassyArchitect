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
#include "ClassyGraphics/ggClassyClassBoxPoints.h"


ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mDataSet(nullptr),
  mBoxPoints(nullptr)
{
  mBoxPoints = new ggClassyClassBoxPoints();
}


ggClassyGraphicsScene::~ggClassyGraphicsScene()
{
  delete mBoxPoints;
}


void ggClassyGraphicsScene::Update(const ggSubject* aSubject)
{
  // nothing to do, if there is no dataset
  if (mDataSet == nullptr) return;

  if (aSubject == &mDataSet->GetClassBoxes()) {
    DeleteClassBoxItems();
    CreateClassBoxItems();
    CreateConnectionItems();
  }

  else if (aSubject == mDataSet->GetSubjectConnections()) {
    DeleteConnectionItems();
    mBoxPoints->Refresh();
    CreateConnectionItems();
  }
}


void ggClassyGraphicsScene::SetDataSet(ggClassyDataSet* aDataSet)
{
  // nothing to do it unchaged dataset
  if (aDataSet == mDataSet) return;

  // (re)connect subject
  if (mDataSet != nullptr) {
    Detach(&mDataSet->GetClassBoxes());
    Detach(mDataSet->GetSubjectConnections());
  }
  mDataSet = aDataSet;
  if (mDataSet != nullptr) {
    Attach(&mDataSet->GetClassBoxes());
    Attach(mDataSet->GetSubjectConnections());
  }

  // update all items
  DeleteClassBoxItems();
  CreateClassBoxItems();
  CreateConnectionItems();
}


ggClassyGraphicsScene::tClassBoxes ggClassyGraphicsScene::GetSelectedClassBoxes() const
{
  tClassBoxes vSelectedClassBoxes;
  foreach (QGraphicsItem* vItem, selectedItems()) {
    ggClassyGraphicsBoxItem* vClassBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(vItem);
    if (vClassBoxItem != nullptr) {
      vSelectedClassBoxes.insert(vClassBoxItem->GetClassBox());
    }
  }
  return vSelectedClassBoxes;
}


void ggClassyGraphicsScene::SelectClassBoxes(const tClassBoxes& aClassBoxes)
{
  foreach (QGraphicsItem* vItem, items()) {
    ggClassyGraphicsBoxItem* vClassBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(vItem);
    if (vClassBoxItem != nullptr) {
      const ggClassyClassBox* vClassBox = vClassBoxItem->GetClassBox();
      if (aClassBoxes.find(vClassBox) != aClassBoxes.end()) {
        vClassBoxItem->setSelected(true);
      }
    }
  }
}


void ggClassyGraphicsScene::MoveSelectedClassBoxesUp()
{
  if (mDataSet != nullptr) {
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesUp(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
  }
}


void ggClassyGraphicsScene::MoveSelectedClassBoxesDown()
{
  if (mDataSet != nullptr) {
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesDown(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
  }
}


void ggClassyGraphicsScene::MoveSelectedClassBoxesTop()
{
  if (mDataSet != nullptr) {
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesTop(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
  }
}


void ggClassyGraphicsScene::MoveSelectedClassBoxesBottom()
{
  if (mDataSet != nullptr) {
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesBottom(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
  }
}


void ggClassyGraphicsScene::DeleteItems(const std::vector<QGraphicsItem*>& aItems)
{
  ggWalkerT<std::vector<QGraphicsItem*>::const_iterator> vItemsWalker(aItems);
  while (vItemsWalker) {
    QGraphicsItem* vItem = *vItemsWalker;
    removeItem(vItem);
    if (vItem->parentItem() != nullptr) qDebug() << __PRETTY_FUNCTION__ << "parent item detected" << vItem;
    if (vItem->parentObject() != nullptr) qDebug() << __PRETTY_FUNCTION__ << "parent object detected" << vItem;
    delete vItem;
  }
}


void ggClassyGraphicsScene::DeleteClassBoxItems()
{
  // collect items for removal (don't change the list of items, while iterating over it)
  std::vector<QGraphicsItem*> vItemsToDelete;
  foreach (QGraphicsItem* vItem, items()) {
    // if box-items are deleted, the connection-items need to be deleted too
    if (dynamic_cast<ggClassyGraphicsBoxItem*>(vItem) != nullptr ||
        dynamic_cast<ggGraphicsAutoConnectPathItem*>(vItem) != nullptr) {
      vItemsToDelete.push_back(vItem);
    }
  }

  // clear the box-items map
  mBoxPoints->Clear();

  // now remove and delete items
  DeleteItems(vItemsToDelete);
}


void ggClassyGraphicsScene::DeleteConnectionItems()
{
  // collect items for removal (don't change the list of items, while iterating over it)
  std::vector<QGraphicsItem*> vItemsToDelete;
  foreach (QGraphicsItem* vItem, items()) {
    if (dynamic_cast<ggGraphicsAutoConnectPathItem*>(vItem) != nullptr) {
      vItemsToDelete.push_back(vItem);
    }
  }

  // now remove and delete items
  DeleteItems(vItemsToDelete);
}


void ggClassyGraphicsScene::CreateClassBoxItems()
{
  // loop over box items
  if (mDataSet != nullptr) {
    ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesIterator(mDataSet->GetClassBoxes());
    while (vClassBoxesIterator) {
      ggClassyClassBox* vClassBox = *vClassBoxesIterator;
      ggClassyClass* vClass = mDataSet->FindClass(vClassBox->GetClassName());
      ggClassyGraphicsBoxItem* vBoxItem = new ggClassyGraphicsBoxItem(vClass, vClassBox);
      mBoxPoints->AddBoxItem(vBoxItem);
      QGraphicsScene::addItem(vBoxItem);
    }
  }
}


void ggClassyGraphicsScene::CreateConnectionItems()
{
  // loop over class box items
  typedef ggClassyClassBoxPoints::tBoxItemsSet tBoxItemsSet;
  ggWalkerT<tBoxItemsSet::const_iterator> vBoxItemsWalker(mBoxPoints->GetBoxItems());
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    ggClassyClass* vClass = vBoxItem->GetClass();
    if (vClass != nullptr) {

      // loop over base classes
      ggWalkerT<ggStringSet::const_iterator> vBaseClassNamesWalker(vClass->GetBaseClassNames());
      while (vBaseClassNamesWalker) {

        // lines between derived class and base class
        const QString& vBaseClassName = *vBaseClassNamesWalker;
        ggGraphicsAutoConnectPathItem* vAutoPath = new ggGraphicsAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f, ggDecoration::cFill::eSolid);
        vAutoPath->InsertPointSrc(vBoxItem->GetClassConnectionTop());
        vAutoPath->InsertPointsDst(mBoxPoints->GetClassPointsBottom(vBaseClassName));
        QGraphicsScene::addItem(vAutoPath);
      }

      // loop over members
      const ggClassyClass::tMembers& vMembers = vClass->GetMembers();
      for (ggUSize vMemberIndex = 0; vMemberIndex < vMembers.size(); vMemberIndex++) {

        // connect member with class
        const QString& vMemberClassName = vMembers[vMemberIndex].GetClassName();
        ggGraphicsAutoConnectPathItem* vAutoPath = new ggGraphicsAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eDiamond, 15.0f, ggDecoration::cFill::eEmpty);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionLeft(vMemberIndex));
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionRight(vMemberIndex));
        vAutoPath->InsertPointsDst(mBoxPoints->GetClassPointsLeftRight(vMemberClassName));
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
