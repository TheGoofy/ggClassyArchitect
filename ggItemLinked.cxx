#include "ggItemLinked.h"
#include "ggWalkerT.h"


ggItemLinked::ggItemLinked()
{
}


ggItemLinked::ggItemLinked(const ggItemLinked& aOther)
{
  DetachAll();
  Attach(aOther.mItems);
}


ggItemLinked& ggItemLinked::operator = (const ggItemLinked& aOther)
{
  DetachAll();
  Attach(aOther.mItems);
  return *this;
}


ggItemLinked::~ggItemLinked()
{
  DetachAll();
}


void ggItemLinked::Attach(const ggItemLinked* aItem)
{
  if (aItem != nullptr) {
    mItems.insert(aItem);
    const_cast<ggItemLinked*>(aItem)->mItems.insert(this);
  }
}


void ggItemLinked::Detach(const ggItemLinked* aItem)
{
  if (aItem != nullptr) {
    mItems.erase(aItem);
    const_cast<ggItemLinked*>(aItem)->mItems.erase(this);
  }
}


ggItemLinked::tItems& ggItemLinked::Items()
{
  return mItems;
}


const ggItemLinked::tItems& ggItemLinked::Items() const
{
  return mItems;
}


bool ggItemLinked::IsAttached(const ggItemLinked* aItem) const
{
  return mItems.find(aItem) != mItems.end();
}


void ggItemLinked::Attach(const ggItemLinked::tItems& aItems)
{
  ggWalkerT<tItems::const_iterator> vItemsWalker(aItems);
  while (vItemsWalker) Attach(*vItemsWalker);
}


void ggItemLinked::DetachAll()
{
  tItems vItems(mItems);
  mItems.clear();
  ggWalkerT<tItems::iterator> vItemsWalker(vItems);
  while (vItemsWalker) const_cast<ggItemLinked*>(*vItemsWalker)->Detach(this);
}
