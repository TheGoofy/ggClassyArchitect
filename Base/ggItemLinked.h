#ifndef GGITEMLINKED_H
#define GGITEMLINKED_H

// 1) include system
#include <set>

// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * Allows to link (attach, detach) objects with other objects. It's a
 * two-way link - like an undirected graph. If an item is deleted, it
 * is properly detached from all attached items.
 */
class ggItemLinked {

public:

  ggItemLinked();
  ggItemLinked(const ggItemLinked& aOther);
  ggItemLinked& operator = (const ggItemLinked& aOther);

  virtual ~ggItemLinked();

  void Attach(const ggItemLinked* aItem);
  void Detach(const ggItemLinked* aItem);

protected:

  typedef std::set<const ggItemLinked*> tItems;

  tItems& Items();
  const tItems& Items() const;
  bool IsAttached(const ggItemLinked* aItem) const;

private:

  void Attach(const tItems& aItems);
  void DetachAll();

  tItems mItems;

};

#endif // GGITEMLINKED_H
