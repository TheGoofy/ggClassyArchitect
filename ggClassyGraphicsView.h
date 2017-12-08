#ifndef GGCLASSYGRAPHICSVIEW_H
#define GGCLASSYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>
#include <QInputMethodEvent>

#include "ggObserver.h"

class ggClassyGraphicsView :
  public QGraphicsView,
  public ggObserver
{
  Q_OBJECT

public:

  explicit ggClassyGraphicsView(QWidget* aParent = nullptr);
  void SetZoomReset();
  void SetZoomFit();

protected:

  virtual void Update(const ggSubject* aSubject) override;

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  void wheelEvent(QWheelEvent* aWheelEvent) override;

private:

  float GetSceneScale() const;
  void SetSceneScale(float aSceneScale);
  void NotifyZoom();

  bool mMouseDrag;
  QPoint mMouseDragStartPos;
  QTransform mMouseDragStartTransform;

};

#endif // GGCLASSYGRAPHICSVIEW_H
