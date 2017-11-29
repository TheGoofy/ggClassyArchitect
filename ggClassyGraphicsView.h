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
  void SetZoomFit();

protected:
  virtual void Update(const ggSubject* aSubject) override;
  void wheelEvent(QWheelEvent* aWheelEvent) override;

  void inputMethodEvent(QInputMethodEvent* aEvent) override
  {
    qDebug() << __PRETTY_FUNCTION__ << aEvent->type();
    QGraphicsView::inputMethodEvent(aEvent);
  }


private:
  float GetSceneScale() const;
  void SetSceneScale(float aSceneScale);
  void NotifyZoom();

};

#endif // GGCLASSYGRAPHICSVIEW_H
