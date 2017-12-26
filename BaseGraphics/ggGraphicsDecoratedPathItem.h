#ifndef GGGRAPHICSDECORATEDPATHITEM_H
#define GGGRAPHICSDECORATEDPATHITEM_H

// 1) include system or QT
#include <QGraphicsPathItem>

// 2) include own project-related (sort by component dependency)
#include "BaseGraphics/ggConnectionPoint.h"
#include "BaseGraphics/ggDecoration.h"

// 3) forward declarations

/**
 * @brief The ggGraphicsDecoratedPathItem class
 */
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

  void setPen(const QPen& aPen);

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
