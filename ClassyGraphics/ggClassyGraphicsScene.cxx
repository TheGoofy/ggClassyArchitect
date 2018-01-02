// 0) include own header
#include "ggClassyGraphicsScene.h"

// 1) include system or QT
#include <QGraphicsItem>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseGraphics/ggGraphicsConnectionPointItem.h"
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyGraphics/ggClassyClassBoxPoints.h"
#include "ClassyGraphics/ggClassyAutoConnectPathItem.h"
#include "ClassyGraphics/ggClassyGraphicsFrameItem.h"


ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mDataSet(nullptr),
  mBoxPoints(nullptr)
{
  setBackgroundBrush(Qt::gray);
  mBoxPoints = new ggClassyClassBoxPoints();

  ggClassyGraphicsFrameItem* vRectA = new ggClassyGraphicsFrameItem(QRectF(-200,-100,300,200), 10);
  vRectA->setPen(QPen(QColor(0,0,0,50), 1.0f));
  vRectA->setBrush(QColor(255,255,255,255));
  addItem(vRectA);

  ggClassyGraphicsFrameItem* vRectB = new ggClassyGraphicsFrameItem(QRectF(-150,-200,200,100), 10);
  vRectB->setPen(QPen(QColor(0,0,0,50), 1.0f));
  vRectB->setBrush(QColor(255,255,255,255));
  addItem(vRectB);
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
    DeleteClassBoxAndConnectionItems();
    CreateClassBoxItems();
    CreateConnectionItems();
  }

  else if (aSubject == mDataSet->GetSubjectConnections()) {
    DeleteConnectionItems();
    mBoxPoints->Refresh();
    CreateConnectionItems();
  }

  if (aSubject == &mDataSet->GetFrames()) {
    DeleteFrameItems();
    CreateFrameItems();
  }
}


void ggClassyGraphicsScene::SetDataSet(ggClassyDataSet* aDataSet)
{
  // nothing to do it unchaged dataset
  if (aDataSet == mDataSet) return;

  // (re)connect subject
  if (mDataSet != nullptr) {
    Detach(&mDataSet->GetClassBoxes());
    Detach(&mDataSet->GetFrames());
    Detach(mDataSet->GetSubjectConnections());
  }
  mDataSet = aDataSet;
  if (mDataSet != nullptr) {
    Attach(&mDataSet->GetClassBoxes());
    Attach(&mDataSet->GetFrames());
    Attach(mDataSet->GetSubjectConnections());
  }

  // update all items
  DeleteClassBoxAndConnectionItems();
  CreateClassBoxItems();
  CreateConnectionItems();
  DeleteFrameItems();
  CreateFrameItems();
}



ggClassyGraphicsScene::tCollections ggClassyGraphicsScene::GetSelectedCollections() const
{
  tCollections vSelectedCollections;
  foreach (QGraphicsItem* vItem, selectedItems()) {
    ggClassyGraphicsBoxItem* vClassBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(vItem);
    if (vClassBoxItem != nullptr) {
      const ggClassyCollection* vCollection = vClassBoxItem->GetCollection();
      if (vCollection != nullptr) {
        vSelectedCollections.insert(vCollection);
      }
    }
  }
  return vSelectedCollections;
}


ggClassyGraphicsScene::tClasses ggClassyGraphicsScene::GetSelectedClasses() const
{
  tClasses vSelectedClasses;
  foreach (QGraphicsItem* vItem, selectedItems()) {
    ggClassyGraphicsBoxItem* vClassBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(vItem);
    if (vClassBoxItem != nullptr) {
      const ggClassyClass* vClass = vClassBoxItem->GetClass();
      if (vClass != nullptr) {
        vSelectedClasses.insert(vClass);
      }
    }
  }
  return vSelectedClasses;
}


void ggClassyGraphicsScene::SelectClasses(const tClasses& aClasses)
{
  foreach (QGraphicsItem* vItem, items()) {
    ggClassyGraphicsBoxItem* vClassBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(vItem);
    if (vClassBoxItem != nullptr) {
      const ggClassyClass* vClass = vClassBoxItem->GetClass();
      if (aClasses.find(vClass) != aClasses.end()) {
        if (vClass != nullptr) vClassBoxItem->setSelected(true);
      }
    }
  }
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


ggClassyGraphicsScene::tConnections ggClassyGraphicsScene::GetSelectedConnections() const
{
  tConnections vSelectedConnections;
  foreach (QGraphicsItem* vItem, selectedItems()) {
    ggClassyAutoConnectPathItem* vConnectionItem = dynamic_cast<ggClassyAutoConnectPathItem*>(vItem);
    if (vConnectionItem != nullptr) {
      vSelectedConnections.insert(vConnectionItem);
    }
  }
  return vSelectedConnections;
}


void ggClassyGraphicsScene::SelectConnections(const tConnections& aConnections)
{
  foreach (QGraphicsItem* vItem, items()) {
    ggClassyAutoConnectPathItem* vConnectionItem = dynamic_cast<ggClassyAutoConnectPathItem*>(vItem);
    if (vConnectionItem != nullptr) {
      if (aConnections.find(vConnectionItem) != aConnections.end()) {
        vConnectionItem->setSelected(true);
      }
    }
  }
}


ggClassyGraphicsScene::tFrames ggClassyGraphicsScene::GetSelectedFrames() const
{
  tFrames vSelectedFrames;
  foreach (QGraphicsItem* vItem, selectedItems()) {
    ggClassyGraphicsFrameItem* vFrameItem = dynamic_cast<ggClassyGraphicsFrameItem*>(vItem);
    if (vFrameItem != nullptr) {
      vSelectedFrames.insert(vFrameItem->GetFrame());
    }
  }
  return vSelectedFrames;
}


void ggClassyGraphicsScene::SelectFrames(const tFrames& aFrames)
{
  foreach (QGraphicsItem* vItem, items()) {
    ggClassyGraphicsFrameItem* vFrameItem = dynamic_cast<ggClassyGraphicsFrameItem*>(vItem);
    if (vFrameItem != nullptr) {
      const ggClassyFrame* vFrame = vFrameItem->GetFrame();
      if (aFrames.find(vFrame) != aFrames.end()) {
        vFrameItem->setSelected(true);
      }
    }
  }
}


void ggClassyGraphicsScene::MoveSelectedItemsUp()
{
  if (mDataSet != nullptr) {
    // class boxes
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesUp(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
    // frames
    const tFrames& vFrames = GetSelectedFrames();
    mDataSet->GetFrames().MoveFramesUp(vFrames);
    SelectFrames(vFrames);
  }
}


void ggClassyGraphicsScene::MoveSelectedItemsDown()
{
  if (mDataSet != nullptr) {
    // class boxes
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesDown(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
    // frames
    const tFrames& vFrames = GetSelectedFrames();
    mDataSet->GetFrames().MoveFramesDown(vFrames);
    SelectFrames(vFrames);
  }
}


void ggClassyGraphicsScene::MoveSelectedItemsTop()
{
  if (mDataSet != nullptr) {
    // class boxes
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesTop(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
    // frames
    const tFrames& vFrames = GetSelectedFrames();
    mDataSet->GetFrames().MoveFramesTop(vFrames);
    SelectFrames(vFrames);
  }
}


void ggClassyGraphicsScene::MoveSelectedItemsBottom()
{
  if (mDataSet != nullptr) {
    // class boxes
    const tClassBoxes& vClassBoxes = GetSelectedClassBoxes();
    mDataSet->GetClassBoxes().MoveClassBoxesBottom(vClassBoxes);
    SelectClassBoxes(vClassBoxes);
    // frames
    const tFrames& vFrames = GetSelectedFrames();
    mDataSet->GetFrames().MoveFramesBottom(vFrames);
    SelectFrames(vFrames);
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


void ggClassyGraphicsScene::DeleteClassBoxAndConnectionItems()
{
  // collect items for removal (don't change the list of items, while iterating over it)
  std::vector<QGraphicsItem*> vItemsToDelete;
  foreach (QGraphicsItem* vItem, items()) {
    // if box-items are deleted, the connection-items need to be deleted too
    if (dynamic_cast<ggClassyGraphicsBoxItem*>(vItem) != nullptr ||
        dynamic_cast<ggClassyAutoConnectPathItem*>(vItem) != nullptr) {
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
    if (dynamic_cast<ggClassyAutoConnectPathItem*>(vItem) != nullptr) {
      vItemsToDelete.push_back(vItem);
    }
  }

  // now remove and delete items
  DeleteItems(vItemsToDelete);
}


void ggClassyGraphicsScene::DeleteFrameItems()
{
  // collect items for removal (don't change the list of items, while iterating over it)
  std::vector<QGraphicsItem*> vItemsToDelete;
  foreach (QGraphicsItem* vItem, items()) {
    if (dynamic_cast<ggClassyGraphicsFrameItem*>(vItem) != nullptr) {
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
    ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesWalker(mDataSet->GetClassBoxes());
    while (vClassBoxesWalker) {
      ggClassyClassBox* vClassBox = *vClassBoxesWalker;
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
        ggClassyAutoConnectPathItem* vAutoPath = new ggClassyAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f, ggDecoration::cFill::eSolid);
        vAutoPath->InsertPointSrc(vBoxItem->GetClassConnectionTop());
        vAutoPath->InsertPointsDst(mBoxPoints->GetClassPointsBottom(vBaseClassName));
        vAutoPath->SetCollection(mDataSet->FindCollectionFromClass(vBaseClassName));
        vAutoPath->SetClassInfo(vClass->GetName(), vBaseClassName);
        QGraphicsScene::addItem(vAutoPath);
      }

      // loop over members
      const ggClassyClass::tMembers& vMembers = vClass->GetMembers();
      for (ggUSize vMemberIndex = 0; vMemberIndex < vMembers.size(); vMemberIndex++) {

        // connect member with class
        const QString& vMemberClassName = vMembers[vMemberIndex].GetClassName();
        ggClassyAutoConnectPathItem* vAutoPath = new ggClassyAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eDiamond, 15.0f, ggDecoration::cFill::eEmpty);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionLeft(vMemberIndex));
        vAutoPath->InsertPointSrc(vBoxItem->GetMemberConnectionRight(vMemberIndex));
        vAutoPath->InsertPointsDst(mBoxPoints->GetClassPointsLeftRight(vMemberClassName));
        vAutoPath->SetCollection(mDataSet->FindCollectionFromClass(vMemberClassName));
        vAutoPath->SetClassInfo(vClass->GetName(), vMemberClassName, vMemberIndex);
        QPen vPen(vAutoPath->pen());
        vPen.setStyle(Qt::DashLine);
        vAutoPath->setPen(vPen);
        QGraphicsScene::addItem(vAutoPath);
      }
    }
  }
}


void ggClassyGraphicsScene::CreateFrameItems()
{
  // loop over frame items
  if (mDataSet != nullptr) {
    ggWalkerT<ggClassyFrameContainer::iterator> vFramesWalker(mDataSet->GetFrames());
    while (vFramesWalker) {
      ggClassyFrame* vFrame = *vFramesWalker;
      ggClassyGraphicsFrameItem* vFrameItem = new ggClassyGraphicsFrameItem(vFrame);
      QGraphicsScene::addItem(vFrameItem);
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
    ggClassyAutoConnectPathItem* vPath = new ggClassyAutoConnectPathItem();
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
