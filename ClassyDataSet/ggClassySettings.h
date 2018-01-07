#ifndef GGCLASSYSETTINGS_H
#define GGCLASSYSETTINGS_H

// 1) include system (or QT)
#include <QColor>
#include <QPointF>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

class ggClassySettings :
  public ggSubject
{
public:

  static ggClassySettings* GetInstance();

  const QColor& GetSelectionColor() const;
  void SetSelectionColor(const QColor& aColor);
  const QColor& GetHighlightColor() const;
  void SetHighlightColor(const QColor& aColor);
  const QColor& GetShadowColor() const;
  void SetShadowColor(const QColor& aColor);
  const QPointF& GetShadowOffset() const;
  void SetShadowOffset(const QPointF& aOffset);
  qreal GetShadowWidth() const;
  void SetShadowWidth(qreal aWidth);
  qreal GetHandleSize() const;
  void SetHandleSize(qreal aSize);

  void Sync();

private:

  ggClassySettings();
  ggClassySettings(const ggClassySettings& aOther);
  ggClassySettings& operator = (const ggClassySettings& aOther);

  template <typename T>
  void GetValue(const QString& aKey, const T& aValueDefault, T& aValueOut) const;
  template <typename T>
  void SetValue(const QString& aKey, const T& aValueIn, T& aValueOut);

  QColor mSelectionColor;
  QColor mHighlightColor;
  QColor mShadowColor;
  QPointF mShadowOffset;
  qreal mShadowWidth;
  qreal mHandleSize;

};

#endif // GGCLASSYSETTINGS_H
