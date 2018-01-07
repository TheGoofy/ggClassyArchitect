#ifndef GGCLASSYCONNECTIONMANIPULATORITEM_H
#define GGCLASSYCONNECTIONMANIPULATORITEM_H

// 1) include system or QT
#include <QGraphicsItem>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"
#include "BaseGraphics/ggConnectionPoint.h"

// 3) forward declarations
class ggClassyGraphicsBoxItem;
class ggGraphicsDecoratedPathItem;
class QGraphicsEllipseItem;
template <class T> class ggGraphicsHandleItemT;

class ggClassyConnectionManipulatorItem :
  public QGraphicsItem,
  public ggObserver
{
public:

  ggClassyConnectionManipulatorItem(ggClassyGraphicsBoxItem* aParent = nullptr);
  ggClassyConnectionManipulatorItem(float aSize, ggClassyGraphicsBoxItem* aParent = nullptr);

  void SetSize(float aSize);
  float GetSize() const;
  void SetPositionSrc(const QPointF& aPosition);
  void SetColor(const QColor& aColor);
  const QColor& GetColor() const;
  void ShowDragger();
  void HideDragger();

protected:

  virtual void Update(const ggSubject* aSubject) override;
  virtual QRectF boundingRect() const override;
  virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

private:

  void Construct(float aSize);

  ggClassyGraphicsBoxItem* GetSrcBoxItem();
  ggClassyGraphicsBoxItem* GetDstBoxItem();
  ggConnectionPoint GetConnectionSrc();
  ggConnectionPoint GetConnectionDst();
  void UpdatePath();

  QPointF mPositionSrc;
  ggGraphicsHandleItemT<QGraphicsEllipseItem>* mDraggerItem;
  ggGraphicsDecoratedPathItem* mPathItem;
  QColor mColor;

};

#endif // GGCLASSYCONNECTIONMANIPULATORITEM_H
