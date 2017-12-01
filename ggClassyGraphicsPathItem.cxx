#include "ggClassyGraphicsPathItem.h"


ggClassyGraphicsPathItem::ggClassyGraphicsPathItem(QGraphicsItem* aParent) :
  QGraphicsPathItem(aParent),
  mBoxItems(nullptr),
  mArrowLength(12.0f),
  mLineAnchorLength(3.0f)
{
  QPen vPen(QColor(180, 60, 0), 1.5f);
  vPen.setCapStyle(Qt::FlatCap);
  setPen(vPen);
  setBrush(Qt::NoBrush);
  setZValue(-1.0f);
}


void ggClassyGraphicsPathItem::SetBoxItems(const ggClassyGraphicsBoxItems* aBoxItems)
{
  if (mBoxItems != aBoxItems) {
    Detach(mBoxItems);
    mBoxItems = aBoxItems;
    Attach(mBoxItems);
    UpdateBoxItems();
    UpdatePath();
  }
}


void ggClassyGraphicsPathItem::SetConnection(const QString& aClassNameSrc, const QString& aClassNameDst)
{
  bool vModified = false;
  if (mClassNameSrc != aClassNameSrc) {
    mClassNameSrc = aClassNameSrc;
    vModified = true;
  }
  if (mClassNameDst != aClassNameDst) {
    mClassNameDst = aClassNameDst;
    vModified = true;
  }
  if (vModified) {
    UpdateBoxItems();
    UpdatePath();
  }
}


void ggClassyGraphicsPathItem::SetColor(const QColor& aColor)
{
  // actual line color
  QPen vPen(pen());
  vPen.setColor(aColor);
  setPen(vPen);

  // arrow(s) color
  QList<QGraphicsItem*> vChildItems = childItems();
  ggWalkerT<QList<QGraphicsItem*>::iterator> vChildItemsWalker(vChildItems);
  while (vChildItemsWalker) {
    QAbstractGraphicsShapeItem* vItem = dynamic_cast<QAbstractGraphicsShapeItem*>(*vChildItemsWalker);
    if (vItem != nullptr) vItem->setBrush(aColor);
  }
}


void ggClassyGraphicsPathItem::SetLineWidth(float aWidth)
{
  QPen vPen(pen());
  vPen.setWidth(aWidth);
  setPen(vPen);
  mLineAnchorLength = aWidth + 1.5f;
  UpdatePath();
}


void ggClassyGraphicsPathItem::SetArrowLength(float aLength)
{
  mArrowLength = aLength;
  UpdatePath();
}


void ggClassyGraphicsPathItem::Update(const ggSubject* aSubject)
{
  if (aSubject == mBoxItems) {
    UpdateBoxItems();
    UpdatePath();
  }
  else if (mBoxItemSubjects.find(aSubject) != mBoxItemSubjects.end()) {
    UpdatePath();
  }
}


void ggClassyGraphicsPathItem::AttachSubjects(const tSubjectsSet& aSubjects)
{
  ggWalkerT<tSubjectsSet::const_iterator> vSubjectsWalker(aSubjects);
  while (vSubjectsWalker) Attach(*vSubjectsWalker);
}


void ggClassyGraphicsPathItem::DetachSubjects(const tSubjectsSet& aSubjects)
{
  ggWalkerT<tSubjectsSet::const_iterator> vSubjectsWalker(aSubjects);
  while (vSubjectsWalker) Detach(*vSubjectsWalker);
}


void ggClassyGraphicsPathItem::InsertBoxItemSubjects(const tBoxItemsVec& aBoxItems)
{
  ggWalkerT<tBoxItemsVec::const_iterator> vBoxItemsWalker(aBoxItems);
  while (vBoxItemsWalker) {
    ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    mBoxItemSubjects.insert(vBoxItem->GetSubjectPosition());
    mBoxItemSubjects.insert(vBoxItem->GetSubjectSize());
  }
}


void ggClassyGraphicsPathItem::UpdateBoxItems()
{
  if (mBoxItems != nullptr) {

    // get the box items
    mBoxItemsSrc = mBoxItems->GetBoxItems(mClassNameSrc);
    mBoxItemsDst = mBoxItems->GetBoxItems(mClassNameDst);

    // (re)connect all subjects
    DetachSubjects(mBoxItemSubjects);
    mBoxItemSubjects.clear();
    InsertBoxItemSubjects(mBoxItemsSrc);
    InsertBoxItemSubjects(mBoxItemsDst);
    AttachSubjects(mBoxItemSubjects);
  }
}


void ggClassyGraphicsPathItem::UpdatePath()
{
  // get rid of the old arrows
  QList<QGraphicsItem*> vChildItems = childItems();
  ggWalkerT<QList<QGraphicsItem*>::iterator> vChildItemsWalker(vChildItems);
  while (vChildItemsWalker) delete *vChildItemsWalker;

  // start with empty path
  QPainterPath vPath;

  // connect ALL sources with THE nearest destination (outer loop over src)
  ggWalkerT<tBoxItemsVec::iterator> vBoxItemsSrcWalker(mBoxItemsSrc);
  while (vBoxItemsSrcWalker) {

    // start looking for destination far away
    float vShortestDistance = std::numeric_limits<float>::max();
    ggClassyGraphicsBoxItem* vBoxItemSrc = *vBoxItemsSrcWalker;
    QPointF vPosSrc = vBoxItemSrc->GetPositionTopCenter();
    QPointF vPosDst = vPosSrc;
    float vDecorationSizeY = 2.0f*mLineAnchorLength + mArrowLength;

    // search destination with shortest distance (loop over dst)
    ggWalkerT<tBoxItemsVec::iterator> vBoxItemsDstWalker(mBoxItemsDst);
    while (vBoxItemsDstWalker) {
      ggClassyGraphicsBoxItem* vBoxItemDst = *vBoxItemsDstWalker;
      QPointF vPosition = vBoxItemDst->GetPositionBottomCenter();
      // euclidean distance
      float vDistanceE = QLineF(vPosSrc, vPosition).length();
      // special distance along y: favor distance pointing into positive direction
      float vDistanceY = vPosSrc.y() - vPosition.y();
      if (vDistanceY < vDecorationSizeY) {
        vDistanceY = -100.0f*(vDistanceY-vDecorationSizeY) + vDecorationSizeY;
      }
      // combine the two distances
      float vDistance = vDistanceE + vDistanceY;
      if (vDistance < vShortestDistance) {
        vShortestDistance = vDistance;
        vPosDst = vPosition;
      }
    }

    // did we find a destination point?
    if (vShortestDistance != std::numeric_limits<float>::max()) {

      // calculate some more control points
      QPointF vPos1(vPosSrc.x(), vPosSrc.y() - mLineAnchorLength);
      QPointF vPos2(vPosDst.x(), vPosDst.y() + mArrowLength + mLineAnchorLength);
      QPointF vPos3(vPosDst.x(), vPosDst.y() + mArrowLength);
      float vPosCenterY = (vPos1.y() + vPos2.y()) / 2.0f;
      float vPosCtrlOffY = QLineF(vPos1,vPos2).length()/3.0f + 2.0f*mLineAnchorLength;
      float vPos1CtrlY = qMin<float>(vPos1.y() - vPosCtrlOffY, vPosCenterY);
      float vPos2CtrlY = qMax<float>(vPos2.y() + vPosCtrlOffY, vPosCenterY);
      QPointF vPos1Ctrl(vPosSrc.x(), vPos1CtrlY);
      QPointF vPos2Ctrl(vPosDst.x(), vPos2CtrlY);

      // the courved spline path
      vPath.moveTo(vPosSrc);
      vPath.lineTo(vPos1);
      vPath.cubicTo(vPos1Ctrl,
                    vPos2Ctrl,
                    vPos2);
      vPath.lineTo(vPos3);

      // a nice arrow using the same color from the line as a brush
      QPainterPath vArrowPath;
      vArrowPath.setFillRule(Qt::WindingFill);
      vArrowPath.moveTo(vPosDst);
      vArrowPath.lineTo(vPosDst + QPointF( mArrowLength/2.5f, mArrowLength));
      vArrowPath.lineTo(vPosDst + QPointF(-mArrowLength/2.5f, mArrowLength));
      vArrowPath.closeSubpath();
      QGraphicsPathItem* vArrowItem = new QGraphicsPathItem(vArrowPath, this);
      vArrowItem->setBrush(pen().color());
      vArrowItem->setPen(Qt::NoPen);
    }
  }

  // replace the previous path
  setPath(vPath);
}
