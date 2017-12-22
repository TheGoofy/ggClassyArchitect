#ifndef GGUTILITYQT_H
#define GGUTILITYQT_H

// 1) include system (or QT)
#include <QColor>
#include <QRect>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"

// 3) forward declarations

/**
 * collection of general, simple utilities related with QT
 */
namespace ggUtilityQt
{

  int GetColorMax(const QColor& aColor);
  float GetColorMaxF(const QColor& aColor);
  float GetColorLightness(const QColor& aColor);
  float GetColorValue(const QColor& aColor);
  QColor GetColorSaturized(const QColor& aColor);
  QColor GetColorScaled(const QColor& aColor, float aBrightness);
  QColor GetColorContrast(const QColor& aColor);
  QColor GetColorAlpha(const QColor& aColor, float aAlpha);
  QRectF GetRectInflated(const QRectF& aRect, float aDelta);

}

#endif // GGUTILITYQT_H
