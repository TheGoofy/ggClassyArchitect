// 0) include own header
#include "ggClassyCollectionContainer.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


const QString& ggClassyCollectionContainer::TypeID()
{
  static const QString vTypeID("ggClassyCollectionContainer");
  return vTypeID;
}


const QString& ggClassyCollectionContainer::VTypeID() const
{
  return TypeID();
}


ggClassyCollection* ggClassyCollectionContainer::AddCollection(ggClassyCollection* aCollection)
{
  // don't add nothing
  if (aCollection != nullptr) {
    // check if name is unique
    const ggClassyCollection* vCollection = FindCollection(aCollection->GetName());
    // another collection with the same name does not yet exist
    if (vCollection == nullptr) {
      mCollections.insert(aCollection);
      Notify();
      return aCollection;
    }
    // the actual collection is already there
    if (vCollection == aCollection) {
      return aCollection;
    }
  }
  return nullptr;
}


ggClassyCollection* ggClassyCollectionContainer::RemoveCollection(const QString& aName)
{
  ggClassyCollection* vCollection = FindCollection(aName);
  mCollections.erase(vCollection);
  return vCollection;
}


ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName)
{
  ggClassyCollection vCollection(aName);
  tCollections::iterator vCollectionsIterator = mCollections.find(&vCollection);
  if (vCollectionsIterator != mCollections.end()) return *vCollectionsIterator;
  else return nullptr;
}


const ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName) const
{
  ggClassyCollection vCollection(aName);
  tCollections::iterator vCollectionsIterator = mCollections.find(&vCollection);
  if (vCollectionsIterator != mCollections.end()) return *vCollectionsIterator;
  else return nullptr;
}


void ggClassyCollectionContainer::DeleteAllCollections()
{
  ggWalkerT<tCollections::iterator> vCollectionsWalker(mCollections);
  while (vCollectionsWalker) {
    delete *vCollectionsWalker;
  }
  mCollections.clear();
  Notify();
}


ggUSize ggClassyCollectionContainer::GetSize() const
{
  return mCollections.size();
}


const ggClassyCollection* ggClassyCollectionContainer::GetCollection(ggUSize aIndex) const
{
  return mCollections[aIndex];
}
