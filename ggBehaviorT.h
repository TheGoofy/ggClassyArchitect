#ifndef GGBEHAVIORT_H
#define GGBEHAVIORT_H

template <class TBehavior>
class ggBehaviorT {
public:

  ggBehaviorT() {
  }

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
