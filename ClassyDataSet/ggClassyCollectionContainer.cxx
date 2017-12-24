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
    const ggClassyCollection* vCollection = FindCollection(aCollection->mName);
    // another collection with the same name does not yet exist
    if (vCollection == nullptr) {
      insert(aCollection);
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
  erase(vCollection);
  return vCollection;
}


ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName)
{
  ggClassyCollection vCollection(aName);
  iterator vCollectionsIterator = find(&vCollection);
  if (vCollectionsIterator != end()) return *vCollectionsIterator;
  else return nullptr;
}


const ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName) const
{
  ggClassyCollection vCollection(aName);
  iterator vCollectionsIterator = find(&vCollection);
  if (vCollectionsIterator != end()) return *vCollectionsIterator;
  else return nullptr;
}


ggUSize ggClassyCollectionContainer::GetSize() const
{
  return size();
}


const ggClassyCollection* ggClassyCollectionContainer::SearchCollection(ggUSize aIndex) const
{
  // goofy: this might be slow
  if (aIndex >= size()) return nullptr;
  ggClassyCollectionContainer::const_iterator vCollectionsIterator = begin();
  for (ggUSize vIndex = 0; vIndex < aIndex; vIndex++) vCollectionsIterator++;
  return *vCollectionsIterator;
}
