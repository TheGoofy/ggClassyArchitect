#include "ggObserver.h"
#include "ggSubject.h"
#include "ggWalkerT.h"


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


void ggObserver::WakingUp()
{
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

/*
void Attach(const ggItemLinked* aItem);
void Detach(const ggItemLinked* aItem);
*/

void ggObserver::UpdatePrivate(const ggSubject* aSubject)
{
  if (!IsBlocking()) {
    if (!IsSleeping()) Update(aSubject);
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
  ggItemBlockable::operator = (aOther);
  ggItemLazy::operator = (aOther);
}
