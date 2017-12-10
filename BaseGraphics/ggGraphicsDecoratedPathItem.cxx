// 0) include own header
#include "ggGraphicsDecoratedPathItem.h"

// 1) include system or QT
#include <QPen>

// 2) include own project-related (sort by component dependency)
#include "BaseGraphics/ggPainterPath.h"


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
  bool vExtraSrcPath = ggPainterPath::ConnectToCenter(mDecorationSrc.GetType(), mDecorationSrc.GetFill());
  bool vExtraDstPath = ggPainterPath::ConnectToCenter(mDecorationDst.GetType(), mDecorationDst.GetFill());
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
  QPen vPen(pen());
  vPen.setStyle(Qt::SolidLine);
  if (aDecoration.GetFillSolid() &&
      ggPainterPath::IsClosed(aDecoration.GetType())) {
    vPen = Qt::NoPen;
  }

  QBrush vBrush(pen().color());
  if (aDecoration.GetFillEmpty() ||
      ggPainterPath::IsOpen(aDecoration.GetType())) {
    vBrush = Qt::NoBrush;
  }

  ggPainterPath vPath;
  vPath.AddDecoration(aDecoration, aPoint.GetPosition(), aPoint.GetDirection());

  QGraphicsPathItem* vPathItem = new QGraphicsPathItem(this);
  vPathItem->setPen(vPen);
  vPathItem->setBrush(vBrush);
  vPathItem->setPath(vPath);

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
