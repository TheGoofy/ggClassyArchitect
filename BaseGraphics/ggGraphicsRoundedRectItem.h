#ifndef GGGRAPHICSROUNDEDRECTITEM_H
#define GGGRAPHICSROUNDEDRECTITEM_H

// 1) include system or QT
#include <QGraphicsRectItem>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggGraphicsRoundedRectItem :
  public QGraphicsRectItem
{
public:

  ggGraphicsRoundedRectItem(QGraphicsItem* aParent = nullptr);

  ggGraphicsRoundedRectItem(const QRectF& aRect,
                            const qreal aRadius = 3.0f,
                            QGraphicsItem* aParent = nullptr);

  qreal GetRadiusX() const;
  qreal GetRadiusY() const;
  void SetRadius(qreal aRadius);
  void SetRadius(qreal aRadiusX, qreal aRadiusY);
  QRectF GetInnerRect() const;
  void SetInnerRect(const QRectF& aRect);

protected:

  virtual void paint(QPainter* aPainter,
                     const QStyleOptionGraphicsItem* aOption,
                     QWidget* aWidget) override;

  QPainterPath shape() const override;

private:

  qreal mRadiusX;
  qreal mRadiusY;

};

#endif // GGGRAPHICSROUNDEDRECTITEM_H
