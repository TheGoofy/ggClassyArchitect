// 0) include own header
#include "ggClassyTreeItem.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyTreeItem::ggClassyTreeItem(const ggClassyDataSet* aDataSet) :
  mType(cType::eDataSet),
  mData(aDataSet),
  mParent(nullptr)
{
}


ggClassyTreeItem::ggClassyTreeItem(const ggClassyCollection* aCollection) :
  mType(cType::eCollection),
  mData(aCollection),
  mParent(nullptr)
{
}


ggClassyTreeItem::ggClassyTreeItem(const ggClassyClass* aClass) :
  mType(cType::eClass),
  mData(aClass),
  mParent(nullptr)
{
}

ggClassyTreeItem::ggClassyTreeItem(const ggClassyClassMember* aMember) :
  mType(cType::eMember),
  mData(aMember),
  mParent(nullptr)
{
}


ggClassyTreeItem::ggClassyTreeItem(const ggClassyFrame* aFrame) :
  mType(cType::eFrame),
  mData(aFrame),
  mParent(nullptr)
{
}


ggClassyTreeItem::~ggClassyTreeItem()
{
  DeleteChildren();
}


template <typename TClassyType>
ggClassyTreeItem* ggClassyTreeItem::AddChild(const TClassyType* aClassyChildItem)
{
  ggClassyTreeItem* vChildItem = new ggClassyTreeItem(aClassyChildItem);
  vChildItem->mParent = this;
  mChildren.insert(vChildItem);
  return vChildItem;
}


template ggClassyTreeItem* ggClassyTreeItem::AddChild<ggClassyDataSet>(const ggClassyDataSet*);
template ggClassyTreeItem* ggClassyTreeItem::AddChild<ggClassyCollection>(const ggClassyCollection*);
template ggClassyTreeItem* ggClassyTreeItem::AddChild<ggClassyClass>(const ggClassyClass*);
template ggClassyTreeItem* ggClassyTreeItem::AddChild<ggClassyClassMember>(const ggClassyClassMember*);
template ggClassyTreeItem* ggClassyTreeItem::AddChild<ggClassyFrame>(const ggClassyFrame*);


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
  switch (mType) {
    case cType::eDataSet: return GetDataSet()->TypeID();
    case cType::eCollection: return GetCollection()->GetName();
    case cType::eClass: return GetClass()->GetName();
    case cType::eMember: return GetMember()->GetName();
    case cType::eFrame: return GetFrame()->TypeID();
    default: {static const QString vName("Goofy"); return vName; }
  }
}


const ggClassyDataSet* ggClassyTreeItem::GetDataSet() const
{
  return (mType == cType::eDataSet) ? static_cast<const ggClassyDataSet*>(mData) : nullptr;
}


const ggClassyCollection* ggClassyTreeItem::GetCollection() const
{
  return (mType == cType::eCollection) ? static_cast<const ggClassyCollection*>(mData) : nullptr;
}


const ggClassyClass* ggClassyTreeItem::GetClass() const
{
  return (mType == cType::eClass) ? static_cast<const ggClassyClass*>(mData) : nullptr;
}


const ggClassyClassMember* ggClassyTreeItem::GetMember() const
{
  return (mType == cType::eMember) ? static_cast<const ggClassyClassMember*>(mData) : nullptr;
}


const ggClassyFrame* ggClassyTreeItem::GetFrame() const
{
  return (mType == cType::eFrame) ? static_cast<const ggClassyFrame*>(mData) : nullptr;
}


bool ggClassyTreeItem::cLess::operator () (const ggClassyTreeItem* aItemA,
                                           const ggClassyTreeItem* aItemB) const
{
  // first compare the types
  if (aItemA->mType != aItemB->mType)
    return (aItemA->mType < aItemB->mType);

  // the types are the same: compare the names
  if (aItemA->GetName() != aItemB->GetName())
    return (aItemA->GetName() < aItemB->GetName());

  // in case of the same name, let's compare the address (pointer)
  return aItemA->mData < aItemB->mData;
}
