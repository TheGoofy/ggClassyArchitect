#ifndef GGCLASSYGRAPHICSVIEW_H
#define GGCLASSYGRAPHICSVIEW_H

// 1) include system or QT
#include <QGraphicsView>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubjectT.h"
#include "Base/ggObserver.h"

// 3) forward declarations

/**
 * @brief The ggGraphicsView class
 */
class ggGraphicsView :
  public QGraphicsView,
  public ggObserver
{
  Q_OBJECT

public:

  explicit ggGraphicsView(QWidget* aParent = nullptr);

  ggSubjectFloat* GetSubjectZoom();
  void SetZoomReset();
  void SetZoomFit();

  void setScene(QGraphicsScene* aScene);

protected:

  virtual void Update(const ggSubject* aSubject) override;

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  virtual void wheelEvent(QWheelEvent* aWheelEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;

private:

  void NotifyZoom();
  float GetSceneScale() const;
  void SetSceneScale(float aSceneScale);
  QPoint ToPoint(const QSize& aSize) const;

  ggSubjectFloat mSubjectZoom;

  bool mMouseDrag;
  QPoint mMouseDragStartPos;
  QTransform mMouseDragStartTransform;
  bool mZoomResetOnResize;

};

#endif // GGCLASSYGRAPHICSVIEW_H
