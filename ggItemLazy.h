#ifndef GGITEMLAZY_H
#define GGITEMLAZY_H

/**
 * An item, which can be put to sleep. The same guardian object that puts the
 * item to sleep should kindly wake it up again (other guardians must not wake
 * up the sleeping item). And an item which is already awake should not be
 * roused again. Use "ggItemLazy::Sleep" for a regardful treatment! Override
 * "FallingAsleep()" or "WakingUp()" in order to get notified, when the item
 * sleeping state changes.
 */
class ggItemLazy {

public:

  ggItemLazy();
  ggItemLazy(const ggItemLazy& aOther);
  ggItemLazy& operator = (const ggItemLazy& aOther);
  virtual ~ggItemLazy();

  /**
   * Puts the item to sleep as long as such a "sleep-object" exists.
   */
  class Sleep {
  public:
    Sleep(ggItemLazy* aItem);
    virtual ~Sleep();
  private:
    // hide default construction and copy
    Sleep();
    Sleep(const Sleep&);
    Sleep& operator = (const Sleep&);
    ggItemLazy* mItem;
    bool mWasAwake;
  };

  bool IsSleeping() const;

protected:

  // override this method, in order to get notified, when the item falls asleep
  virtual void FallingAsleep() {}

  // override this method, in order to get notified, when the item is waking up
  virtual void WakingUp() {}

private:

  void CopyFrom(const ggItemLazy&);
  void SleepPrivate();
  void WakeUpPrivate();

  bool mIsSleeping;

};

#endif // GGITEMLAZY_H
