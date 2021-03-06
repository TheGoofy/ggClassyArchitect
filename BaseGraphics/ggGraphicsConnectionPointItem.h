#ifndef GGGRAPHICSCONNECTIONPOINTITEM_H
#define GGGRAPHICSCONNECTIONPOINTITEM_H

// 1) include system or QT
#include <QGraphicsEllipseItem>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"
#include "BaseGraphics/ggSubjectConnectionPoint.h"

// 3) forward declarations

/**
 * @brief The ggGraphicsConnectionPointItem class
 */
class ggGraphicsConnectionPointItem :
  public QGraphicsEllipseItem,
  private ggObserver
{

public:

  ggGraphicsConnectionPointItem(float aSize = 11.0f,
                                QGraphicsItem* aParent = nullptr);

  void SetSize(float aSize);
  float GetSize() const;

  void SetPointPosition(const QPointF& aPosition);

  void SetPointDirection(float aAngle);
  void SetPointDirection(const QVector2D& aDirection);
  void SetPointDirectionRight();
  void SetPointDirectionLeft();
  void SetPointDirectionUp();
  void SetPointDirectionDown();

  const ggSubjectConnectionPoint* GetSubjectConnectionPoint() const;

protected:

  virtual void Update(const ggSubject* aSubject) override;

  virtual QVariant itemChange(GraphicsItemChange aChange,
                              const QVariant& aValue) override;

  virtual void wheelEvent(QGraphicsSceneWheelEvent* aEvent) override;

private:

  void UpdateConnectionPoint();

  ggSubjectConnectionPoint mConnectionPoint;
  QGraphicsLineItem* mDirectionLine;

};

#endif // GGGRAPHICSCONNECTIONPOINTITEM_H
