#ifndef GGITEMBLOCKABLE_H
#define GGITEMBLOCKABLE_H


/**
 * An item, which can be set to blocking
 */
class ggItemBlockable {

public:

  ggItemBlockable();
  ggItemBlockable(const ggItemBlockable& aOther);
  ggItemBlockable& operator = (const ggItemBlockable& aOther);

  virtual ~ggItemBlockable();

  bool IsBlocking() const;

  class Blocker {
  public:
    Blocker(ggItemBlockable* aItem);
    virtual ~Blocker();
  private:
    // hide default construction and copy
    Blocker();
    Blocker(const Blocker&);
    Blocker& operator = (const Blocker&);
    ggItemBlockable* mItem;
    bool mWasNotBlocking;
  };

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

};

#endif // GGITEMBLOCKABLE_H
