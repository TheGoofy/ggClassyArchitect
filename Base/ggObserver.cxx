#include "Base/ggObserver.h"
#include "Base/ggSubject.h"
#include "Base/ggWalkerT.h"


ggObserver::ggObserver()
{
}


ggObserver::ggObserver(const ggObserver& aOther)
{
  CopyFrom(aOther);
}


ggObserver& ggObserver::operator = (const ggObserver& aOther)
{
  CopyFrom(aOther);
  return *this;
}


ggObserver::~ggObserver()
{
  // when a sleeping observer dies: dous it make sense to get the pending updates?
}


void ggObserver::ExecutingStop(const ggBehaviorBlocking*)
{
  // nothing to do here. missed updates can be ignored since we were blocking
}


void ggObserver::ExecutingStop(const ggBehaviorLazy*)
{
  // we certainly missed to update some subjects. do it now
  UpdatePendingSubjects();
}


void ggObserver::Attach(const ggSubject* aSubject)
{
  ggItemLinked::Attach(aSubject);
}


void ggObserver::Detach(const ggSubject* aSubject)
{
  ggItemLinked::Detach(aSubject);
}


bool ggObserver::IsBlocking(const ggBehaviorBlocking* aBehavior) const
{
  return ggBehaviorBlocking::IsExecuting(aBehavior);
}


bool ggObserver::IsLazy(const ggBehaviorLazy* aBehavior) const
{
  return ggBehaviorLazy::IsExecuting(aBehavior);
}


void ggObserver::UpdatePrivate(const ggSubject* aSubject)
{
  if (!IsBlocking() && !IsBlocking(aSubject)) {
    if (!IsLazy()) Update(aSubject);
    else mUpdatePendingSubjects.insert(aSubject);
  }
}


void ggObserver::UpdatePendingSubjects()
{
  ggWalkerT<tSubjects::iterator> vSubjectsWalker(mUpdatePendingSubjects);
  while (vSubjectsWalker) Update(*vSubjectsWalker);
  mUpdatePendingSubjects.clear();
}


void ggObserver::CopyFrom(const ggObserver& aOther)
{
  // basically only pending subjects that are going to be detached need to be
  // updated, and removed from the pendence list, but we're updating simply all.
  UpdatePendingSubjects();
  ggItemLinked::operator = (aOther);
  ggBehaviorBlocking::operator = (aOther);
  ggBehaviorLazy::operator = (aOther);
}
