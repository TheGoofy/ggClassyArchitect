#ifndef GGCONNECTIONPOINT_H
#define GGCONNECTIONPOINT_H

// 1) include system or QT
#include <QPointF>
#include <QVector2D>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

/**
 * @brief The ggConnectionPoint class
 */
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
