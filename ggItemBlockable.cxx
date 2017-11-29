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


ggItemBlockable::Blocker::Blocker(ggItemBlockable* aItem) :
  mItem(aItem),
  mWasNotBlocking(false)
{
  if (!mItem->IsBlocking()) {
    mWasNotBlocking = true;
    mItem->BlockStartPrivate();
  }
}


ggItemBlockable::Blocker::~Blocker()
{
  if (mWasNotBlocking) {
    if (mItem->IsBlocking()) mItem->BlockFinishPrivate();
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
