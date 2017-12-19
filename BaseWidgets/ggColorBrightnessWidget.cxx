// 0) include own header
#include "ggColorBrightnessWidget.h"

// 1) include system or QT
#include <QPainter>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"
#include "BaseWidgets//ggUtilityQt.h"


ggColorBrightnessWidget::ggColorBrightnessWidget(QWidget* aParent) :
  QWidget(aParent)
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
    update(); // trigger repaint
  }
}


QColor ggColorBrightnessWidget::GetColor() const
{
  return ggUtilityQt::GetColorScaled(mColorSaturized, mColorBrightness);
}


void ggColorBrightnessWidget::mousePressEvent(QMouseEvent* aEvent)
{
  QWidget::mousePressEvent(aEvent);
}


void ggColorBrightnessWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
  QWidget::mouseReleaseEvent(aEvent);
}


void ggColorBrightnessWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  QWidget::mouseMoveEvent(aEvent);
}


void ggColorBrightnessWidget::resizeEvent(QResizeEvent* aEvent)
{
  QWidget::resizeEvent(aEvent);
}


void ggColorBrightnessWidget::paintEvent(QPaintEvent* aEvent)
{
  QPainter vPainter(this);
  vPainter.setRenderHint(QPainter::Antialiasing, true);
  vPainter.setPen(Qt::NoPen);
  vPainter.setBrush(mColorSaturized);
  vPainter.drawRoundedRect(rect(), 5.0f, 5.0f);
  QWidget::paintEvent(aEvent);
}
