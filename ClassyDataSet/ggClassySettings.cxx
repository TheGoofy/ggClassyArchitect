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
  GetValue("ClassySettings/BackgroundColor", QColor(150, 150, 150, 255), mBackgroundColor);
  GetValue("ClassySettings/SelectionColor", QColor(200, 255, 0, 255), mSelectionColor);
  GetValue("ClassySettings/HighlightColor", QColor(255, 100, 0, 255), mHighlightColor);
  GetValue("ClassySettings/ShadowColor", QColor(0, 0, 0, 40), mShadowColor);
  GetValue("ClassySettings/ShadowOffset", QPointF(1.5, 1.0), mShadowOffset);
  GetValue("ClassySettings/ShadowWidth", 4.0, mShadowWidth);
  GetValue("ClassySettings/HandleSize", 9.0, mHandleSize);
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


const QColor& ggClassySettings::GetBackgroundColor() const
{
  return mBackgroundColor;
}


void ggClassySettings::SetBackgroundColor(const QColor& aColor)
{
  SetValue("ClassySettings/BackgroundColor", aColor, mBackgroundColor);
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
