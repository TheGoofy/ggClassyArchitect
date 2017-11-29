#include "ggItemLazy.h"


ggItemLazy::ggItemLazy() :
  mIsSleeping(false)
{
}


ggItemLazy::ggItemLazy(const ggItemLazy& aOther)
{
  CopyFrom(aOther);
}


ggItemLazy& ggItemLazy::operator = (const ggItemLazy& aOther)
{
  CopyFrom(aOther);
  return *this;
}


ggItemLazy::~ggItemLazy()
{
  if (IsSleeping()) WakeUpPrivate();
}


ggItemLazy::Sleep::Sleep(ggItemLazy* aItem) :
  mItem(aItem),
  mWasAwake(false)
{
  if (!mItem->IsSleeping()) {
    mWasAwake = true;
    mItem->SleepPrivate();
  }
}


ggItemLazy::Sleep::~Sleep()
{
  if (mWasAwake) {
    // maybe somebody else did already wake it up
    if (mItem->IsSleeping()) mItem->WakeUpPrivate();
  }
}


bool ggItemLazy::IsSleeping() const
{
  return mIsSleeping;
}


void ggItemLazy::CopyFrom(const ggItemLazy&)
{
  // don't do anything: only "ggItemLazy::Sleep" is allowed to change the sleep
}


void ggItemLazy::SleepPrivate()
{
  mIsSleeping = true;
  FallingAsleep();
}


void ggItemLazy::WakeUpPrivate()
{
  mIsSleeping = false;
  WakingUp();
}
