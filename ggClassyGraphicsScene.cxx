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



class ggConnectionPoint
{
public:

  ggConnectionPoint() :
    mPosition(0.0f, 0.0f),
    mDirection(1.0f, 0.0f) {
  }

  float GetDirectionAngle() const {
    if (mDirection.isNull()) return 0.0f;
    return atan2(mDirection.y(), mDirection.x());
  }

  void SetDirectionAngle(float aAngle) {
    mDirection.setX(cos(aAngle));
    mDirection.setY(sin(aAngle));
  }

  QPointF GetControlPoint(float aDistance) const {
    QPointF vControlPoint(mPosition);
    vControlPoint += (aDistance * mDirection).toPointF();
    return vControlPoint;
  }

  QPointF mPosition;
  QVector2D mDirection;

};



class ggSubjectConnectionPoint :
    public ggSubjectValueT<ggConnectionPoint>
{
public:

  bool mVisible;

};



class ggGraphicsConnectionPointItem :
    public QGraphicsEllipseItem,
    private ggObserver
{
public:

  ggGraphicsConnectionPointItem(float aSize = 11.0f, QGraphicsItem* aParent = nullptr) :
    QGraphicsEllipseItem(QRect(-aSize/2,-aSize/2,aSize,aSize), aParent),
    mDirectionLine(nullptr)
  {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIgnoresTransformations);
    setAcceptHoverEvents(true);
    setCursor(Qt::DragMoveCursor);
    setToolTip("Click and drag.");
    setZValue(1.0f);
    setPen(Qt::NoPen);
    setBrush(Qt::red);

    QPen vPen(Qt::DotLine);
    vPen.setWidthF(0.8f);
    vPen.setCapStyle(Qt::RoundCap);
    mDirectionLine = new QGraphicsLineItem(this);
    mDirectionLine->setPen(vPen);

    Attach(&mConnectionPoint);

    UpdateConnectionPoint();
  }

  void SetSize(float aSize)
  {
    setRect(-aSize/2, -aSize/2, aSize, aSize);
  }

  float GetSize() const
  {
    return rect().width();
  }

  void SetConnectionPointPosition(const QPointF& aPosition)
  {
    mConnectionPoint.mPosition = aPosition;
    mConnectionPoint.Notify();
  }

  void SetConnectionPointDirection(float aAngle)
  {
    mConnectionPoint.mDirection = QVector2D(sin(aAngle), cos(aAngle));
    mConnectionPoint.Notify();
  }

  void SetConnectionPointDirection(const QVector2D& aDirection)
  {
    if (aDirection.length() != 0.0f) {
      mConnectionPoint.mDirection = aDirection.normalized();
      mConnectionPoint.Notify();
    }
  }

  const ggSubjectConnectionPoint& ConnectionPoint() const
  {
    return mConnectionPoint;
  }

protected:

  virtual void Update(const ggSubject* aSubject) override
  {
    if (aSubject == &mConnectionPoint) {
      UpdateConnectionPoint();
    }
  }

  virtual QVariant itemChange(GraphicsItemChange aChange,
                              const QVariant& aValue) override
  {
    // notify position change
    if (aChange == ItemPositionHasChanged) {
      mConnectionPoint.mPosition = aValue.toPointF();
      mConnectionPoint.Notify(this);
    }
    // execute base item change
    return QGraphicsEllipseItem::itemChange(aChange, aValue);
  }

  virtual void wheelEvent(QGraphicsSceneWheelEvent* aEvent) override
  {
    float vAngle = mConnectionPoint.GetDirectionAngle();
    vAngle -= 0.001f * aEvent->delta();
    mConnectionPoint.SetDirectionAngle(vAngle);
    mConnectionPoint.Notify();
  }

private:

  void UpdateConnectionPoint()
  {
    // adjust position
    setPos(mConnectionPoint.mPosition);

    // adjust direction indicator line
    QPointF vPointA(0.0f, 0.0f);
    QPointF vPointB = (5.0f * GetSize() * mConnectionPoint.mDirection).toPointF();
    mDirectionLine->setLine(QLineF(vPointA, vPointB));
  }

  ggSubjectConnectionPoint mConnectionPoint;
  QGraphicsLineItem* mDirectionLine;

};




class ggDecoration
{
public:

  enum class cType {
    eLine,
    eArrow,
    eTriangle,
    eRhomb,
    eCircle
  };

  cType mType;
  float mLength;
  bool mSolid;

};




class ggGraphicsPathItem :
    public QGraphicsPathItem
{
public:

  ggGraphicsPathItem(QGraphicsItem* aParent = nullptr) :
    QGraphicsPathItem(aParent),
    mDecorationItemSrc(nullptr),
    mDecorationItemDst(nullptr)
  {
    QPen vPen(pen());
    vPen.setStyle(Qt::DashLine);
    vPen.setCapStyle(Qt::FlatCap);
    vPen.setJoinStyle(Qt::MiterJoin);
    vPen.setColor(Qt::blue);
    vPen.setWidthF(1.5f);
    setPen(vPen);
    setBrush(Qt::NoBrush);
  }

  void ClearConnection()
  {
    setPath(QPainterPath());
    delete mDecorationItemSrc;
    delete mDecorationItemDst;
    mDecorationItemSrc = nullptr;
    mDecorationItemDst = nullptr;
  }

  void SetConnection(const ggConnectionPoint& aPointSrc,
                     const ggConnectionPoint& aPointDst)
  {
    mPointSrc = aPointSrc;
    mPointDst = aPointDst;
    RebuildPath();
  }

  const ggDecoration& DecorationSrc() const
  {
    return mDecorationSrc;
  }

  const ggDecoration& DecorationDst() const
  {
    return mDecorationDst;
  }

  void SetDecorationSrc(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true)
  {
    mDecorationSrc.mType = aType;
    mDecorationSrc.mLength = aLength;
    mDecorationSrc.mSolid = aSolid;
    RebuildPath();
  }

  void SetDecorationDst(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true)
  {
    mDecorationDst.mType = aType;
    mDecorationDst.mLength = aLength;
    mDecorationDst.mSolid = aSolid;
    RebuildPath();
  }

private:

  bool NeedExtraPath(const ggDecoration& aDecoration) const
  {
    if (aDecoration.mSolid) {
      return (aDecoration.mType == ggDecoration::cType::eCircle) ||
             (aDecoration.mType == ggDecoration::cType::eRhomb);
    }
    return false;
  }

  QPainterPath GetPath(const ggConnectionPoint& aPointSrc,
                       const ggConnectionPoint& aPointDst) const
  {
    // some decorations need a bit a longer path
    bool vExtraSrcPath = NeedExtraPath(mDecorationSrc);
    bool vExtraDstPath = NeedExtraPath(mDecorationDst);

    // calculate the key points
    QPointF vPointSrcBase = aPointSrc.GetControlPoint(mDecorationSrc.mLength);
    QPointF vPointDstBase = aPointDst.GetControlPoint(mDecorationDst.mLength);
    QPointF vPointSrcExtraBase = vExtraSrcPath ? aPointSrc.GetControlPoint(mDecorationSrc.mLength/2.0f) : vPointSrcBase;
    QPointF vPointDstExtraBase = vExtraDstPath ? aPointDst.GetControlPoint(mDecorationDst.mLength/2.0f) : vPointDstBase;
    float vDistance = QLineF(vPointSrcBase, vPointDstBase).length();
    QPointF vPointSrcControl = aPointSrc.GetControlPoint(mDecorationSrc.mLength + vDistance/2.5f);
    QPointF vPointDstControl = aPointDst.GetControlPoint(mDecorationDst.mLength + vDistance/2.5f);

    // compile path
    QPainterPath vPath;
    vPath.moveTo(vPointSrcExtraBase);
    vPath.lineTo(vPointSrcBase);
    vPath.cubicTo(vPointSrcControl,
                  vPointDstControl,
                  vPointDstBase);
    vPath.lineTo(vPointDstExtraBase);

    return vPath;
  }

  QGraphicsItem* CreateDecoration(const ggConnectionPoint& aPoint,
                                  const ggDecoration& aDecoration)
  {
    QGraphicsPathItem* vPathItem = new QGraphicsPathItem(this);
    QPainterPath vPath;

    QPen vPen(pen());
    vPen.setStyle(Qt::SolidLine);
    QBrush vBrush(vPen.color());
    if (!aDecoration.mSolid) vBrush = Qt::NoBrush;

    float vLength = aDecoration.mLength;
    float vWidth = 0.5f * vLength;
    QVector2D vPointControl(aPoint.GetControlPoint(vLength));
    QVector2D vNormal(aPoint.mDirection.y(), -aPoint.mDirection.x());

    if (aDecoration.mType == ggDecoration::cType::eLine) {
      vPath.moveTo(aPoint.mPosition);
      vPath.lineTo(vPointControl.toPointF());
      vBrush = Qt::NoBrush;
    }
    else if (aDecoration.mType == ggDecoration::cType::eArrow) {
      QVector2D vPointA(vPointControl + vWidth*vNormal);
      QVector2D vPointB(vPointControl - vWidth*vNormal);
      vPath.moveTo(aPoint.mPosition);
      vPath.lineTo(vPointControl.toPointF());
      vPath.moveTo(vPointA.toPointF());
      vPath.lineTo(aPoint.mPosition);
      vPath.lineTo(vPointB.toPointF());
      vBrush = Qt::NoBrush;
    }
    else if (aDecoration.mType == ggDecoration::cType::eTriangle) {
      QVector2D vPointA(vPointControl + vWidth*vNormal);
      QVector2D vPointB(vPointControl - vWidth*vNormal);
      vPath.moveTo(vPointA.toPointF());
      vPath.lineTo(aPoint.mPosition);
      vPath.lineTo(vPointB.toPointF());
      vPath.closeSubpath();
      if (aDecoration.mSolid) vPen = Qt::NoPen;
    }
    else if (aDecoration.mType == ggDecoration::cType::eRhomb) {
      QVector2D vCenter(aPoint.GetControlPoint(vLength / 2.0f));
      QVector2D vPointA(vCenter + 0.5f*vWidth*vNormal);
      QVector2D vPointB(vCenter - 0.5f*vWidth*vNormal);
      vPath.moveTo(vPointControl.toPointF());
      vPath.lineTo(vPointA.toPointF());
      vPath.lineTo(aPoint.mPosition);
      vPath.lineTo(vPointB.toPointF());
      vPath.closeSubpath();
      if (aDecoration.mSolid) vPen = Qt::NoPen;
    }
    else if (aDecoration.mType == ggDecoration::cType::eCircle) {
      float vRadius = vLength / 2.0f;
      QPointF vCenter(aPoint.GetControlPoint(vRadius));
      vPath.addEllipse(vCenter, vRadius, vRadius);
      if (aDecoration.mSolid) vPen = Qt::NoPen;
    }

    vPathItem->setPath(vPath);
    vPathItem->setPen(vPen);
    vPathItem->setBrush(vBrush);

    return vPathItem;
  }

  void RebuildPath()
  {
    // update path
    setPath(GetPath(mPointSrc, mPointDst));

    // update decoration
    delete mDecorationItemSrc;
    delete mDecorationItemDst;
    mDecorationItemSrc = CreateDecoration(mPointSrc, mDecorationSrc);
    mDecorationItemDst = CreateDecoration(mPointDst, mDecorationDst);
  }

  // the connection points
  ggConnectionPoint mPointSrc;
  ggConnectionPoint mPointDst;

  // connection decoration properties
  ggDecoration mDecorationSrc;
  ggDecoration mDecorationDst;

  // decoraton graph items
  QGraphicsItem* mDecorationItemSrc;
  QGraphicsItem* mDecorationItemDst;

};






class ggGraphicsShortestPathItem :
    public ggGraphicsPathItem,
    private ggObserver
{
public:

  ggGraphicsShortestPathItem(QGraphicsItem* aParent = nullptr) :
    ggGraphicsPathItem(aParent)
  {
  }

  void SetConnection(const ggSubjectConnectionPoint* aPointSrc,
                     const ggSubjectConnectionPoint* aPointDst)
  {
    ClearPointsSrc();
    ClearPointsDst();
    InsertPointSrc(aPointSrc);
    InsertPointDst(aPointDst);
  }

  void ClearPointsSrc()
  {
    Detach(mPointsSrc);
    mPointsSrc.clear();
    RebuildPath();
  }

  void ClearPointsDst()
  {
    Detach(mPointsDst);
    mPointsDst.clear();
    RebuildPath();
  }

  void InsertPointSrc(const ggSubjectConnectionPoint* aPointSrc)
  {
    mPointsSrc.insert(aPointSrc);
    ggObserver::Attach(aPointSrc);
    RebuildPath();
  }

  void InsertPointDst(const ggSubjectConnectionPoint* aPointDst)
  {
    mPointsDst.insert(aPointDst);
    ggObserver::Attach(aPointDst);
    RebuildPath();
  }

  void SetDecorationSrc(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true)
  {
    ggGraphicsPathItem::SetDecorationSrc(aType, aLength, aSolid);
    RebuildPath();
  }

  void SetDecorationDst(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true)
  {
    ggGraphicsPathItem::SetDecorationDst(aType, aLength, aSolid);
    RebuildPath();
  }

protected:

  virtual void Update(const ggSubject* aSubject) override
  {
    const ggSubjectConnectionPoint* vPoint = dynamic_cast<const ggSubjectConnectionPoint*>(aSubject);
    if (vPoint != nullptr) {
      if (mPointsSrc.find(vPoint) != mPointsSrc.end() ||
          mPointsDst.find(vPoint) != mPointsDst.end()) {
        RebuildPath();
      }
    }
  }

private:

  typedef std::set<const ggSubjectConnectionPoint*> tPoints;

  void Attach(const tPoints& aPoints)
  {
    ggWalkerT<tPoints::const_iterator> vPointsWalker(aPoints);
    while (vPointsWalker) ggObserver::Attach(*vPointsWalker);
  }

  void Detach(const tPoints& aPoints)
  {
    ggWalkerT<tPoints::const_iterator> vPointsWalker(aPoints);
    while (vPointsWalker) ggObserver::Detach(*vPointsWalker);
  }

  float GetDistanceCosts(const ggConnectionPoint& aPointSrc,
                         const ggConnectionPoint& aPointDst) const
  {
    QPointF vPointSrcBase = aPointSrc.GetControlPoint(DecorationSrc().mLength);
    QPointF vPointDstBase = aPointDst.GetControlPoint(DecorationDst().mLength);
    float vDistance = QLineF(vPointSrcBase, vPointDstBase).length();
    QPointF vPointSrcControl = aPointSrc.GetControlPoint(DecorationSrc().mLength + vDistance/4.0f);
    QPointF vPointDstControl = aPointDst.GetControlPoint(DecorationDst().mLength + vDistance/4.0f);
    return QLineF(vPointSrcControl, vPointDstControl).length();
  }

  void RebuildPath()
  {
    // connection points with the shortest distance
    float vDistanceCostsMin = std::numeric_limits<float>::max();
    const ggConnectionPoint* vPointSrc = nullptr;
    const ggConnectionPoint* vPointDst = nullptr;

    // loop over all src- and dst-points
    ggWalkerT<tPoints::const_iterator> vPointsSrcWalker(mPointsSrc);
    while (vPointsSrcWalker) {
      ggWalkerT<tPoints::const_iterator> vPointsDstWalker(mPointsDst);
      while (vPointsDstWalker) {
        float vDistanceCosts = GetDistanceCosts(**vPointsSrcWalker, **vPointsDstWalker);
        if (vDistanceCosts < vDistanceCostsMin) {
          vDistanceCostsMin = vDistanceCosts;
          vPointSrc = *vPointsSrcWalker;
          vPointDst = *vPointsDstWalker;
        }
      }
    }

    // update path
    ClearConnection();
    if (vPointSrc != nullptr &&
        vPointDst != nullptr) {
      ggGraphicsPathItem::SetConnection(*vPointSrc, *vPointDst);
    }
  }

  // the connection points
  tPoints mPointsSrc;
  tPoints mPointsDst;

};











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

  ggGraphicsConnectionPointItem* vPointA = new ggGraphicsConnectionPointItem();
  ggGraphicsConnectionPointItem* vPointB = new ggGraphicsConnectionPointItem();
  ggGraphicsConnectionPointItem* vPointC = new ggGraphicsConnectionPointItem();
  vPointA->SetConnectionPointPosition(QPointF(100.0f, 50.0f));
  vPointB->SetConnectionPointPosition(QPointF(300.0f, 150.0f));
  vPointC->SetConnectionPointPosition(QPointF(200.0f, 200.0f));
  ggGraphicsShortestPathItem* vPath = new ggGraphicsShortestPathItem();
  vPath->InsertPointSrc(&vPointA->ConnectionPoint());
  vPath->InsertPointSrc(&vPointB->ConnectionPoint());
  vPath->InsertPointDst(&vPointC->ConnectionPoint());
  vPath->SetDecorationSrc(ggDecoration::cType::eRhomb, 20.0f, false);
  vPath->SetDecorationDst(ggDecoration::cType::eCircle, 10.0f, false);

  QGraphicsScene::addItem(vPointA);
  QGraphicsScene::addItem(vPointB);
  QGraphicsScene::addItem(vPointC);
  QGraphicsScene::addItem(vPath);


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
    ggWalkerT<ggStrings::const_iterator> vBaseClassNameWalker(vClass->mBaseNames);
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
