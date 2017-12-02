#ifndef GGCONNECTIONPOINT_H
#define GGCONNECTIONPOINT_H

#include <QPointF>
#include <QVector2D>

class ggConnectionPoint
{
public:

  ggConnectionPoint();

  void SetPosition(const QPointF& aPosition);
  const QPointF& GetPosition() const;

  void SetDirection(const QVector2D& aDirection);
  const QVector2D& GetDirection() const;

  float GetDirectionAngle() const;
  void SetDirectionAngle(float aAngle);

  void SetDirectionRight();
  void SetDirectionLeft();
  void SetDirectionUp();
  void SetDirectionDown();

  QPointF GetControlPoint(float aDistance) const;

private:

  QPointF mPosition;
  QVector2D mDirection;

};

#endif // GGCONNECTIONPOINT_H
