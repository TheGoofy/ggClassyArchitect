// 0) include own header
#include "ggColorPreviewWidget.h"

// 1) include system or QT
#include <QPainter>
#include <QBitmap>
#include <QResizeEvent>

// 2) include own project-related (sort by component dependency)
#include "BaseWidgets/ggUtilityQt.h"


ggColorPreviewWidget::ggColorPreviewWidget(QWidget* aParent) :
  QWidget(aParent),
  mSelectorRadius(3.0f),
  mSelectorRadiusLarge(9.0f),
  mLayout(cLayout::eVertical),
  mClickable(false)
{
  setMouseTracking(true);
  SetHorizontal();
  SetColor(QColor(200, 150, 50, 255));
}


void ggColorPreviewWidget::SetColor(const QColor& aColor)
{
  // update colors, if changed
  if (aColor != mColor) {
    mColor = aColor;
    update(); // trigger repaint
  }
}


const QColor& ggColorPreviewWidget::GetColor() const
{
  return mColor;
}


void ggColorPreviewWidget::SetHorizontal()
{
  if (mLayout != cLayout::eHorizontal) {
    mLayout = cLayout::eHorizontal;
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    update(); // trigger repaint
  }
}


void ggColorPreviewWidget::SetVertical()
{
  if (mLayout != cLayout::eVertical) {
    mLayout = cLayout::eVertical;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    update(); // trigger repaint
  }
}


bool ggColorPreviewWidget::IsHorizontal() const
{
  return mLayout == cLayout::eHorizontal;
}


bool ggColorPreviewWidget::IsVertical() const
{
  return mLayout == cLayout::eVertical;
}


void ggColorPreviewWidget::SetClickable(bool aClickable)
{
  if (mClickable != aClickable) {
    mClickable = aClickable;
  }
}


bool ggColorPreviewWidget::IsClickable() const
{
  return mClickable;
}


QSize ggColorPreviewWidget::sizeHint() const
{
  int vSize = 2.0f * mSelectorRadiusLarge;
  QSize vSizeHint(vSize, vSize);
  if (IsHorizontal()) vSizeHint += QSize(vSize, 0);
  if (IsVertical()) vSizeHint += QSize(0, vSize);
  return vSizeHint;
}


void ggColorPreviewWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  // adjust mouse pointer if insied preview area
  setCursor(mClickable && mColorIndicatorLarge.contains(aEvent->pos()) ? Qt::PointingHandCursor : Qt::ArrowCursor);

  // call base function
  QWidget::mouseMoveEvent(aEvent);
}


void ggColorPreviewWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
  if (mClickable && mColorIndicatorLarge.contains(aEvent->pos())) emit Clicked();
  else QWidget::mouseReleaseEvent(aEvent);
}


void ggColorPreviewWidget::resizeEvent(QResizeEvent* aEvent)
{
  // color bar corners
  mColorBar = QRectF(mSelectorRadius, mSelectorRadius,
                     aEvent->size().width() - 2.0f * mSelectorRadius,
                     aEvent->size().height() - 2.0f * mSelectorRadius);

  // color indicator
  mColorIndicatorSmall = GetIndicatorRect(1.0);
  mColorIndicatorLarge = GetIndicatorRect(0.0);

  // base method
  QWidget::resizeEvent(aEvent);
}


QRectF ggColorPreviewWidget::GetIndicatorRect(qreal aMargin) const
{
  const QRect& vRect(rect());
  if (mLayout == cLayout::eHorizontal) {
    return QRectF(mColorBar.left() + mColorBar.width() / 5.0 + aMargin,
                  vRect.top() + aMargin,
                  mColorBar.width() * 3.0 / 5.0 - 2.0 * aMargin,
                  vRect.height() - 2.0f * aMargin);
  }
  else {
    return QRectF(vRect.left() + aMargin,
                  mColorBar.top() + mColorBar.height() / 5.0 + aMargin,
                  vRect.width() - 2.0 * aMargin,
                  mColorBar.height() * 3.0 / 5.0 - 2.0f * aMargin);
  }
}


QRectF ggColorPreviewWidget::GetColorBarLowerRect() const
{
  QRectF vRect(mColorBar);
  if (mLayout == cLayout::eHorizontal) {
    vRect.setWidth(mColorBar.width() / 2.0);
  }
  else {
    vRect.setHeight(mColorBar.height() / 2.0);
    vRect.translate(0.0, mColorBar.height() / 2.0);
  }
  return vRect;
}


void ggColorPreviewWidget::paintEvent(QPaintEvent* aEvent)
{
  // the friendly painter
  QPainter vPainter(this);
  vPainter.setPen(Qt::NoPen);
  vPainter.setRenderHint(QPainter::Antialiasing, true);

  // draw main color
  qreal vRadius = 2.0 * mSelectorRadius;
  vPainter.setBrush(ggUtilityQt::GetColorWithAlpha(mColor, (mColor.alpha() != 255 ? 1.0f : 0.33f)));
  vPainter.drawRoundedRect(mColorBar, vRadius, vRadius);

  // draw checkered background (if the color is transparent)
  if (mColor.alpha() != 255) {
    QRectF vColorBarLowerRect(GetColorBarLowerRect());
    vPainter.setBrush(QColor(130, 130, 130, 255));
    vPainter.drawRoundedRect(vColorBarLowerRect, vRadius, vRadius);
    vPainter.drawRoundedRect(mColorIndicatorSmall, mSelectorRadiusLarge, mSelectorRadiusLarge);
    vPainter.setBrush(ggUtilityQt::GetCheckerBoardBrush(1.5f * mSelectorRadius));
    vPainter.drawRoundedRect(vColorBarLowerRect, vRadius, vRadius);
    vPainter.drawRoundedRect(mColorIndicatorSmall, mSelectorRadiusLarge, mSelectorRadiusLarge);
  }

  // indicator of selected color
  vRadius = mSelectorRadiusLarge - 1.0;
  vPainter.setPen(QPen(Qt::white, 1.5f));
  vPainter.setBrush(Qt::NoBrush);
  vPainter.drawRoundedRect(mColorIndicatorLarge, vRadius + 1.0, vRadius + 1.0);
  vPainter.setPen(Qt::black);
  vPainter.setBrush(GetColor());
  vPainter.drawRoundedRect(mColorIndicatorSmall, vRadius, vRadius);

  // grey out, if disabled
  if (!isEnabled()) {
    vPainter.setPen(Qt::NoPen);
    const QColor& vBackgroundColor = palette().color(QPalette::Disabled, QPalette::Window);
    vPainter.setBrush(ggUtilityQt::GetColorWithAlpha(vBackgroundColor, 0.75f));
    vPainter.drawRect(rect());
  }

  // base paint event
  QWidget::paintEvent(aEvent);
}
