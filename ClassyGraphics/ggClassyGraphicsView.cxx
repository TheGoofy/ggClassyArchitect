#include "ggClassyGraphicsView.h"

#include "Base/ggUtility.h"
#include "ClassyData/ggClassyDataSet.h"
#include "ClassyGraphics/ggClassyGraphicsBoxItem.h"
#include "ClassyGraphics/ggClassyGraphicsScene.h"
#include "ClassyMain/ggClassyApplication.h"


ggClassyGraphicsView::ggClassyGraphicsView(QWidget* aParent)
 : QGraphicsView(aParent),
   mMouseDrag(false),
   mZoomResetOnResize(false)
{
  // suppress automatic scene centering
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  QRectF vRect(-10000.0f, -10000.0f, 20000.0f, 20000.0f);
  setSceneRect(vRect);

  // make some objects (for development and testing)
  ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();
  ggClassyGraphicsBoxItem* vClassBoxItem = new ggClassyGraphicsBoxItem(QRectF(0, 150, 200, 200));

  // add these objects to the scene (for development and testing)
  ggClassyGraphicsScene* vScene = new ggClassyGraphicsScene(this);
  vScene->addItem(vClassBoxItem);
  vScene->AddClassBoxItems(vDataSet);
  vScene->AddLineItems();
  vScene->AddTestConnections();
  setScene(vScene);

  // register subject zoom
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

    // zoom at mouse pointer position
    if (aWheelEvent->modifiers() == Qt::NoModifier) {
      aWheelEvent->accept();
      float vScale = 1.0f;
      if (aWheelEvent->delta() > 0) vScale = 1.1f / 1.0f;
      if (aWheelEvent->delta() < 0) vScale = 1.0f / 1.1f;
      vScale = ggUtility::RoundToOMG(vScale * GetSceneScale(), 2);
      QPointF vPosA = mapToScene(aWheelEvent->pos());
      SetSceneScale(vScale);
      QPointF vPosB = mapToScene(aWheelEvent->pos());
      QPointF vDeltaPos = vPosB - vPosA;
      translate(vDeltaPos.x(), vDeltaPos.y());
      NotifyZoom();
    }

    // rotate around mouse pointer position
    if (aWheelEvent->modifiers() & Qt::AltModifier) {
      aWheelEvent->accept();
      float vAngle = aWheelEvent->delta() > 0 ? -5.0f : 5.0f;
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


void ggClassyGraphicsView::setScene(QGraphicsScene* aScene)
{
  mZoomResetOnResize = true;
  QGraphicsView::setScene(aScene);
}


QPoint ggClassyGraphicsView::ToPoint(const QSize& aSize) const
{
  return QPoint(aSize.width(), aSize.height());
}


void ggClassyGraphicsView::resizeEvent(QResizeEvent* aEvent)
{
  // fix the center
  QPointF vCenterDelta = mapToScene(ToPoint(aEvent->size())) - mapToScene(ToPoint(aEvent->oldSize()));
  setTransform(transform().translate(vCenterDelta.x() / 2.0f, vCenterDelta.y() / 2.0f));

  // reset zoom and center scene
  if (mZoomResetOnResize) {
    mZoomResetOnResize = false;
    SetZoomReset();
  }

  // execute event on base
  QGraphicsView::resizeEvent(aEvent);
}
