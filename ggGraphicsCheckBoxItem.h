#ifndef GGGRAPHICSCHECKBOXITEM_H
#define GGGRAPHICSCHECKBOXITEM_H

#include <QGraphicsRectItem>

#include "ggSubjectT.h"

class ggGraphicsCheckBoxItem :
  public QGraphicsRectItem
{

public:

  ggGraphicsCheckBoxItem(QGraphicsItem* aParent = nullptr);

  void SetChecked(bool aChecked);
  bool GetChecked() const;
  const ggSubjectBool* GetSubjectChecked() const;
  void SetSize(float aSize);
  void SetCrossLineWidth(float aWidth);
  void SetColor(const QColor& aColor);
  void SetHighlightOn();
  void SetHighlightOff();

protected:

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;

private:

  void UpdateCross();
  void UpdateColor();

  ggSubjectBool mChecked;
  QColor mColor;

  QGraphicsLineItem* mCrossLineA;
  QGraphicsLineItem* mCrossLineB;

};

#endif // GGGRAPHICSCHECKBOXITEM_H
