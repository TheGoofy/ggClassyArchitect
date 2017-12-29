#ifndef GGCLASSYBOX_H
#define GGCLASSYBOX_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "BaseGraphics/ggGraphicsManipulatorBarItemT.h"
#include "BaseGraphics/ggSubjectConnectionPoint.h"

// 3) forward declarations
class ggGraphicsTextItem;
class ggGraphicsCheckBoxItem;
class ggGraphicsRectShadowItem;
class ggClassyClassBoxPoints;
class ggClassyClassBox;
class ggClassyClass;
class ggClassyCollection;
class ggClassyConnectionItem;

/**
 * @brief The ggClassyGraphicsBoxItem class
 */
class ggClassyGraphicsBoxItem :
  public ggGraphicsManipulatorBarItemT<>
{

public:

  ggClassyGraphicsBoxItem(const QRectF& aRect);
  ggClassyGraphicsBoxItem(ggClassyClass* aClass,
                          ggClassyClassBox* aClassBox);

  void SetClassBox(ggClassyClass* aClass,
                   ggClassyClassBox* aClassBox);

  ggClassyClass* GetClass() const;
  ggClassyClassBox* GetClassBox() const;  

  const ggSubject* GetSubjectSize() const;

  const ggSubjectConnectionPoint* GetClassConnectionTop() const;
  const ggSubjectConnectionPoint* GetClassConnectionBottom() const;
  const ggSubjectConnectionPoint* GetClassConnectionLeft() const;
  const ggSubjectConnectionPoint* GetClassConnectionRight() const;
  const ggSubjectConnectionPoint* GetMemberConnectionLeft(ggUSize aMemberIndex) const;
  const ggSubjectConnectionPoint* GetMemberConnectionRight(ggUSize aMemberIndex) const;

protected:

  virtual void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget) override;
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) override;

  virtual void Update(const ggSubject* aSubject) override;

private:

  void Construct();

  QPointF GetClassPositionTop() const;
  QPointF GetClassPositionBottom() const;
  QPointF GetClassPositionLeft() const;
  QPointF GetClassPositionRight() const;
  QPointF GetMemberPositionLeft(ggUSize aMemberIndex) const;
  QPointF GetMemberPositionRight(ggUSize aMemberIndex) const;
  const ggClassyCollection* GetCollection() const;

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

  void UpdateCollectionRead();

  ggGraphicsRectShadowItem* mShadow;
  QGraphicsRectItem* mBoxBorder;
  ggGraphicsTextItem* mClassNameText;
  ggGraphicsTextItem* mMembersText;
  ggGraphicsTextItem* mDescriptionText;
  ggGraphicsCheckBoxItem* mMembersCheckBox;
  ggGraphicsCheckBoxItem* mDescriptionCheckBox;
  ggClassyConnectionItem* mBaseClassConnector;

  ggSubject mSubjectSize;

  typedef std::vector<ggSubjectConnectionPoint> tSubjectConnectionPoints;

  ggSubjectConnectionPoint mClassConnectionTop;
  ggSubjectConnectionPoint mClassConnectionBottom;
  ggSubjectConnectionPoint mClassConnectionLeft;
  ggSubjectConnectionPoint mClassConnectionRight;
  tSubjectConnectionPoints mMembersConnectionLeft;
  tSubjectConnectionPoints mMembersConnectionRight;

  ggClassyClass* mClass;
  ggClassyClassBox* mClassBox;

};

#endif // GGCLASSYBOX_H
