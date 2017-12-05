#include "ggClassyGraphicsView.h"

#include <QObject>
#include <QDebug>
#include <QWheelEvent>
#include <QTextEdit>
#include <map>
#include <algorithm>

#include "ggClassyApplication.h"
#include "ggClassyGraphicsBoxItem.h"
#include "ggClassyDataSet.h"
#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyGraphicsScene.h"


ggClassyGraphicsView::ggClassyGraphicsView(QWidget* aParent)
 : QGraphicsView(aParent)
{
  setRenderHint(QPainter::Antialiasing);

  ggClassyGraphicsScene* vScene = new ggClassyGraphicsScene(this);
  setScene(vScene);
  //QRectF vRect(-200, -200, 400, 400);
  //scene()->addRect(vRect);
  //scene()->setSceneRect(vRect);

  ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

  ggClassyGraphicsBoxItem* vClassBoxItem = new ggClassyGraphicsBoxItem(QRectF(0, 150, 200, 200));
  vScene->addItem(vClassBoxItem);

  vScene->AddClassBoxItems(vDataSet);
  vScene->AddLineItems(vDataSet);

  ggClassyApplication::GetInstance().Zoom().SetValue(GetSceneScale());
  Attach(&ggClassyApplication::GetInstance().Zoom());
}


void ggClassyGraphicsView::NotifyZoom()
{
  ggClassyApplication::GetInstance().Zoom().SetValue(GetSceneScale());
  ggClassyApplication::GetInstance().Zoom().Notify(this);
}


void ggClassyGraphicsView::SetZoomFit()
{
  if (scene() != nullptr) {
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    //fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
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


void ggClassyGraphicsView::wheelEvent(QWheelEvent* aWheelEvent)
{
  if (aWheelEvent->delta() != 0) {
    if (aWheelEvent->modifiers() & Qt::ShiftModifier) {
      aWheelEvent->accept();
      float vScale = 1.0f;
      if (aWheelEvent->delta() > 0) vScale = 1.05f / 1.0f;
      if (aWheelEvent->delta() < 0) vScale = 1.0f / 1.05f;
      scale(vScale, vScale);
      NotifyZoom();
    }
    if (aWheelEvent->modifiers() & Qt::AltModifier) {
      aWheelEvent->accept();
      float vAngle = aWheelEvent->delta() > 0 ? 5.0f : -5.0f;
      rotate(vAngle);
    }
  }
  if (!aWheelEvent->isAccepted()) {
    QGraphicsView::wheelEvent(aWheelEvent);
  }
}
