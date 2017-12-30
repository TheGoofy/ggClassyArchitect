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






#include <QTextDocument>
#include <QTextOption>
#include <QGraphicsSceneMouseEvent>
#include "BaseGraphics/ggGraphicsRoundedRectItem.h"
#include "BaseGraphics/ggGraphicsShadowRectItem.h"
#include "BaseGraphics/ggGraphicsTextItem.h"

class ggClassyGraphicsFrameItem :
  public ggGraphicsRoundedRectItem,
  public ggObserver
{
public:

  ggClassyGraphicsFrameItem(QGraphicsItem* aParent = nullptr) :
    ggGraphicsRoundedRectItem(aParent),
    mShadow(nullptr)
  {
    Construct();
  }

  ggClassyGraphicsFrameItem(const QRectF& aRect,
                            const qreal aRadius = 3.0f,
                            QGraphicsItem* aParent = nullptr) :
    ggGraphicsRoundedRectItem(aRect, aRadius, aParent)
  {
    Construct();
  }

  void SetRadius(qreal aRadius)
  {
    ggGraphicsRoundedRectItem::SetRadius(aRadius, aRadius);
    UpdateLayout();
  }

  void SetRadius(qreal aRadiusX, qreal aRadiusY)
  {
    ggGraphicsRoundedRectItem::SetRadius(aRadiusX, aRadiusY);
    UpdateLayout();
  }

  void setPen(const QPen& aPen)
  {
    ggGraphicsRoundedRectItem::setPen(aPen);
    UpdateLayout();
  }

protected:

  virtual void Update(const ggSubject* aSubject) override
  {
    if (aSubject == mText->GetSubjectText()) {
      UpdateLayout();
    }
  }

  virtual QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) override
  {
    if (aChange == ItemSelectedHasChanged) {
      mShadow->SetShadowColors(aValue.toBool() ? QColor(200, 255, 0, 255) : QColor(0, 0, 0, 40));
      mShadow->setPos(aValue.toBool() ? QPointF(0.0f, 0.0f) : QPointF(3.0f, 2.0f));
    }
    return ggGraphicsRoundedRectItem::itemChange(aChange, aValue);
  }

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override
  {
    // make it editable, if user clicks twice to the same position
    QPointF vPos = aEvent->pos();
    if (vPos == mLastMousePressPos) mText->SetEditable(true);
    mLastMousePressPos = vPos;

    // do inherited event handling
    ggGraphicsRoundedRectItem::mousePressEvent(aEvent);
  }

private:

  void Construct()
  {
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setCursor(Qt::SizeAllCursor);
    mShadow = new ggGraphicsShadowRectItem(this);
    mShadow->setFlag(ItemStacksBehindParent);
    mShadow->setPen(Qt::NoPen);
    mShadow->SetShadowColors(QColor(0, 0, 0, 40));
    mShadow->setPos(3.0f, 2.0f);
    mText = new ggGraphicsTextItem(this);
    mText->SetPen(Qt::NoPen);
    mText->SetText("Goofy was here!");
    mText->SetAlignment(Qt::AlignLeft | Qt::AlignBottom);
    Attach(mText->GetSubjectText());
    UpdateLayout();
  }

  Qt::Alignment GetVerticalAlignment() const
  {
    if (mText->GetAlignment() & Qt::AlignTop) return Qt::AlignTop;
    if (mText->GetAlignment() & Qt::AlignVCenter) return Qt::AlignVCenter;
    if (mText->GetAlignment() & Qt::AlignBottom) return Qt::AlignBottom;
    return Qt::AlignTop;
  }

  void UpdateLayout()
  {
    // adjust the shadow
    qreal vShadowWidth = 7.0f;
    qreal vFrameBorder2 = pen().widthF() / 2.0f;
    qreal vShadowMargin = vFrameBorder2 + vShadowWidth;
    mShadow->SetShadowWidth(vShadowWidth);
    mShadow->SetRadius(GetRadiusX() + vFrameBorder2 + vShadowWidth);
    mShadow->setRect(rect() + QMarginsF(vShadowMargin, vShadowMargin, vShadowMargin, vShadowMargin));

    // adjust the text
    qreal vTextMargin = mText->document()->documentMargin();
    qreal vTextBorder = (GetRadiusX() > vTextMargin + 3.0f) ? GetRadiusX() - vTextMargin - 3.0f: 0.0f;
    qreal vTextWidth = rect().width() - 2.0f * vTextBorder;
    mText->setTextWidth(vTextWidth);
    qreal vTextHeight = mText->boundingRect().height();
    mText->setX(rect().left() + vTextBorder);
    switch (GetVerticalAlignment()) {
      case Qt::AlignTop: mText->setY(rect().top() + vTextBorder); break;
      case Qt::AlignVCenter: mText->setY(rect().top() + (rect().height() - vTextHeight) / 2.0f); break;
      case Qt::AlignBottom: mText->setY(rect().bottom() - vTextHeight - vTextBorder); break;
      default: mText->setY(rect().top() + vTextBorder); break;
    }
  }

  ggGraphicsShadowRectItem* mShadow;
  ggGraphicsTextItem* mText;
  QPointF mLastMousePressPos;

};












ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mDataSet(nullptr),
  mBoxPoints(nullptr)
{
  setBackgroundBrush(Qt::gray);
  mBoxPoints = new ggClassyClassBoxPoints();


  ggClassyGraphicsFrameItem* vRect = new ggClassyGraphicsFrameItem(QRectF(-200,-100,300,200), 15);
  vRect->setPen(QPen(QColor(0,0,0,50), 1.0f));
  vRect->setBrush(QColor(255,255,255,255));
  addItem(vRect);
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
        ggClassyAutoConnectPathItem* vAutoPath = new ggClassyAutoConnectPathItem();
        vAutoPath->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
        vAutoPath->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f, ggDecoration::cFill::eSolid);
        vAutoPath->InsertPointSrc(vBoxItem->GetClassConnectionTop());
        vAutoPath->InsertPointsDst(mBoxPoints->GetClassPointsBottom(vBaseClassName));
        vAutoPath->SetCollection(mDataSet->FindCollectionFromClass(vBaseClassName));
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
