#include "BaseGraphics/ggConnectionPoint.h"

#include <math.h>


ggConnectionPoint::ggConnectionPoint() :
  mPosition(0.0f, 0.0f),
  mDirection(1.0f, 0.0f)
{
}


void ggConnectionPoint::SetPosition(const QPointF& aPosition)
{
  mPosition = aPosition;
}


const QPointF& ggConnectionPoint::GetPosition() const
{
  return mPosition;
}


void ggConnectionPoint::SetDirection(const QVector2D& aDirection)
{
  if (!aDirection.isNull()) mDirection = aDirection.normalized();
}


const QVector2D& ggConnectionPoint::GetDirection() const
{
  return mDirection;
}


float ggConnectionPoint::GetDirectionAngle() const
{
  if (mDirection.isNull()) return 0.0f;
  return atan2(mDirection.y(), mDirection.x());
}


void ggConnectionPoint::SetDirectionAngle(float aAngle)
{
  mDirection.setX(cos(aAngle));
  mDirection.setY(sin(aAngle));
}


void ggConnectionPoint::SetDirectionRight()
{
  SetDirectionAngle(0.0f);
}


void ggConnectionPoint::SetDirectionLeft()
{
  SetDirectionAngle(M_PI);
}


void ggConnectionPoint::SetDirectionUp()
{
  SetDirectionAngle(-M_PI_2);
}


void ggConnectionPoint::SetDirectionDown()
{
  SetDirectionAngle(M_PI_2);
}


QPointF ggConnectionPoint::GetControlPoint(float aDistance) const
{
  QPointF vControlPoint(mPosition);
  vControlPoint += (aDistance * mDirection).toPointF();
  return vControlPoint;
}
