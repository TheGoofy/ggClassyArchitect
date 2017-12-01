#ifndef GGBEHAVIORT_H
#define GGBEHAVIORT_H

/**
 * A class, which can behave (or not behave) in a certain way.
 *
 * The type of behavior is defined by the template argument. E.g. "TBehavior" could
 * be "lazy", or "sick". A behavior can not be executed again, if it is already
 * executed. E.g. if the behavior "sick" is already executed, that particulat gehavior
 * can't be executed again (because it already doe it).
 *
 * The behavuor can only be executed temporarily. When it is started, it will also
 * be stopped. Nested calls are ignored. The Executor which has started the execution
 * of the behavior will also stop it.
 *
 * Override "ExecutingStart" or "ExecutingStop" in order to get notified, when the
 * behavior state is changed.
 */
template <class TBehavior>
class ggBehaviorT {
public:

  /**
   * Sets the execution flag as long as the object exists. The behavior is started
   * at construction, and it's deleted when the executor is destroyed.
   */
  class Executor {
  public:

    Executor(ggBehaviorT* aBehavior) :
      mBehavior(aBehavior),
      mBehaviorOther(nullptr),
      mWasExecuting(true) {
      if (!mBehavior->IsExecuting()) {
        mWasExecuting = false;
        mBehavior->ExecuteStart();
      }
    }

    Executor(ggBehaviorT* aBehavior, const ggBehaviorT* aBehaviorOther) :
      mBehavior(aBehavior),
      mBehaviorOther(aBehaviorOther),
      mWasExecuting(true) {
      if (!mBehavior->IsExecuting(mBehaviorOther)) {
        mWasExecuting = false;
        mBehavior->ExecuteStart(mBehaviorOther);
      }
    }

    virtual ~Executor() {
      if (!mWasExecuting) {
        if (mBehaviorOther == nullptr) mBehavior->ExecuteStop();
        else mBehavior->ExecuteStop(mBehaviorOther);
      }
    }

  private:

    ggBehaviorT* mBehavior;
    const ggBehaviorT* mBehaviorOther;
    bool mWasExecuting;

  };

  bool IsExecuting() const {
    return IsExecuting(this);
  }

  bool IsExecuting(const ggBehaviorT* aBehavior) const {
    if (aBehavior == nullptr) return IsExecuting(this);
    return mIsExecuting.find(aBehavior) != mIsExecuting.end();
  }

protected:

  // override this method, in order to get notified, when the object starts a behavior
  virtual void ExecutingStart(const ggBehaviorT*) {}

  // override this method, in order to get notified, when the object stops a behavior
  virtual void ExecutingStop(const ggBehaviorT*) {}

private:

  void ExecuteStart() {
    ExecuteStart(this);
  }

  void ExecuteStop() {
    ExecuteStop(this);
  }

  void ExecuteStart(const ggBehaviorT* aBehavior) {
    if (!IsExecuting(aBehavior)) {
      mIsExecuting.insert(aBehavior);
      ExecutingStart(aBehavior);
    }
  }

  void ExecuteStop(const ggBehaviorT* aBehavior) {
    if (IsExecuting(aBehavior)) {
      mIsExecuting.erase(aBehavior);
      ExecutingStop(aBehavior);
    }
  }

  std::set<const ggBehaviorT*> mIsExecuting;

};


#endif // GGBEHAVIORT_H
