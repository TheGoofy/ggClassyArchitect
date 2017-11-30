#ifndef GGCLASSYBOX_H
#define GGCLASSYBOX_H

#include "ggGraphicsManipulatorBarItemT.h"

class ggGraphicsTextItem;
class ggGraphicsCheckBoxItem;
class ggClassyGraphicsBoxItems;
class ggClassyClassBox;
class ggClassyClass;

class ggClassyGraphicsBoxItem :
  public ggGraphicsManipulatorBarItemT<>
{

public:

  ggClassyGraphicsBoxItem(const QRectF& aRect);
  ggClassyGraphicsBoxItem(ggClassyClassBox* aClassBox);

  void SetClassBox(ggClassyClassBox* aClassBox);
  ggClassyClassBox* GetClassBox() const;
  ggClassyClass* GetClass() const;

  void SetBoxItems(const ggClassyGraphicsBoxItems* aBoxItems);

  const ggSubject* GetSubjectHeight() const;
  QPointF GetPositionTopCenter() const;
  QPointF GetPositionBottomCenter() const;

protected:

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void Update(const ggSubject* aSubject) override;

private:

  void Construct();

  void UpdateLayout();
  void NotifyHeight();
  void UpdateClassRead();
  void UpdateClassWrite();
  void NotifyClassChange();
  void UpdateClassBoxRead();
  void UpdateClassBoxWrite();
  void NotifyClassBoxChange();

  ggGraphicsTextItem* mClassNameText;
  ggGraphicsTextItem* mMembersText;
  ggGraphicsTextItem* mCommentText;
  ggGraphicsCheckBoxItem* mMembersCheckBox;
  ggGraphicsCheckBoxItem* mCommentCheckBox;

  ggSubject mSubjectHeight;

  ggClassyClassBox* mClassBox;
  const ggClassyGraphicsBoxItems* mBoxItems;

};

#endif // GGCLASSYBOX_H
