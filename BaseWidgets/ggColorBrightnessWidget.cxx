// 0) include own header
#include "ggColorBrightnessWidget.h"

// 1) include system or QT
#include <QPainter>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"


ggColorBrightnessWidget::ggColorBrightnessWidget(QWidget* aParent) :
  QWidget(aParent),
  mColor(255, 150, 0, 255)
{
}


void ggColorBrightnessWidget::SetColor(const QColor& aColor)
{
  if (mColor != aColor) {
    mColor = aColor;
    update();
  }
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
  vPainter.setBrush(mColor);
  vPainter.drawRoundedRect(rect(), 5.0f, 5.0f);
  QWidget::paintEvent(aEvent);
}
