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

  QColor GetColor() const;
  bool GetAdaptBrightness() const;

signals:

  void ColorChanged(const QColor& aColor);

public slots:

  void SetColor(const QColor& aColor);
  void SetAdaptBrightness(bool aAdapt);

protected:

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;
  virtual void paintEvent(QPaintEvent* aEvent) override;

private:

  // helper methods
  void UpdateCornerColors();
  QPointF GetPosition(const QColor& aColor) const;
  QPointF ClampPosition(const QPointF& aPosition) const;
  QColor GetColorSaturized(const QPointF& aPosition) const;
  QColor GetColorFromWheel(const QPointF& aPosition) const;
  bool IsInside(const QPointF& aPosition) const;

  // the selected color
  QColor mColorSaturized;
  float mColorBrightness;

  // adjust brightness of the wheel according to the color
  bool mAdaptBrightness;

  // selector / indicator
  QPointF mColorPosition;
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

  // some mouse button is pressed
  bool mMouseDragging;

};

#endif // GGCOLORPICKERWIDGET_H
