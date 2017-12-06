#ifndef GGPAINTERPATH_H
#define GGPAINTERPATH_H

#include <QPainterPath>

#include "ggDecoration.h"

class QVector2D;

class ggPainterPath :
  public QPainterPath
{
public:

  ggPainterPath();

  void SetDecorationRatio(float aRatio);
  float GetDecorationRatio() const;

  void AddDecoration(const ggDecoration& aDecoration,
                     const QPointF& aEnd,
                     const QVector2D& aDirection);

  void AddDecoration(ggDecoration::cType aType,
                     const QPointF& aStart,
                     const QPointF& aEnd);

  void AddLine(const QPointF& aStart, const QPointF& aEnd);
  void AddArrow(const QPointF& aStart, const QPointF& aEnd);
  void AddArrowBack(const QPointF& aStart, const QPointF& aEnd);
  void AddTriangle(const QPointF& aStart, const QPointF& aEnd);
  void AddTriangleBack(const QPointF& aStart, const QPointF& aEnd);
  void AddDiamond(const QPointF& aStart, const QPointF& aEnd);
  void AddCross(const QPointF& aStart, const QPointF& aEnd);
  void AddCircle(const QPointF& aStart, const QPointF& aEnd);

  static bool IsClosed(ggDecoration::cType aType);
  static bool IsOpen(ggDecoration::cType aType);
  static bool ConnectToCenter(ggDecoration::cType aType,
                              ggDecoration::cFill aFill);

private:

  QPointF CalculateCenter(const QPointF& aStart,
                          const QPointF& aEnd) const;

  void CalculateDimensions(ggDecoration::cType aType,
                           const QPointF& aStart,
                           const QPointF& aEnd,
                           QVector2D& aDirection,
                           QVector2D& aNormal,
                           float& aLength,
                           float& aWidth2) const;

  float GetDecorationRatio(ggDecoration::cType aType) const;

  float mDecorationRatio;

};

#endif // GGPAINTERPATH_H
