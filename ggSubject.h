#ifndef GGSUBJECT_H
#define GGSUBJECT_H

#include "ggItemLinked.h"
#include "ggItemBlockable.h"
#include "ggItemLazy.h"

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
 */
class ggSubject :
  public ggItemLinked,
  public ggItemBlockable,
  public ggItemLazy {

public:

  ggSubject();
  ggSubject(const ggSubject& aOther);
  ggSubject& operator = (const ggSubject& aOther);
  virtual ~ggSubject();

  void Attach(const ggSubject* aSubject);
  void Detach(const ggSubject* aSubject);

  void Notify();
  void Notify(const ggObserver* aObserverExcluded);

protected:

  virtual void WakingUp() override;

private:

  // make inherited methods private (prevent incest!)
  void Attach(const ggItemLinked* aItem);
  void Detach(const ggItemLinked* aItem);

  // the helpers ...
  void NotifyPrivate(const ggObserver* aObserverExcluded);
  void CopyFrom(const ggSubject aOther);

  bool mNotifyPending;

};

#endif // GGSUBJECT_H
