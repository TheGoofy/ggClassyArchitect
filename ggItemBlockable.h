#ifndef GGITEMBLOCKABLE_H
#define GGITEMBLOCKABLE_H

#include <set>

/**
 * An item, which can be set to blocking
 */
class ggItemBlockable {

public:

  ggItemBlockable();
  ggItemBlockable(const ggItemBlockable& aOther);
  ggItemBlockable& operator = (const ggItemBlockable& aOther);

  virtual ~ggItemBlockable();

  /**
   * makes item blocking as long as the blocker exists
   */
  class Blocker {
  public:
    Blocker(ggItemBlockable* aItem);
    Blocker(ggItemBlockable* aItem, const ggItemBlockable* aOtherItem);
    virtual ~Blocker();
  private:
    // hide default construction and copy
    Blocker();
    Blocker(const Blocker&);
    Blocker& operator = (const Blocker&);
    ggItemBlockable* mItem;
    const ggItemBlockable* mOtherItem;
    bool mWasNotBlocking;
  };

  bool IsBlocking() const;
  bool IsBlocking(const ggItemBlockable* aItem) const;

protected:

  // override this method, in order to get notified, when the item starts blocking
  virtual void BlockingStart() {}

  // override this method, in order to get notified, when the item is no longer blocking
  virtual void BlockingFinish() {}

private:

  void CopyFrom(const ggItemBlockable&);

  void BlockStartPrivate();
  void BlockFinishPrivate();
  bool mIsBlocking;

  void BlockStartPrivate(const ggItemBlockable* aOtherItem);
  void BlockFinishPrivate(const ggItemBlockable* aOtherItem);
  std::set<const ggItemBlockable*> mIsBlockingOther;
};

#endif // GGITEMBLOCKABLE_H
