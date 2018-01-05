// 0) include own header
#include "ggClassyTreeItem.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyTreeItem::ggClassyTreeItem() :
  mItem(nullptr),
  mParent(nullptr)
{
}


ggClassyTreeItem::ggClassyTreeItem(const ggClassyTreeItem& aOther) :
  mItem(aOther.mItem),
  mParent(aOther.mParent),
  mChildren(aOther.mChildren)
{
}


ggClassyTreeItem::ggClassyTreeItem(const ggClassyItem* aItem) :
  mItem(aItem),
  mParent(nullptr)
{
}


ggClassyTreeItem::~ggClassyTreeItem()
{
  DeleteChildren();
}


const QString& ggClassyTreeItem::TypeID()
{
  static const QString vTypeID("ggClassyTreeItem");
  return vTypeID;
}


const QString& ggClassyTreeItem::VTypeID() const
{
  return TypeID();
}


ggClassyTreeItem* ggClassyTreeItem::AddChild(const ggClassyItem* aItem)
{
  ggClassyTreeItem* vChildItem = new ggClassyTreeItem(aItem);
  vChildItem->mParent = this;
  mChildren.insert(vChildItem);
  return vChildItem;
}


ggUSize ggClassyTreeItem::GetNumberOfChildren() const
{
  return mChildren.size();
}


ggClassyTreeItem* ggClassyTreeItem::GetChild(ggUSize aIndex) const
{
  if (aIndex < mChildren.size()) return mChildren[aIndex];
  else return nullptr;
}


ggClassyTreeItem* ggClassyTreeItem::RemoveChild(ggUSize aIndex)
{
  if (aIndex < mChildren.size()) {
    ggClassyTreeItem* vChildItem = mChildren[aIndex];
    mChildren.erase(vChildItem);
    vChildItem->mParent = nullptr;
    return vChildItem;
  }
  return nullptr;
}


void ggClassyTreeItem::DeleteChild(ggUSize aIndex)
{
  if (aIndex < mChildren.size()) {
    ggClassyTreeItem* vChildItem = mChildren[aIndex];
    mChildren.erase(vChildItem);
    delete vChildItem;
  }
}


void ggClassyTreeItem::DeleteChildren()
{
  ggWalkerT<tChildren::iterator> vChildrenWalker(mChildren);
  while (vChildrenWalker) {
    delete *vChildrenWalker;
  }
  mChildren.clear();
}


ggClassyTreeItem* ggClassyTreeItem::GetParent() const
{
  return mParent;
}


ggUSize ggClassyTreeItem::GetSiblingIndex() const
{
  if (mParent != nullptr) {
    return mParent->mChildren.indexOf(const_cast<ggClassyTreeItem*>(this));
  }
  return 0;
}


const QString& ggClassyTreeItem::GetName() const
{
  if (GetDataSet() != nullptr) return GetDataSet()->TypeID();
  if (GetCollection() != nullptr) return GetCollection()->GetName();
  if (GetClass() != nullptr) return GetClass()->GetName();
  if (GetMember() != nullptr) return GetMember()->GetName();
  if (GetFrame() != nullptr) return GetFrame()->TypeID();
  if (mItem != nullptr) return mItem->VTypeID();
  static const QString vName("ggClassyTreeItem::GetName() - nullptr");
  return vName;
}


const QString& ggClassyTreeItem::GetDescription() const
{
  if (GetDataSet() != nullptr) return GetDataSet()->TypeID();
  if (GetCollection() != nullptr) return GetCollection()->TypeID();
  if (GetClass() != nullptr) return GetClass()->GetDescription();
  if (GetMember() != nullptr) return GetMember()->GetClassName();
  if (GetFrame() != nullptr) return GetFrame()->GetDescription();
  if (mItem != nullptr) return mItem->VTypeID();
  static const QString vDescription("ggClassyTreeItem::GetDescription() - nullptr");
  return vDescription;
}


const ggClassyDataSet* ggClassyTreeItem::GetDataSet() const
{
  return dynamic_cast<const ggClassyDataSet*>(mItem);
}


const ggClassyCollection* ggClassyTreeItem::GetCollection() const
{
  return dynamic_cast<const ggClassyCollection*>(mItem);
}


const ggClassyClass* ggClassyTreeItem::GetClass() const
{
  return dynamic_cast<const ggClassyClass*>(mItem);
}


const ggClassyClassMember* ggClassyTreeItem::GetMember() const
{
  return dynamic_cast<const ggClassyClassMember*>(mItem);
}


const ggClassyFrame* ggClassyTreeItem::GetFrame() const
{
  return dynamic_cast<const ggClassyFrame*>(mItem);
}


bool ggClassyTreeItem::cLess::operator () (const ggClassyTreeItem* aItemA,
                                           const ggClassyTreeItem* aItemB) const
{
  // first compare the types
  if (aItemA->VTypeID() != aItemB->VTypeID())
    return (aItemA->VTypeID() < aItemB->VTypeID());

  // the types are the same: compare the names
  if (aItemA->GetName() != aItemB->GetName())
    return (aItemA->GetName() < aItemB->GetName());

  // in case of the same name, let's compare the address (pointer)
  return aItemA->mItem < aItemB->mItem;
}
