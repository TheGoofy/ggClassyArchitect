#ifndef GGCOLORBRIGHTNESSWIDGET_H
#define GGCOLORBRIGHTNESSWIDGET_H

// 1) include system or QT
#include <QWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggColorBrightnessWidget :
  public QWidget
{
  Q_OBJECT
public:

  explicit ggColorBrightnessWidget(QWidget* aParent = nullptr);

  QColor GetColor() const;

signals:

  void ColorChanged(const QColor& aColor);

public slots:

  void SetColor(const QColor& aColor);

protected:

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;
  virtual void paintEvent(QPaintEvent* aEvent) override;

private:

  QPointF GetPosition(const float aBrightness) const;
  QPointF ClampPosition(const QPointF& aPosition) const;
  float GetBrightness(const QPointF& aPosition) const;
  bool IsInside(const QPointF& aPosition) const;
  QBrush GetGradientBrush() const;
  QRectF GetSelectorRect(float aSize) const;

  // THE color
  QColor mColorSaturized;
  float mColorBrightness;

  // selector / indicator
  QPointF mColorPosition;
  float mSelectorRadius;
  float mSelectorRadiusLarge;
  bool mMouseDragging;

  // geometry / laout
  enum class cLayout {
    eHorizontal,
    eVertical
  };
  cLayout mLayout;
  QRectF mColorBar;

};

#endif // GGCOLORBRIGHTNESSWIDGET_H
