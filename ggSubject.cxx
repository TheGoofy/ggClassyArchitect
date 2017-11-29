#include "ggSubject.h"
#include "ggObserver.h"
#include "ggWalkerT.h"


ggSubject::ggSubject() :
  mNotifyPending(false)
{
}


ggSubject::ggSubject(const ggSubject& aOther)
{
  CopyFrom(aOther);
}


ggSubject& ggSubject::operator = (const ggSubject& aOther)
{
  CopyFrom(aOther);
  return *this;
}


ggSubject::~ggSubject()
{
  // when a sleeping subject dies, and a notification is pending:
  // should it notify the observer? ... probably not.
}


void ggSubject::Attach(const ggSubject* aSubject)
{
  ggItemLinked::Attach(aSubject);
}


void ggSubject::Detach(const ggSubject* aSubject)
{
  ggItemLinked::Detach(aSubject);
}


void ggSubject::Notify()
{
  Notify(nullptr);
}


void ggSubject::Notify(const ggObserver* aObserverExcluded)
{
  if (!IsBlocking()) {
    mNotifyPending = true;
    if (!IsSleeping()) NotifyPrivate(aObserverExcluded);
  }
}


void ggSubject::WakingUp()
{
  NotifyPrivate(nullptr);
}


void ggSubject::NotifyPrivate(const ggObserver* aObserverExcluded)
{
  if (mNotifyPending) {
    mNotifyPending = false;
    ggWalkerT<tItems::iterator> vItemsWalker(Items());
    while (vItemsWalker) {
      const ggObserver* vObserver = dynamic_cast<const ggObserver*>(*vItemsWalker);
      if ((vObserver != nullptr) &&
          (vObserver != aObserverExcluded)) {
        const_cast<ggObserver*>(vObserver)->UpdatePrivate(this);
      }
    }
  }
}


void ggSubject::CopyFrom(const ggSubject aOther)
{
  // there will be observers disconnected, when a new subject with different
  // observers is assigned. if there is a notification pending (because of sleep),
  // these observers have to be notified.
  // basically only observers that are going to be detached would need to be
  // notified (if there is a notification pending). for simplicity reasons
  // we're going to notify all observers.
  NotifyPrivate(nullptr);
  ggItemLinked::operator = (aOther);
  ggItemBlockable::operator = (aOther);
  ggItemLazy::operator = (aOther);
}
