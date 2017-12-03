#include "ggGraphicsDecoratedPathItem.h"

#include <QPen>


ggGraphicsDecoratedPathItem::ggGraphicsDecoratedPathItem(QGraphicsItem* aParent) :
  QGraphicsPathItem(aParent),
  mDecorationItemSrc(nullptr),
  mDecorationItemDst(nullptr)
{
  QPen vPen(pen());
  vPen.setCapStyle(Qt::FlatCap);
  vPen.setJoinStyle(Qt::MiterJoin);
  vPen.setColor(QColor(180, 60, 0));
  vPen.setWidthF(1.5f);
  setPen(vPen);
  setBrush(Qt::NoBrush);
  setZValue(-1.0f);
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
                                                   ggDecoration::cFill aFill)
{
  mDecorationSrc.Set(aType, aLength, aFill);
  RebuildPath();
}


void ggGraphicsDecoratedPathItem::SetDecorationDst(ggDecoration::cType aType,
                                                   float aLength,
                                                   ggDecoration::cFill aFill)
{
  mDecorationDst.Set(aType, aLength, aFill);
  RebuildPath();
}


bool ggGraphicsDecoratedPathItem::NeedExtraPath(const ggDecoration& aDecoration) const
{
  if (aDecoration.GetFill() == ggDecoration::cFill::eSolid) {
    return (aDecoration.GetType() == ggDecoration::cType::eCircle) ||
           (aDecoration.GetType() == ggDecoration::cType::eTriangleBack) ||
           (aDecoration.GetType() == ggDecoration::cType::eDiamond) ||
           (aDecoration.GetType() == ggDecoration::cType::eCross);
  }
  else {
    return (aDecoration.GetType() == ggDecoration::cType::eArrow) ||
           (aDecoration.GetType() == ggDecoration::cType::eCross);
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
  if (!aDecoration.GetFillSolid()) vBrush = Qt::NoBrush;

  float vLength = aDecoration.GetLength();
  float vWidth = 0.4f * vLength;
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
  else if (aDecoration.GetType() == ggDecoration::cType::eArrowBack) {
    QVector2D vPointA(QVector2D(aPoint.GetPosition()) + vWidth*vNormal);
    QVector2D vPointB(QVector2D(aPoint.GetPosition()) - vWidth*vNormal);
    vPath.moveTo(vPointA.toPointF());
    vPath.lineTo(vPointControl.toPointF());
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
    if (aDecoration.GetFillSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eTriangleBack) {
    QVector2D vPointA(QVector2D(aPoint.GetPosition()) + vWidth*vNormal);
    QVector2D vPointB(QVector2D(aPoint.GetPosition()) - vWidth*vNormal);
    vPath.moveTo(vPointA.toPointF());
    vPath.lineTo(vPointControl.toPointF());
    vPath.lineTo(vPointB.toPointF());
    vPath.closeSubpath();
    if (aDecoration.GetFillSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eDiamond) {
    QVector2D vCenter(aPoint.GetControlPoint(vLength / 2.0f));
    QVector2D vPointA(vCenter + 0.6f*vWidth*vNormal);
    QVector2D vPointB(vCenter - 0.6f*vWidth*vNormal);
    vPath.moveTo(vPointControl.toPointF());
    vPath.lineTo(vPointA.toPointF());
    vPath.lineTo(aPoint.GetPosition());
    vPath.lineTo(vPointB.toPointF());
    vPath.closeSubpath();
    if (aDecoration.GetFillSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eCircle) {
    float vRadius = vLength / 2.0f;
    QPointF vCenter(aPoint.GetControlPoint(vRadius));
    vPath.addEllipse(vCenter, vRadius, vRadius);
    if (aDecoration.GetFillSolid()) vPen = Qt::NoPen;
  }
  else if (aDecoration.GetType() == ggDecoration::cType::eCross) {
    QVector2D vPointA(vPointControl + vWidth*vNormal);
    QVector2D vPointB(vPointControl - vWidth*vNormal);
    QVector2D vPointC(QVector2D(aPoint.GetPosition()) + vWidth*vNormal);
    QVector2D vPointD(QVector2D(aPoint.GetPosition()) - vWidth*vNormal);
    vPath.moveTo(vPointA.toPointF());
    vPath.lineTo(vPointD.toPointF());
    vPath.moveTo(vPointB.toPointF());
    vPath.lineTo(vPointC.toPointF());
    vBrush = Qt::NoBrush;
  }

  vPathItem->setPath(vPath);
  vPathItem->setPen(vPen);
  vPathItem->setBrush(vBrush);
  vPathItem->setZValue(5.0f);

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
