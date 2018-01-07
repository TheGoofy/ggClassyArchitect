// 0) include own header
#include "ggClassySettings.h"

// 1) include system or QT
#include <QSettings>

// 2) include own project-related (sort by component dependency)
#include "BaseWidgets/ggUtilityQt.h"


ggClassySettings::ggClassySettings()
{
  Sync();
}


void ggClassySettings::Sync()
{
  GetValue("ClassySettings/SelectionColor", QColor(20, 255, 0, 255), mSelectionColor); // 200, 255, 0, 255
  GetValue("ClassySettings/HighlightColor", QColor(25, 255, 255, 255), mHighlightColor); // 255, 150, 0, 255
  GetValue("ClassySettings/ShadowColor", QColor(0, 0, 255, 100), mShadowColor); // 0, 0, 0, 40
  GetValue("ClassySettings/ShadowOffset", QPointF(-6.5, 2.0), mShadowOffset); // 1.5, 1.0
  GetValue("ClassySettings/ShadowWidth", 10.0, mShadowWidth); // 4.0
  GetValue("ClassySettings/HandleSize", 15.0, mHandleSize); // 9.0
}


ggClassySettings* ggClassySettings::GetInstance()
{
  static ggClassySettings vClassySettings;
  return &vClassySettings;
}


template <typename T>
void ggClassySettings::GetValue(const QString& aKey, const T& aValueDefault, T& aValueOut) const
{
  QSettings vSettings;
  const QVariant& vValue = vSettings.value(aKey, aValueDefault);
  aValueOut = vValue.value<T>();
}


template <typename T>
void ggClassySettings::SetValue(const QString& aKey, const T& aValueIn, T& aValueOut)
{
  if (aValueOut != aValueIn) {
    aValueOut = aValueIn;
    QSettings vSettings;
    vSettings.setValue(aKey, aValueIn);
    Notify();
  }
}


const QColor& ggClassySettings::GetSelectionColor() const
{
  return mSelectionColor;
}


void ggClassySettings::SetSelectionColor(const QColor& aColor)
{
  SetValue("ClassySettings/SelectionColor", aColor, mSelectionColor);
}


const QColor& ggClassySettings::GetHighlightColor() const
{
  return mHighlightColor;
}


void ggClassySettings::SetHighlightColor(const QColor& aColor)
{
  SetValue("ClassySettings/HighlightColor", aColor, mHighlightColor);
}


const QColor& ggClassySettings::GetShadowColor() const
{
  return mShadowColor;
}


void ggClassySettings::SetShadowColor(const QColor& aColor)
{
  SetValue("ClassySettings/ShadowColor", aColor, mShadowColor);
}


const QPointF& ggClassySettings::GetShadowOffset() const
{
  return mShadowOffset;
}


void ggClassySettings::SetShadowOffset(const QPointF& aOffset)
{
  SetValue("ClassySettings/ShadowOffset", aOffset, mShadowOffset);
}


qreal ggClassySettings::GetShadowWidth() const
{
  return mShadowWidth;
}


void ggClassySettings::SetShadowWidth(qreal aWidth)
{
  SetValue("ClassySettings/ShadowWidth", aWidth, mShadowWidth);
}


qreal ggClassySettings::GetHandleSize() const
{
  return mHandleSize;
}


void ggClassySettings::SetHandleSize(qreal aSize)
{
  SetValue("ClassySettings/HandleSize", aSize, mHandleSize);
}
