#ifndef GGSUBJECT_H
#define GGSUBJECT_H

// 1) include system

// 2) include own project-related (sort by component dependency)
#include "Base/ggItemLinked.h"
#include "Base/ggBehaviorBlocking.h"
#include "Base/ggBehaviorLazy.h"

// 3) forward declarations
class ggObserver;

/**
 * A subject is representing some data of interest. If the data is changed,
 * the subject can be used to notify all attached observers. A subject may be
 * sleeping. While it's sleeping, it doesn't notify the attached observers.
 * However it sends the notification, when it wakes up. If there is an outer
 * loop that sends lots of notifications about the same subject with each
 * iteration, it's potentially causing a performance issue (in particular, if
 * there are also many observers). The subject can be put to sleep before the
 * loop starts (and wakeup, when the loop ends).
 * A blocked subject ignores all notifications.
 */
class ggSubject :
  public ggItemLinked,
  public ggBehaviorBlocking,
  public ggBehaviorLazy {

public:

  typedef ggBehaviorBlocking::cExecutor cExecutorBlocking;
  typedef ggBehaviorLazy::cExecutor cExecutorLazy;

  ggSubject();
  ggSubject(const ggSubject& aOther);
  ggSubject& operator = (const ggSubject& aOther);
  virtual ~ggSubject();

  void Attach(const ggSubject* aSubject);
  void Detach(const ggSubject* aSubject);

  void Notify();
  void Notify(const ggObserver* aObserverExcluded);

  bool IsBlocking(const ggBehaviorBlocking* aBehavior = nullptr) const;
  bool IsLazy(const ggBehaviorLazy* aBehavior = nullptr) const;

protected:

  virtual void ExecutingStop(const ggBehaviorBlocking* aBehavior) override;
  virtual void ExecutingStop(const ggBehaviorLazy* aBehavior) override;

private:

  // make inherited methods private (prevent incest!)
  void Attach(const ggItemLinked* aItem);
  void Detach(const ggItemLinked* aItem);

  // the helpers ...
  void NotifyPrivate(const ggObserver* aObserverExcluded);
  void CopyFrom(const ggSubject& aOther);

  bool mNotifyPending;

};

#endif // GGSUBJECT_H
