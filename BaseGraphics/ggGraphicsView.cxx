// 0) include own header
#include "ggGraphicsView.h"

// 1) include system or QT
#include <QMouseEvent>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"


ggGraphicsView::ggGraphicsView(QWidget* aParent)
 : QGraphicsView(aParent),
   mMouseDrag(false),
   mZoomResetOnResize(false)
{
  // suppress automatic scene centering
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setSceneRect(QRectF(-10000.0f, -10000.0f, 20000.0f, 20000.0f));

  // enable box-selection, and dragging multiple items
  setDragMode(RubberBandDrag);

  // attach subject(s)
  Attach(&mSubjectZoom);
}


void ggGraphicsView::NotifyZoom()
{
  mSubjectZoom.SetValue(GetSceneScale());
  mSubjectZoom.Notify(this);
}


ggSubjectFloat* ggGraphicsView::GetSubjectZoom()
{
  return &mSubjectZoom;
}


void ggGraphicsView::SetZoomReset()
{
  resetTransform();
  if (scene() != nullptr) {
    centerOn(scene()->itemsBoundingRect().center());
    NotifyZoom();
  }
}


void ggGraphicsView::SetZoomFit()
{
  if (scene() != nullptr) {
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    NotifyZoom();
  }
}


float ggGraphicsView::GetSceneScale() const
{
  return transform().mapRect(QRectF(0.0f,0.0f,1.0f,1.0f)).width();
}


void ggGraphicsView::SetSceneScale(float aSceneScale)
{
  float vScale = aSceneScale / GetSceneScale();
  setTransform(transform().scale(vScale, vScale));
}


void ggGraphicsView::Update(const ggSubject* aSubject)
{
  if (aSubject == &mSubjectZoom) {
    SetSceneScale(mSubjectZoom.GetValue());
  }
}


void ggGraphicsView::mousePressEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton) {
    mMouseDrag = true;
    mMouseDragStartPos = aEvent->pos();
    mMouseDragStartTransform = transform();
  }
  QGraphicsView::mousePressEvent(aEvent);
}


void ggGraphicsView::mouseMoveEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton && mMouseDrag) {
    QPointF vDeltaPos = mapToScene(aEvent->pos()) - mapToScene(mMouseDragStartPos);
    QTransform vTransform(mMouseDragStartTransform);
    setTransform(vTransform.translate(vDeltaPos.x(), vDeltaPos.y()));
  }
  QGraphicsView::mouseMoveEvent(aEvent);
}


void ggGraphicsView::mouseReleaseEvent(QMouseEvent* aEvent)
{
  if (aEvent->buttons() & Qt::RightButton) {
    mMouseDrag = false;
  }
  QGraphicsView::mouseReleaseEvent(aEvent);
}


void ggGraphicsView::wheelEvent(QWheelEvent* aWheelEvent)
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


void ggGraphicsView::setScene(QGraphicsScene* aScene)
{
  mZoomResetOnResize = true;
  QGraphicsView::setScene(aScene);
  SetZoomReset();
}


QPoint ggGraphicsView::ToPoint(const QSize& aSize) const
{
  return QPoint(aSize.width(), aSize.height());
}


void ggGraphicsView::resizeEvent(QResizeEvent* aEvent)
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
