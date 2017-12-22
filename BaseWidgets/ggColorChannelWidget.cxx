// 0) include own header
#include "ggColorChannelWidget.h"

// 1) include system or QT
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QBitmap>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"
#include "BaseWidgets//ggUtilityQt.h"


ggColorChannelWidget::ggColorChannelWidget(QWidget* aParent) :
  QWidget(aParent),
  mChannel(cChannel::eBrightness),
  mSelectorRadius(3.0f),
  mSelectorRadiusLarge(9.0f),
  mMouseDragging(false),
  mLayout(cLayout::eHorizontal)
{
  setMouseTracking(true);
  SetColor(QColor(200, 150, 0, 255));
}


void ggColorChannelWidget::SetColor(const QColor& aColor)
{
  // update colors, if changed
  if (aColor != mColor) {
    mColor = aColor;
    mChannelValue = GetChannelValue(mColor);
    mColorPosition = GetPosition(mChannelValue);
    update(); // trigger repaint
  }
}


void ggColorChannelWidget::SetChannel(cChannel aChannel)
{
  if (aChannel != mChannel) {
    mChannel = aChannel;
    mChannelValue = GetChannelValue(mColor);
    mColorPosition = GetPosition(mChannelValue);
    update(); // trigger repaint
  }
}


ggColorChannelWidget::cChannel ggColorChannelWidget::GetChannel() const
{
  return mChannel;
}


float ggColorChannelWidget::GetChannelValue(const QColor& aColor) const
{
  switch (mChannel) {
    case cChannel::eRed: return aColor.redF();
    case cChannel::eGreen: return aColor.greenF();
    case cChannel::eBlue: return aColor.blueF();
    case cChannel::eAlpha: return aColor.alphaF();
    case cChannel::eBrightness: return ggUtilityQt::GetColorBrightness(aColor);
    default: return 0.0f;
  }
}


QColor ggColorChannelWidget::GetColor() const
{
  switch (mChannel) {
    case cChannel::eRed: return QColor::fromRgbF(mChannelValue, mColor.greenF(), mColor.blueF(), mColor.alphaF());
    case cChannel::eGreen: return QColor::fromRgbF(mColor.redF(), mChannelValue, mColor.blueF(), mColor.alphaF());
    case cChannel::eBlue: return QColor::fromRgbF(mColor.redF(), mColor.greenF(), mChannelValue, mColor.alphaF());
    case cChannel::eAlpha: return QColor::fromRgbF(mColor.redF(), mColor.greenF(), mColor.blueF(), mChannelValue);
    case cChannel::eBrightness: return ggUtilityQt::GetColorScaled(ggUtilityQt::GetColorSaturized(mColor), mChannelValue);
    default: return mColor;
  }
}


QColor ggColorChannelWidget::GetColorMax() const
{
  switch (mChannel) {
    case cChannel::eRed: return QColor(Qt::red);
    case cChannel::eGreen: return QColor(Qt::green);
    case cChannel::eBlue: return QColor(Qt::blue);
    case cChannel::eAlpha: return ggUtilityQt::GetColorAlpha(ggUtilityQt::GetColorSaturized(mColor), 1.0f);
    case cChannel::eBrightness: return ggUtilityQt::GetColorAlpha(ggUtilityQt::GetColorSaturized(mColor), 1.0f);
    default: return mColor;
  }
}


QColor ggColorChannelWidget::GetColorMin() const
{
  switch (mChannel) {
    case cChannel::eRed: return QColor(Qt::black);
    case cChannel::eGreen: return QColor(Qt::black);
    case cChannel::eBlue: return QColor(Qt::black);
    case cChannel::eAlpha: return ggUtilityQt::GetColorAlpha(ggUtilityQt::GetColorSaturized(mColor), 0.0f);
    case cChannel::eBrightness: return QColor(Qt::black);
    default: return QColor(Qt::black);
  }
}


QPointF ggColorChannelWidget::ClampPosition(const QPointF& aPosition) const
{
  return QPointF(ggUtility::Clamp(aPosition.x(), mColorBar.x(), mColorBar.right()),
                 ggUtility::Clamp(aPosition.y(), mColorBar.y(), mColorBar.bottom()));
}


QPointF ggColorChannelWidget::GetPosition(const float aChannelValue) const
{
  if (mLayout == cLayout::eHorizontal) {
    return QPointF(mColorBar.x() + aChannelValue * mColorBar.width(), mColorBar.center().y());
  }
  if (mLayout == cLayout::eVertical) {
    return QPointF(mColorBar.center().x(), mColorBar.bottom() - aChannelValue * mColorBar.height());
  }
  return mColorBar.center();
}


float ggColorChannelWidget::GetChannelValue(const QPointF& aPosition) const
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


bool ggColorChannelWidget::IsInside(const QPointF& aPosition) const
{
  return mColorBar.contains(aPosition);
}


void ggColorChannelWidget::mousePressEvent(QMouseEvent* aEvent)
{
  // change selected color
  if (aEvent->button() == Qt::LeftButton) {
    mMouseDragging = true;
    setCursor(Qt::BlankCursor);
    mChannelValue = GetChannelValue(aEvent->pos());
    mColorPosition = GetPosition(mChannelValue);
    emit ColorChanged(GetColor());
    update();
  }

  // base method
  QWidget::mousePressEvent(aEvent);
}


void ggColorChannelWidget::mouseReleaseEvent(QMouseEvent* aEvent)
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


void ggColorChannelWidget::mouseMoveEvent(QMouseEvent* aEvent)
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
    mChannelValue = GetChannelValue(aEvent->pos());
    mColorPosition = GetPosition(mChannelValue);
    emit ColorChanged(GetColor());
    update();
  }

  // base method
  QWidget::mouseMoveEvent(aEvent);
}


void ggColorChannelWidget::resizeEvent(QResizeEvent* aEvent)
{
  // layout
  mLayout = aEvent->size().width() > aEvent->size().height() ? cLayout::eHorizontal : cLayout::eVertical;

  // color bar corners
  mColorBar = QRectF(mSelectorRadius, mSelectorRadius,
                     aEvent->size().width() - 2.0f * mSelectorRadius,
                     aEvent->size().height() - 2.0f * mSelectorRadius);

  // position of the selected color
  mColorPosition = GetPosition(mChannelValue);

  // base method
  QWidget::resizeEvent(aEvent);
}


QBrush CheckerBoardBrush(int aSize, const QColor& aColor)
{
  QBitmap vBitmap(2*aSize, 2*aSize);
  QPainter vBitmapPainter(&vBitmap);
  vBitmapPainter.setPen(Qt::NoPen);
  vBitmapPainter.setBrush(Qt::black);
  vBitmapPainter.drawRect(    0,     0, aSize, aSize);
  vBitmapPainter.drawRect(aSize, aSize, aSize, aSize);
  vBitmapPainter.setBrush(Qt::white);
  vBitmapPainter.drawRect(aSize,     0, aSize, aSize);
  vBitmapPainter.drawRect(    0, aSize, aSize, aSize);
  QBrush vBrush(vBitmap);
  vBrush.setColor(aColor);
  return vBrush;
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


QBrush ggColorChannelWidget::GetGradientBrush() const
{
  if (mLayout == cLayout::eHorizontal) {
    return ::GradientBrush(mColorBar.topLeft(), mColorBar.topRight(),
                           GetColorMin(), GetColorMax());
  }
  if (mLayout == cLayout::eVertical) {
    return ::GradientBrush(mColorBar.bottomLeft(), mColorBar.topLeft(),
                           GetColorMin(), GetColorMax());
  }
  return QBrush(mColor);
}


QRectF ggColorChannelWidget::GetSelectorRect(float aSize) const
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


void ggColorChannelWidget::paintEvent(QPaintEvent* aEvent)
{
  // the friendly painter
  QPainter vPainter(this);
  vPainter.setRenderHint(QPainter::Antialiasing, true);

  // draw the bar
  vPainter.setPen(Qt::NoPen);
  vPainter.setBrush(QColor(100,100,100,255));
  vPainter.drawRoundedRect(mColorBar, 2.0f * mSelectorRadius, 2.0f * mSelectorRadius);
  vPainter.setBrush(CheckerBoardBrush(mColorBar.height()/3, QColor(150,150,150,255)));
  vPainter.drawRoundedRect(mColorBar, 2.0f * mSelectorRadius, 2.0f * mSelectorRadius);
  vPainter.setBrush(GetGradientBrush());
  vPainter.drawRoundedRect(mColorBar, 2.0f * mSelectorRadius, 2.0f * mSelectorRadius);
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




