// 0) include own header
#include "ggClassyAutoConnectPathItem.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyCollection.h"


ggClassyAutoConnectPathItem::ggClassyAutoConnectPathItem(QGraphicsItem* aParent) :
  ggGraphicsAutoConnectPathItem(aParent),
  mCollection(nullptr)
{
}


ggClassyAutoConnectPathItem::~ggClassyAutoConnectPathItem()
{
}


void ggClassyAutoConnectPathItem::SetCollection(const ggClassyCollection* aCollection)
{
  if (mCollection != aCollection) {
    ggObserver::Detach(mCollection);
    mCollection = aCollection;
    ggObserver::Attach(mCollection);
    UpdateCollectionRead();
  }
}


void ggClassyAutoConnectPathItem::Update(const ggSubject* aSubject)
{
  if (aSubject == mCollection) {
    UpdateCollectionRead();
  }
  ggGraphicsAutoConnectPathItem::Update(aSubject);
}


void ggClassyAutoConnectPathItem::UpdateCollectionRead()
{
  if (mCollection != nullptr) {
    setPen(mCollection->mConnectionLines);
  }
}
