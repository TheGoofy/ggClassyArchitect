#ifndef GGUTILITYQT_H
#define GGUTILITYQT_H

// 1) include system (or QT)
#include <QColor>
#include <QRect>
#include <QBrush>

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
  float GetColorLuma(const QColor& aColor);

  QColor GetColorScaled(const QColor& aColor, float aScale);
  QColor GetColorInterpolated(const QColor& aColorA, const QColor& aColorB, float aWeightA);
  QColor GetColorContrast(const QColor& aColor);
  QColor GetColorSaturized(const QColor& aColor);
  QColor GetColorWithAlpha(const QColor& aColor, float aAlpha);
  QColor GetColorWithValue(const QColor& aColor, float aValue);
  QColor GetColorWithLightness(const QColor& aColor, float aLightness);

  QRectF GetRectInflated(const QRectF& aRect, float aDelta);

  QString ToString(const QColor& aColor);
  bool FromString(const QString& aColorARGB, QColor& aColor);

  template <typename T>
  T FromString(const QString& aString, bool* aOK = nullptr);

  QBrush GetCheckerBoardBrush(int aSize = 5, const QColor& aColor = QColor(170, 170, 170, 255));
}

#endif // GGUTILITYQT_H
