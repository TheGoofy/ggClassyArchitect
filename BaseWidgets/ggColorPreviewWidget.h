#ifndef GGCOLORPREVIEWWIDGET_H
#define GGCOLORPREVIEWWIDGET_H

// 1) include system or QT
#include <QWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggColorPreviewWidget :
  public QWidget
{
  Q_OBJECT
public:

  explicit ggColorPreviewWidget(QWidget* aParent = nullptr);

  const QColor& GetColor() const;

  void SetHorizontal();
  void SetVertical();
  bool IsHorizontal() const;
  bool IsVertical() const;

  void SetClickable(bool aClickable);
  bool IsClickable() const;

signals:

  void ColorChanged(const QColor& aColor);
  void Clicked();

public slots:

  void SetColor(const QColor& aColor);

protected:

  virtual QSize sizeHint() const override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;
  virtual void paintEvent(QPaintEvent* aEvent) override;

private:

  QRectF GetIndicatorRect(qreal aMargin) const;
  QRectF GetColorBarLowerRect() const;

  // THE color
  QColor mColor;

  // selector / indicator
  float mSelectorRadius;
  float mSelectorRadiusLarge;

  // geometry / laout
  enum class cLayout {
    eHorizontal,
    eVertical
  };
  cLayout mLayout;

  // clickable like a button
  bool mClickable;

  // area for color channel
  QRectF mColorBar;
  QRectF mColorIndicatorLarge;
  QRectF mColorIndicatorSmall;

};

#endif // GGCOLORPREVIEWWIDGET_H
