#include "ggItemBlockable.h"


ggItemBlockable::ggItemBlockable() :
  mIsBlocking(false)
{
}


ggItemBlockable::ggItemBlockable(const ggItemBlockable& aOther)
{
  CopyFrom(aOther);
}


ggItemBlockable& ggItemBlockable::operator = (const ggItemBlockable& aOther)
{
  CopyFrom(aOther);
  return *this;
}


ggItemBlockable::~ggItemBlockable()
{
}


bool ggItemBlockable::IsBlocking() const
{
  return mIsBlocking;
}


bool ggItemBlockable::IsBlocking(const ggItemBlockable* aItem) const
{
  return mIsBlockingOther.find(aItem) != mIsBlockingOther.end();
}


ggItemBlockable::Blocker::Blocker(ggItemBlockable* aItem) :
  mItem(aItem),
  mOtherItem(nullptr),
  mWasNotBlocking(false)
{
  if (!mItem->IsBlocking()) {
    mWasNotBlocking = true;
    mItem->BlockStartPrivate();
  }
}


ggItemBlockable::Blocker::Blocker(ggItemBlockable* aItem, const ggItemBlockable* aOtherItem) :
  mItem(aItem),
  mOtherItem(aOtherItem),
  mWasNotBlocking(false)
{
  if (!mItem->IsBlocking(mOtherItem)) {
    mWasNotBlocking = true;
    mItem->BlockStartPrivate(mOtherItem);
  }
}


ggItemBlockable::Blocker::~Blocker()
{
  if (mWasNotBlocking) {
    if (mOtherItem != nullptr) {
      if (mItem->IsBlocking(mOtherItem)) mItem->BlockFinishPrivate(mOtherItem);
    }
    else {
      if (mItem->IsBlocking()) mItem->BlockFinishPrivate();
    }
  }
}


void ggItemBlockable::CopyFrom(const ggItemBlockable&)
{
  // don't do anything: only "ggItemBlockable::Blocker" is allowed to change the blocking state
}


void ggItemBlockable::BlockStartPrivate()
{
  mIsBlocking = true;
  BlockingStart();
}


void ggItemBlockable::BlockFinishPrivate()
{
  mIsBlocking = false;
  BlockingFinish();
}


void ggItemBlockable::BlockStartPrivate(const ggItemBlockable* aOtherItem)
{
  mIsBlockingOther.insert(aOtherItem);
}


void ggItemBlockable::BlockFinishPrivate(const ggItemBlockable* aOtherItem)
{
  mIsBlockingOther.erase(aOtherItem);
}
