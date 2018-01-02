// 0) include own header
#include "ggClassyCollectionContainer.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


ggClassyCollectionContainer::ggClassyCollectionContainer()
{
}


ggClassyCollectionContainer::ggClassyCollectionContainer(const ggClassyCollectionContainer& aOther)
{
  *this = aOther;
}


const QString& ggClassyCollectionContainer::TypeID()
{
  static const QString vTypeID("ggClassyCollectionContainer");
  return vTypeID;
}


const QString& ggClassyCollectionContainer::VTypeID() const
{
  return TypeID();
}


ggClassyCollectionContainer& ggClassyCollectionContainer::operator = (const ggClassyCollectionContainer& aOther)
{
  // one collective notification (and not each individual class)
  ggSubject::cExecutorLazy vLazy(this);

  // delete all classes
  DeleteAllCollections();

  // add (and notify) copies
  ggWalkerT<tCollections::const_iterator> vOtherCollectionsWalker(aOther.mCollections);
  while (vOtherCollectionsWalker) {
    AddCollection(new ggClassyCollection(**vOtherCollectionsWalker));
  }

  return *this;
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
  if (vCollection != nullptr) {
    mCollections.erase(vCollection);
    Notify();
  }
  return vCollection;
}


ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName)
{
  static ggClassyCollection vCollection(""); vCollection.SetName(aName);
  tCollections::iterator vCollectionsIterator = mCollections.find(&vCollection);
  if (vCollectionsIterator != mCollections.end()) return *vCollectionsIterator;
  else return nullptr;
}


const ggClassyCollection* ggClassyCollectionContainer::FindCollection(const QString& aName) const
{
  static ggClassyCollection vCollection(""); vCollection.SetName(aName);
  tCollections::iterator vCollectionsIterator = mCollections.find(&vCollection);
  if (vCollectionsIterator != mCollections.end()) return *vCollectionsIterator;
  else return nullptr;
}


void ggClassyCollectionContainer::DeleteCollection(const QString& aName)
{
  ggClassyCollection* vCollection = RemoveCollection(aName);
  delete vCollection;
}


void ggClassyCollectionContainer::DeleteAllCollections()
{
  ggWalkerT<tCollections::iterator> vCollectionsWalker(mCollections);
  while (vCollectionsWalker) {
    ggClassyCollection* vCollection = *vCollectionsWalker;
    delete vCollection;
  }
  mCollections.clear();
  Notify();
}


ggUSize ggClassyCollectionContainer::GetSize() const
{
  return mCollections.size();
}


ggClassyCollection* ggClassyCollectionContainer::GetCollection(ggUSize aIndex)
{
  return mCollections[aIndex];
}


const ggClassyCollection* ggClassyCollectionContainer::GetCollection(ggUSize aIndex) const
{
  return mCollections[aIndex];
}


bool ggClassyCollectionContainer::cLess::operator() (const ggClassyCollection* aCollectionA,
                                                     const ggClassyCollection* aCollectionB) const
{
  if (aCollectionA != nullptr && aCollectionB != nullptr) {
    return aCollectionA->GetName() < aCollectionB->GetName();
  }
  else {
    return aCollectionA < aCollectionB;
  }
}
