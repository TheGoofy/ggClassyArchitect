#ifndef GGCOLORPICKERWIDGET_H
#define GGCOLORPICKERWIDGET_H

// 1) include system or QT
#include <QWidget>
#include <QVector2D>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggColorWheelWidget :
  public QWidget
{
  Q_OBJECT

public:

  explicit ggColorWheelWidget(QWidget* aParent = nullptr);

  void SetColor(const QColor& aColor);
  QColor GetColorFromWheel() const;
  void SetAdaptBrightness(bool aAdapt);
  bool GetAdaptBrightness() const;

protected:

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;
  virtual void paintEvent(QPaintEvent* aEvent) override;

private:

  // helper methods
  void UpdateColors();
  QPointF GetPosition(const QColor& aColor) const;
  QPointF ClampPosition(const QPointF& aPosition) const;
  QColor GetColorSaturized(const QPointF& aPosition) const;
  QColor GetColorFromWheel(const QPointF& aPosition) const;

  // the selected color
  QColor mColorSaturized;
  QPointF mColorPosition;

  // adjust brightness of the wheel according to the color
  bool mAdaptBrightness;
  float mColorBrightness;

  // selector / indicator
  float mSelectorRadius;

  // color wheel corner colors
  QColor mColorR; // red
  QColor mColorY; // yellow
  QColor mColorG; // green
  QColor mColorC; // cyan
  QColor mColorB; // blue
  QColor mColorM; // magenta

  // color wheel corner coordinates
  QPointF mPosCenter; // center
  QVector2D mDirR; // red corner
  QVector2D mDirY; // yellow corner
  QVector2D mDirG; // green corner
  QVector2D mDirC; // cyan corner
  QVector2D mDirB; // blue corner
  QVector2D mDirM; // magenta corner

};

#endif // GGCOLORPICKERWIDGET_H