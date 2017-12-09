#include "ggClassyClassContainer.h"

#include <QDebug>

#include "Base/ggWalkerT.h"


const QString& ggClassyClassContainer::TypeID()
{
  static const QString vTypeID("ggClassyClassContainer");
  return vTypeID;
}


const QString& ggClassyClassContainer::VTypeID() const
{
  return TypeID();
}


ggClassyClass* ggClassyClassContainer::FindClass(const QString& aClassName) const
{
  ggClassyClass vClass(aClassName);
  const_iterator vClassesIterator = find(&vClass);
  if (vClassesIterator != end()) return *vClassesIterator;
  else return nullptr;
}


bool ggClassyClassContainer::RenameClass(const QString& aOldClassName,
                                         const QString& aNewClassName)
{
  qDebug() << __PRETTY_FUNCTION__ << aOldClassName << "=>" << aNewClassName;

  // if the name is unchanged, renaming is obsolete (success)
  if (aNewClassName == aOldClassName) return true;
  // can't rename, if there is no class with the "old" name (fail)
  if (FindClass(aOldClassName) == nullptr) return false;
  // can't rename, if there is another class with the "new" name (fail)
  if (FindClass(aNewClassName) != nullptr) return false;

  // change base classes and members of all other classes
  ggWalkerT<iterator> vClassesWalker(*this);
  while (vClassesWalker) {
    // get the class pointer
    ggClassyClass* vClass = *vClassesWalker;
    // be lazy: collect multiple notifications and only notify once
    ggClassyClass::cExecutorLazy vExecutorLazy(vClass);
    // replace a base class name (if there is)
    if (vClass->mBaseClassNames.erase(aOldClassName)) {
      vClass->mBaseClassNames.insert(aNewClassName);
      vClass->Notify();
    }
    // replace member class name(s)
    ggWalkerT<ggClassyClass::tMembers::iterator> vMembersWalker(vClass->mMembers);
    while (vMembersWalker) {
      ggClassyClassMember& vMember = *vMembersWalker;
      if (vMember.GetClassName() == aOldClassName) {
        vMember.SetClassName(aNewClassName);
        vClass->Notify();
      }
    }
  }

  // Since "aOldClassName" is passed as a reference, we have to change the actual
  // class name "vClass->mClassName" at the very last. It (may) happens that "aOldClassName"
  // magically changes to the new class name! ... AND the class can't be renamed, while
  // it is stored in a set, because the name is used as the key.
  ggClassyClass* vClass = FindClass(aOldClassName);
  // temporarily remove it from the set
  erase(vClass);
  // rename it
  vClass->mClassName = aNewClassName;
  // insert it again
  insert(vClass);
  // notify the change
  vClass->Notify();

  // content of this container changed
  Notify();

  // done (success)
  return true;
}
