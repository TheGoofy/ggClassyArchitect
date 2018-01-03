#ifndef GGCLASSYTREEITEM_H
#define GGCLASSYTREEITEM_H

// 1) include system (or QT)
#include <QString>

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "Base/ggVectorSetT.h"

// 3) forward declarations
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

  ggClassyTreeItem(const ggClassyDataSet* aDataSet);
  virtual ~ggClassyTreeItem();

  template <typename TClassyType>
  ggClassyTreeItem* AddChild(const TClassyType* aClassyChildItem);
  ggUSize GetNumberOfChildren() const;
  ggClassyTreeItem* GetChild(ggUSize aIndex) const;
  ggClassyTreeItem* RemoveChild(ggUSize aIndex);
  ggClassyTreeItem* GetParent() const;
  ggUSize GetSiblingIndex() const;
  const QString& GetName() const;
  void DeleteChild(ggUSize aIndex);
  void DeleteChildren();

  const ggClassyDataSet* GetDataSet() const;
  const ggClassyCollection* GetCollection() const;
  const ggClassyClass* GetClass() const;
  const ggClassyClassMember* GetMember() const;
  const ggClassyFrame* GetFrame() const;

private:

  ggClassyTreeItem(const ggClassyCollection* aCollection);
  ggClassyTreeItem(const ggClassyClass* aClass);
  ggClassyTreeItem(const ggClassyClassMember* aMember);
  ggClassyTreeItem(const ggClassyFrame* aFrame);

  enum class cType {
    eUnknown,
    eDataSet,
    eCollection,
    eClass,
    eMember,
    eFrame,
  };

  struct cLess {
    bool operator () (const ggClassyTreeItem* aItemA, const ggClassyTreeItem* aItemB) const;
  };

  typedef ggVectorSetT<ggClassyTreeItem*, cLess> tChildren;

  const cType mType;
  const void* mData;
  ggClassyTreeItem* mParent;
  tChildren mChildren;

};

#endif // GGCLASSYTREEITEM_H
