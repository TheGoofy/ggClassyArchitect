#ifndef GGCLASSYAUTOCONNECTPATHITEM_H
#define GGCLASSYAUTOCONNECTPATHITEM_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "BaseGraphics/ggGraphicsAutoConnectPathItem.h"

// 3) forward declarations
class ggClassyCollection;

class ggClassyAutoConnectPathItem :
  public ggGraphicsAutoConnectPathItem
{
public:

  ggClassyAutoConnectPathItem(QGraphicsItem* aParent = nullptr);
  virtual ~ggClassyAutoConnectPathItem();

  void SetCollection(const ggClassyCollection* aCollection);

  void SetClassInfo(const QString& aClassNameSrc, const QString& aClassNameDst, int aMemberIndex = -1);
  const QString& GetClassNameSrc() const;
  const QString& GetClassNameDst() const;
  int GetMemberIndex() const;

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  void UpdateCollectionRead();
  void UpdateSettings();

  const ggClassyCollection* mCollection;

  QString mClassNameSrc;
  QString mClassNameDst;
  int mMemberIndex;

};

#endif // GGCLASSYAUTOCONNECTPATHITEM_H
