#ifndef GGOBSERVER_H
#define GGOBSERVER_H

#include "ggItemLinked.h"
#include "ggItemBlockable.h"
#include "ggItemLazy.h"

class ggSubject;

/**
 * An observer can "listen" to various linked subjects. If a subject posts a
 * "Notify" all attached observers receive an "Update". An observer may be put
 * to sleep. While the observer is sleeping, it is not beeing updated. The
 * observer receives all updates at wake-up, when it missed some updates while
 * it was sleeping (multiple updates of the same subject are only notified once).
 * A sleeping observer is putentially useful for performance reasons (e.g. if a
 * viewer window is not visible, it maybe does not need to receive updates).
 */
class ggObserver :
  public ggItemLinked,
  public ggItemBlockable,
  public ggItemLazy {

public:

  ggObserver();
  ggObserver(const ggObserver& aOther);
  ggObserver& operator = (const ggObserver& aOther);
  virtual ~ggObserver();

  void Attach(const ggSubject* aSubject);
  void Detach(const ggSubject* aSubject);

  // an ovserver has to override this method, otherwise it makes no sense to be observer
  virtual void Update(const ggSubject* aSubject) = 0;

protected:

  virtual void WakingUp() override;

private:

  // make inherited methods private (prevent incest!)
  void Attach(const ggItemLinked* aItem);
  void Detach(const ggItemLinked* aItem);

  // the subject requires some special privileges (needs "UpdatePrivate");
  friend class ggSubject;

  // the helpers
  void UpdatePrivate(const ggSubject* aSubject);
  void UpdatePendingSubjects();
  void CopyFrom(const ggObserver& aOther);

  typedef std::set<const ggSubject*> tSubjects;
  tSubjects mUpdatePendingSubjects;

};

#endif // GGOBSERVER_H
