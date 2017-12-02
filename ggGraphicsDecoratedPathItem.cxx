#include "ggGraphicsDecoratedPathItem.h"

#include <QPen>


ggGraphicsDecoratedPathItem::ggGraphicsDecoratedPathItem(QGraphicsItem* aParent) :
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


void ggGraphicsDecoratedPathItem::ClearConnection()
{
  setPath(QPainterPath());
  delete mDecorationItemSrc;
  delete mDecorationItemDst;
  mDecorationItemSrc = nullptr;
  mDecorationItemDst = nullptr;
}


void ggGraphicsDecoratedPathItem::SetConnection(const ggConnectionPoint& aPointSrc,
                                       const ggConnectionPoint& aPointDst)
{
  mPointSrc = aPointSrc;
  mPointDst = aPointDst;
  RebuildPath();
}


const ggDecoration& ggGraphicsDecoratedPathItem::DecorationSrc() const
{
  return mDecorationSrc;
}


const ggDecoration& ggGraphicsDecoratedPathItem::DecorationDst() const
{
  return mDecorationDst;
}


void ggGraphicsDecoratedPathItem::SetDecorationSrc(ggDecoration::cType aType,
                                          float aLength,
                                          bool aSolid)
{
  mDecorationSrc.Set(aType, aLength, aSolid);
  RebuildPath();
}


void ggGraphicsDecoratedPathItem::SetDecorationDst(ggDecoration::cType aType,
                                          float aLength,
                                          bool aSolid)
{
  mDecorationDst.Set(aType, aLength, aSolid);
  RebuildPath();
}


bool ggGraphicsDecoratedPathItem::NeedExtraPath(const ggDecoration& aDecoration) const
{
  if (aDecoration.GetSolid()) {
    return (aDecoration.GetType() == ggDecoration::cType::eCircle) ||
           (aDecoration.GetType() == ggDecoration::cType::eRhomb);
  }
  else {
    return (aDecoration.GetType() == ggDecoration::cType::eArrow);
  }
}


QPainterPath ggGraphicsDecoratedPathItem::GetPath(const ggConnectionPoint& aPointSrc,
                                         const ggConnectionPoint& aPointDst) const
{
  // calculate the key points
  QPointF vPointSrcBase = aPointSrc.GetControlPoint(mDecorationSrc.GetLength());
  QPointF vPointDstBase = aPointDst.GetControlPoint(mDecorationDst.GetLength());
  float vDistance = QLineF(vPointSrcBase, vPointDstBase).length();
  QPointF vPointSrcControl = aPointSrc.GetControlPoint(mDecorationSrc.GetLength() + vDistance/2.5f);
  QPointF vPointDstControl = aPointDst.GetControlPoint(mDecorationDst.GetLength() + vDistance/2.5f);

  // some decorations need a bit a longer path
  bool vExtraSrcPath = NeedExtraPath(mDecorationSrc);
  bool vExtraDstPath = NeedExtraPath(mDecorationDst);
  QPointF vPointSrcExtraBase = vExtraSrcPath ? aPointSrc.GetControlPoint(mDecorationSrc.GetLength()/2.0f) : vPointSrcBase;
  QPointF vPointDstExtraBase = vExtraDstPath ? aPointDst.GetControlPoint(mDecorationDst.GetLength()/2.0f) : vPointDstBase;

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


QGraphicsItem* ggGraphicsDecoratedPathItem::CreateDecoration(const ggConnectionPoint& aPoint,
                                                    const ggDecoration& aDecoration)
{
  QGraphicsPathItem* vPathItem = new QGraphicsPathItem(this);
  QPainterPath vPath;

  QPen vPen(pen());
  vPen.setStyle(Qt::SolidLine);
  QBrush vBrush(vPen.color());
  if (!aDecoration.GetSolid()) vBrush = Qt::NoBrush;

  float vLength = aDecoration.GetLength();
  float vWidth = 0.5f * vLength;
  QVector2D vPointControl(aPoint.GetControlPoint(vLength));
  QVector2D vNormal(aPoint.GetDirection().y(), -aPoint.GetDirection().x());

  if (aDecoration.GetType() == ggDecoration::cType::eLine) {
    vPath.moveTo(aPoint.GetPosition());
    vPath.lineTo(vPointControl.toPointF());
    vBrush = Qt::NoBrush;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eArrow) {
    QVector2D vCenter(aPoint.GetControlPoint(vLength / 2.0f));
    QVector2D vPointA(vPointControl + vWidth*vNormal);
    QVector2D vPointB(vPointControl - vWidth*vNormal);
    vPath.moveTo(aPoint.GetPosition());
    vPath.lineTo(vCenter.toPointF());
    vPath.moveTo(vPointA.toPointF());
    vPath.lineTo(aPoint.GetPosition());
    vPath.lineTo(vPointB.toPointF());
    vBrush = Qt::NoBrush;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eTriangle) {
    QVector2D vPointA(vPointControl + vWidth*vNormal);
    QVector2D vPointB(vPointControl - vWidth*vNormal);
    vPath.moveTo(vPointA.toPointF());
    vPath.lineTo(aPoint.GetPosition());
    vPath.lineTo(vPointB.toPointF());
    vPath.closeSubpath();
    if (aDecoration.GetSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eRhomb) {
    QVector2D vCenter(aPoint.GetControlPoint(vLength / 2.0f));
    QVector2D vPointA(vCenter + 0.5f*vWidth*vNormal);
    QVector2D vPointB(vCenter - 0.5f*vWidth*vNormal);
    vPath.moveTo(vPointControl.toPointF());
    vPath.lineTo(vPointA.toPointF());
    vPath.lineTo(aPoint.GetPosition());
    vPath.lineTo(vPointB.toPointF());
    vPath.closeSubpath();
    if (aDecoration.GetSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eCircle) {
    float vRadius = vLength / 2.0f;
    QPointF vCenter(aPoint.GetControlPoint(vRadius));
    vPath.addEllipse(vCenter, vRadius, vRadius);
    if (aDecoration.GetSolid()) vPen = Qt::NoPen;
  }

  vPathItem->setPath(vPath);
  vPathItem->setPen(vPen);
  vPathItem->setBrush(vBrush);

  return vPathItem;
}


void ggGraphicsDecoratedPathItem::RebuildPath()
{
  // update path
  setPath(GetPath(mPointSrc, mPointDst));

  // update decoration
  delete mDecorationItemSrc;
  delete mDecorationItemDst;
  mDecorationItemSrc = CreateDecoration(mPointSrc, mDecorationSrc);
  mDecorationItemDst = CreateDecoration(mPointDst, mDecorationDst);
}
