#ifndef GGGRAPHICSDECORATEDPATHITEM_H
#define GGGRAPHICSDECORATEDPATHITEM_H

#include <QGraphicsPathItem>

#include "Graphics/ggConnectionPoint.h"
#include "Graphics/ggDecoration.h"

class ggGraphicsDecoratedPathItem :
  public QGraphicsPathItem
{
public:

  ggGraphicsDecoratedPathItem(QGraphicsItem* aParent = nullptr);

  void ClearConnection();
  void SetConnection(const ggConnectionPoint& aPointSrc,
                     const ggConnectionPoint& aPointDst);

  const ggDecoration& DecorationSrc() const;

  const ggDecoration& DecorationDst() const;

  void SetDecorationSrc(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        ggDecoration::cFill aFill = ggDecoration::cFill::eSolid);

  void SetDecorationDst(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        ggDecoration::cFill aFill = ggDecoration::cFill::eSolid);

private:

  QPainterPath GetPath(const ggConnectionPoint& aPointSrc,
                       const ggConnectionPoint& aPointDst) const;

  QGraphicsItem* CreateDecoration(const ggConnectionPoint& aPoint,
                                  const ggDecoration& aDecoration);

  void RebuildPath();

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

#endif // GGGRAPHICSDECORATEDPATHITEM_H
