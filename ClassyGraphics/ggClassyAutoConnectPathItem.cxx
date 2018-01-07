// 0) include own header
#include "ggClassyAutoConnectPathItem.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassySettings.h"
#include "ClassyDataSet/ggClassyCollection.h"


ggClassyAutoConnectPathItem::ggClassyAutoConnectPathItem(QGraphicsItem* aParent) :
  ggGraphicsAutoConnectPathItem(aParent),
  mCollection(nullptr),
  mClassNameSrc(""),
  mClassNameDst(""),
  mMemberIndex(-1)
{
  ggObserver::Attach(ggClassySettings::GetInstance());
  SetCollection(ggClassyCollection::GetDefaultCollection());
  UpdateSettings();
}


ggClassyAutoConnectPathItem::~ggClassyAutoConnectPathItem()
{
}


void ggClassyAutoConnectPathItem::SetCollection(const ggClassyCollection* aCollection)
{
  if (mCollection != aCollection &&
      aCollection != nullptr) {
    ggObserver::Detach(mCollection);
    mCollection = aCollection;
    ggObserver::Attach(mCollection);
    UpdateCollectionRead();
  }
}


void ggClassyAutoConnectPathItem::SetClassInfo(const QString& aClassNameSrc,
                                               const QString& aClassNameDst,
                                               int aMemberIndex)
{
  mClassNameSrc = aClassNameSrc;
  mClassNameDst = aClassNameDst;
  mMemberIndex = aMemberIndex;
}


const QString& ggClassyAutoConnectPathItem::GetClassNameSrc() const
{
  return mClassNameSrc;
}


const QString& ggClassyAutoConnectPathItem::GetClassNameDst() const
{
  return mClassNameDst;
}


int ggClassyAutoConnectPathItem::GetMemberIndex() const
{
  return mMemberIndex;
}


void ggClassyAutoConnectPathItem::Update(const ggSubject* aSubject)
{
  if (aSubject == mCollection) {
    UpdateCollectionRead();
  }

  else if (aSubject == ggClassySettings::GetInstance()) {
    UpdateSettings();
  }

  ggGraphicsAutoConnectPathItem::Update(aSubject);
}


void ggClassyAutoConnectPathItem::UpdateCollectionRead()
{
  if (mCollection != nullptr) {
    setPen(mCollection->mConnectionLines);
  }
}


void ggClassyAutoConnectPathItem::UpdateSettings()
{
  SetColorSelected(ggClassySettings::GetInstance()->GetSelectionColor());
}
