// 0) include own header
#include "ggColorBrightnessWidget.h"

// 1) include system or QT
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"
#include "BaseWidgets//ggUtilityQt.h"


ggColorBrightnessWidget::ggColorBrightnessWidget(QWidget* aParent) :
  QWidget(aParent),
  mSelectorRadius(3.0f),
  mSelectorRadiusLarge(9.0f),
  mMouseDragging(false),
  mLayout(cLayout::eHorizontal)
{
  setMouseTracking(true);
  SetColor(QColor(200, 150, 0, 255));
}


void ggColorBrightnessWidget::SetColor(const QColor& aColor)
{
  // calculate saturized color and brightness
  float vColorBrightness = ggUtilityQt::GetColorBrightness(aColor);
  QColor vColorSaturized = ggUtilityQt::GetColorSaturized(aColor);

  // update colors, if changed
  if (vColorBrightness != mColorBrightness ||
      vColorSaturized != mColorSaturized) {
    mColorSaturized = vColorSaturized;
    mColorBrightness = vColorBrightness;
    mColorPosition = GetPosition(mColorBrightness);
    update(); // trigger repaint
  }
}


QColor ggColorBrightnessWidget::GetColor() const
{
  return ggUtilityQt::GetColorScaled(mColorSaturized, mColorBrightness);
}


QPointF ggColorBrightnessWidget::ClampPosition(const QPointF& aPosition) const
{
  return QPointF(ggUtility::Clamp(aPosition.x(), mColorBar.x(), mColorBar.right()),
                 ggUtility::Clamp(aPosition.y(), mColorBar.y(), mColorBar.bottom()));
}


QPointF ggColorBrightnessWidget::GetPosition(const float aBrightness) const
{
  if (mLayout == cLayout::eHorizontal) {
    return QPointF(mColorBar.x() + aBrightness * mColorBar.width(), mColorBar.center().y());
  }
  if (mLayout == cLayout::eVertical) {
    return QPointF(mColorBar.center().x(), mColorBar.bottom() - aBrightness * mColorBar.height());
  }
  return mColorBar.center();
}


float ggColorBrightnessWidget::GetBrightness(const QPointF& aPosition) const
{
  QPointF vPosition = ClampPosition(aPosition);
  if (mLayout == cLayout::eHorizontal) {
    return (vPosition.x() - mColorBar.x()) / mColorBar.width();
  }
  if (mLayout == cLayout::eVertical) {
    return (mColorBar.bottom() - vPosition.y()) / mColorBar.height();
  }
  return 0.5f;
}


bool ggColorBrightnessWidget::IsInside(const QPointF& aPosition) const
{
  return mColorBar.contains(aPosition);
}


void ggColorBrightnessWidget::mousePressEvent(QMouseEvent* aEvent)
{
  // change selected color
  if (aEvent->button() == Qt::LeftButton) {
    mMouseDragging = true;
    setCursor(Qt::BlankCursor);
    mColorBrightness = GetBrightness(aEvent->pos());
    mColorPosition = GetPosition(mColorBrightness);
    emit ColorChanged(GetColor());
    update();
  }

  // base method
  QWidget::mousePressEvent(aEvent);
}


void ggColorBrightnessWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
  // change selected color
  if (aEvent->button() == Qt::LeftButton) {
    mMouseDragging = false;
    setCursor(IsInside(aEvent->pos()) ? Qt::CrossCursor : Qt::ArrowCursor);
    update();
  }

  // base method
  QWidget::mouseReleaseEvent(aEvent);
}


void ggColorBrightnessWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  // adjust mouse pointer, if it is inside of the color-wheel
  if (mMouseDragging) {
    setCursor(IsInside(aEvent->pos()) ? Qt::BlankCursor : Qt::CrossCursor);
  }
  else {
    setCursor(IsInside(aEvent->pos()) ? Qt::CrossCursor : Qt::ArrowCursor);
  }

  // change selected color
  if (aEvent->buttons() & Qt::LeftButton) {
    mColorBrightness = GetBrightness(aEvent->pos());
    mColorPosition = GetPosition(mColorBrightness);
    emit ColorChanged(GetColor());
    update();
  }

  // base method
  QWidget::mouseMoveEvent(aEvent);
}


void ggColorBrightnessWidget::resizeEvent(QResizeEvent* aEvent)
{
  // layout
  mLayout = aEvent->size().width() > aEvent->size().height() ? cLayout::eHorizontal : cLayout::eVertical;

  // color bar corners
  mColorBar = QRectF(mSelectorRadius, mSelectorRadius,
                     aEvent->size().width() - 2.0f * mSelectorRadius,
                     aEvent->size().height() - 2.0f * mSelectorRadius);

  // position of the selected color
  mColorPosition = GetPosition(mColorBrightness);

  // base method
  QWidget::resizeEvent(aEvent);
}


QBrush GradientBrush(const QPointF& aPositionA,
                     const QPointF& aPositionB,
                     const QColor& aColorA,
                     const QColor& aColorB)
{
  // setup gradient
  QLinearGradient vGradient(aPositionA, aPositionB);
  vGradient.setColorAt(0.0f, aColorA);
  vGradient.setColorAt(1.0f, aColorB);

  // return the gratent brush
  return QBrush(vGradient);
}


QBrush ggColorBrightnessWidget::GetGradientBrush() const
{
  if (mLayout == cLayout::eHorizontal) {
    return ::GradientBrush(mColorBar.topLeft(), mColorBar.topRight(), Qt::black, mColorSaturized);
  }
  if (mLayout == cLayout::eVertical) {
    return ::GradientBrush(mColorBar.bottomLeft(), mColorBar.topLeft(), Qt::black, mColorSaturized);
  }
  return QBrush(mColorSaturized);
}


QRectF ggColorBrightnessWidget::GetSelectorRect(float aSize) const
{
  float vSize2 = aSize / 2.0f;
  if (mLayout == cLayout::eHorizontal) {
    return QRectF(mColorPosition.x() - vSize2, mColorBar.top() - mSelectorRadius + 1.0f,
                  aSize, mColorBar.height() + 2.0f * mSelectorRadius - 2.0f);
  }
  if (mLayout == cLayout::eVertical) {
    return QRectF(mColorBar.x() - mSelectorRadius + 1.0f, mColorPosition.y() - vSize2,
                  mColorBar.width() + 2.0f * mSelectorRadius - 2.0f, aSize);
  }
  return QRectF(mColorPosition.x() - vSize2, mColorPosition.y() - vSize2,
                aSize, aSize);
}


void ggColorBrightnessWidget::paintEvent(QPaintEvent* aEvent)
{
  // the friendly painter
  QPainter vPainter(this);
  vPainter.setRenderHint(QPainter::Antialiasing, true);

  // draw the bar
  vPainter.setPen(Qt::NoPen);
  vPainter.setBrush(GetGradientBrush());
  vPainter.drawRoundedRect(mColorBar, 2.0f * mSelectorRadius, 2.0f * mSelectorRadius);

  // indicator of selected color
  float vRadius = mMouseDragging ? mSelectorRadiusLarge : mSelectorRadius;
  vPainter.setPen(QPen(Qt::white, 1.5f));
  vPainter.drawEllipse(mColorPosition, vRadius + 1.0f, vRadius + 1.0f);
  vPainter.setPen(Qt::black);
  vPainter.setBrush(GetColor());
  vPainter.drawEllipse(mColorPosition, vRadius, vRadius);

  // base paint event
  QWidget::paintEvent(aEvent);
}




