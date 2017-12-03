#ifndef GGCLASSYBOX_H
#define GGCLASSYBOX_H

#include "ggTypes.h"
#include "ggGraphicsManipulatorBarItemT.h"
#include "ggSubjectConnectionPoint.h"

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

  const ggSubjectConnectionPoint* GetClassConnectionTop() const;
  const ggSubjectConnectionPoint* GetClassConnectionBottom() const;
  const ggSubjectConnectionPoint* GetClassConnectionLeft() const;
  const ggSubjectConnectionPoint* GetClassConnectionRight() const;
  const ggSubjectConnectionPoint* GetMemberConnectionLeft(ggUSize aMemberIndex) const;
  const ggSubjectConnectionPoint* GetMemberConnectionRight(ggUSize aMemberIndex) const;

protected:

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void Update(const ggSubject* aSubject) override;

private:

  void Construct();

  QPointF GetClassPositionTop() const;
  QPointF GetClassPositionBottom() const;
  QPointF GetClassPositionLeft() const;
  QPointF GetClassPositionRight() const;
  QPointF GetMemberPositionLeft(ggUSize aMemberIndex) const;
  QPointF GetMemberPositionRight(ggUSize aMemberIndex) const;

  void UpdateLayout();
  void NotifySize();

  void UpdateClassRead();
  void UpdateClassWrite();
  void NotifyClassChange();

  void UpdateClassBoxRead();
  void UpdateClassBoxWrite();
  void NotifyClassBoxChange();

  void UpdateConnectionPoints();
  void NotifyConnectionPoints();

  ggGraphicsTextItem* mClassNameText;
  ggGraphicsTextItem* mMembersText;
  ggGraphicsTextItem* mCommentText;
  ggGraphicsCheckBoxItem* mMembersCheckBox;
  ggGraphicsCheckBoxItem* mCommentCheckBox;

  ggSubject mSubjectSize;

  typedef std::vector<ggSubjectConnectionPoint> tSubjectConnectionPoints;

  ggSubjectConnectionPoint mClassConnectionTop;
  ggSubjectConnectionPoint mClassConnectionBottom;
  ggSubjectConnectionPoint mClassConnectionLeft;
  ggSubjectConnectionPoint mClassConnectionRight;
  tSubjectConnectionPoints mMembersConnectionLeft;
  tSubjectConnectionPoints mMembersConnectionRight;

  ggClassyClassBox* mClassBox;

};

#endif // GGCLASSYBOX_H
