// 0) include own header
#include "ggUtilityQt.h"

// 1) include system
#include <QDebug>

// 2) include own project-related (sort by component dependency)


int ggUtilityQt::GetColorMax(const QColor& aColor)
{
  return ggUtility::Max(aColor.red(), aColor.green(), aColor.blue());
}


float ggUtilityQt::GetColorMaxF(const QColor& aColor)
{
  return ggUtility::Max(aColor.redF(), aColor.greenF(), aColor.blueF());
}


float ggUtilityQt::GetColorLightness(const QColor& aColor)
{
  return 0.299f * aColor.redF() +
         0.587f * aColor.greenF() +
         0.114f * aColor.blueF();
}


float ggUtilityQt::GetColorBrightness(const QColor& aColor)
{
  return ggUtility::Max(aColor.redF(), aColor.greenF(), aColor.blueF());
}


QColor ggUtilityQt::GetColorSaturized(const QColor& aColor)
{
  float vColorBrightness = ggUtilityQt::GetColorMaxF(aColor);
  if (vColorBrightness != 0.0f) {
    float vScale = 1.0f / vColorBrightness;
    return QColor::fromRgbF(ggUtility::Clamp<float>(vScale * aColor.redF(), 0.0f, 1.0f),
                            ggUtility::Clamp<float>(vScale * aColor.greenF(), 0.0f, 1.0f),
                            ggUtility::Clamp<float>(vScale * aColor.blueF(), 0.0f, 1.0f),
                            aColor.alphaF());
  }
  else {
    QColor vColor(Qt::white);
    vColor.setAlphaF(aColor.alphaF());
    return vColor;
  }
}


QColor ggUtilityQt::GetColorScaled(const QColor& aColor, float aBrightness)
{
  return QColor::fromRgbF(ggUtility::Clamp<float>(aBrightness * aColor.redF(), 0.0f, 1.0f),
                          ggUtility::Clamp<float>(aBrightness * aColor.greenF(), 0.0f, 1.0f),
                          ggUtility::Clamp<float>(aBrightness * aColor.blueF(), 0.0f, 1.0f),
                          aColor.alphaF());
}


QColor ggUtilityQt::GetColorContrast(const QColor& aColor)
{
  return (GetColorLightness(aColor) < 0.5f) ? QColor(Qt::white) : QColor(Qt::black);
}


QColor ggUtilityQt::GetColorAlpha(const QColor& aColor, float aAlpha)
{
  return QColor::fromRgbF(aColor.redF(), aColor.greenF(), aColor.blueF(), aAlpha);
}


QRectF ggUtilityQt::GetRectInflated(const QRectF& aRect, float aDelta)
{
  return QRect(aRect.x() - aDelta,
               aRect.y() - aDelta,
               aRect.width() + 2.0f * aDelta,
               aRect.height() + 2.0f * aDelta).normalized();
}
