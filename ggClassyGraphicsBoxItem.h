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

  const ggSubject* GetSubjectSize() const;
  QPointF GetPositionTopCenter() const;
  QPointF GetPositionBottomCenter() const;
  QPointF GetClassPositionLeft() const;
  QPointF GetClassPositionRight() const;
  QPointF GetMemberPositionLeft(int aMemberIndex) const;
  QPointF GetMemberPositionRight(int aMemberIndex) const;

protected:

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void Update(const ggSubject* aSubject) override;

private:

  void Construct();

  void UpdateLayout();
  void NotifySize();
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

  ggSubject mSubjectSize;

  ggClassyClassBox* mClassBox;

};

#endif // GGCLASSYBOX_H
