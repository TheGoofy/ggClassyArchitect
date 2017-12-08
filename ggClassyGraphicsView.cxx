#include "ggClassyGraphicsView.h"

#include <QObject>
#include <QDebug>
#include <QWheelEvent>
#include <QTextEdit>
#include <QScrollBar>
#include <map>
#include <algorithm>

#include "ggClassyApplication.h"
#include "ggClassyGraphicsBoxItem.h"
#include "ggClassyDataSet.h"
#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyGraphicsScene.h"


ggClassyGraphicsView::ggClassyGraphicsView(QWidget* aParent)
 : QGraphicsView(aParent),
   mMouseDrag(false)
{
  // suppress automatic scene centering
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  QRectF vRect(-10000.0f, -10000.0f, 20000.0f, 20000.0f);
  setSceneRect(vRect);

  ggClassyGraphicsScene* vScene = new ggClassyGraphicsScene(this);
  setScene(vScene);

  ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

  ggClassyGraphicsBoxItem* vClassBoxItem = new ggClassyGraphicsBoxItem(QRectF(0, 150, 200, 200));
  vScene->addItem(vClassBoxItem);

  vScene->AddClassBoxItems(vDataSet);
  vScene->AddLineItems();
  vScene->AddTestConnections();

  ggClassyApplication::GetInstance().Zoom().SetValue(GetSceneScale());
  Attach(&ggClassyApplication::GetInstance().Zoom());
}


void ggClassyGraphicsView::NotifyZoom()
{
  ggClassyApplication::GetInstance().Zoom().SetValue(GetSceneScale());
  ggClassyApplication::GetInstance().Zoom().Notify(this);
}


void ggClassyGraphicsView::SetZoomReset()
{
  resetTransform();
  if (scene() != nullptr) {
    qDebug() << scene()->itemsBoundingRect().center();
    centerOn(scene()->itemsBoundingRect().center());
    NotifyZoom();
  }
}


void ggClassyGraphicsView::SetZoomFit()
{
  if (scene() != nullptr) {
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    NotifyZoom();
  }
}


float ggClassyGraphicsView::GetSceneScale() const
{
  return transform().mapRect(QRectF(0.0f,0.0f,1.0f,1.0f)).width();
}


void ggClassyGraphicsView::SetSceneScale(float aSceneScale)
{
  float vScale = aSceneScale / GetSceneScale();
  setTransform(transform().scale(vScale, vScale));
}


void ggClassyGraphicsView::Update(const ggSubject* aSubject)
{
  if (aSubject == &ggClassyApplication::GetInstance().Zoom()) {
    SetSceneScale(ggClassyApplication::GetInstance().Zoom().GetValue());
  }
}


void ggClassyGraphicsView::mousePressEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton) {
    mMouseDrag = true;
    mMouseDragStartPos = aEvent->pos();
    mMouseDragStartTransform = transform();
  }
  QGraphicsView::mousePressEvent(aEvent);
}


void ggClassyGraphicsView::mouseMoveEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton && mMouseDrag) {
    QPointF vDeltaPos = mapToScene(aEvent->pos()) - mapToScene(mMouseDragStartPos);
    QTransform vTransform(mMouseDragStartTransform);
    setTransform(vTransform.translate(vDeltaPos.x(), vDeltaPos.y()));
  }
  QGraphicsView::mouseMoveEvent(aEvent);
}


void ggClassyGraphicsView::mouseReleaseEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton) {
    mMouseDrag = false;
  }
  QGraphicsView::mouseReleaseEvent(aEvent);
}


void ggClassyGraphicsView::wheelEvent(QWheelEvent* aWheelEvent)
{
  if (aWheelEvent->delta() != 0) {
    if (aWheelEvent->modifiers() == Qt::NoModifier) {
      aWheelEvent->accept();
      float vScale = 1.0f;
      if (aWheelEvent->delta() > 0) vScale = 1.05f / 1.0f;
      if (aWheelEvent->delta() < 0) vScale = 1.0f / 1.05f;
      QPointF vPosA = mapToScene(aWheelEvent->pos());
      scale(vScale, vScale);
      QPointF vPosB = mapToScene(aWheelEvent->pos());
      QPointF vDeltaPos = vPosB - vPosA;
      translate(vDeltaPos.x(), vDeltaPos.y());
      NotifyZoom();
    }
    if (aWheelEvent->modifiers() & Qt::AltModifier) {
      aWheelEvent->accept();
      float vAngle = aWheelEvent->delta() > 0 ? 5.0f : -5.0f;
      QPointF vPos = mapToScene(aWheelEvent->pos());
      translate(vPos.x(), vPos.y());
      rotate(vAngle);
      translate(-vPos.x(), -vPos.y());
    }
  }
  if (!aWheelEvent->isAccepted()) {
    QGraphicsView::wheelEvent(aWheelEvent);
  }
}
