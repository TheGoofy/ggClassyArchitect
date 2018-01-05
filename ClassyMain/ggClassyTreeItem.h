#ifndef GGCLASSYTREEITEM_H
#define GGCLASSYTREEITEM_H

// 1) include system (or QT)
#include <QString>

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "Base/ggVectorSetT.h"

// 3) forward declarations
class ggClassyItem;
class ggClassyDataSet;
class ggClassyCollection;
class ggClassyClass;
class ggClassyClassMember;
class ggClassyFrame;

/**
 * @brief The ggClassyTreeItem class represents a hierarchical structure of the ggClassyDataSet.
 * Items are variants of dataset, collection, class, or member. The dataset is the root node, other
 * items can be added as child nodes. Children are sorted by their name (and not by the sequence
 * how thei are added).
 */
class ggClassyTreeItem {
public:

  ggClassyTreeItem();
  ggClassyTreeItem(const ggClassyTreeItem& aOther);
  ggClassyTreeItem(const ggClassyItem* aItem);
  virtual ~ggClassyTreeItem();

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  ggClassyTreeItem* AddChild(const ggClassyItem* aItem);
  ggUSize GetNumberOfChildren() const;
  ggClassyTreeItem* GetChild(ggUSize aIndex) const;
  ggClassyTreeItem* RemoveChild(ggUSize aIndex);
  ggClassyTreeItem* GetParent() const;
  ggUSize GetSiblingIndex() const;
  const QString& GetName() const;
  const QString& GetDescription() const;
  void DeleteChild(ggUSize aIndex);
  void DeleteChildren();

  const ggClassyDataSet* GetDataSet() const;
  const ggClassyCollection* GetCollection() const;
  const ggClassyClass* GetClass() const;
  const ggClassyClassMember* GetMember() const;
  const ggClassyFrame* GetFrame() const;

private:

  struct cLess {
    bool operator () (const ggClassyTreeItem* aItemA, const ggClassyTreeItem* aItemB) const;
  };

  typedef ggVectorSetT<ggClassyTreeItem*, cLess> tChildren;

  const ggClassyItem* mItem;
  ggClassyTreeItem* mParent;
  tChildren mChildren;

};

#endif // GGCLASSYTREEITEM_H
