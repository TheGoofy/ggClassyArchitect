// 0) include own header
#include "ggClassyClassContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


ggClassyClassContainer::ggClassyClassContainer()
{
}


ggClassyClassContainer::ggClassyClassContainer(const ggClassyClassContainer& aOther)
{
  *this = aOther;
}


ggClassyClassContainer::~ggClassyClassContainer()
{
  DeleteAllClasses();
}


const QString& ggClassyClassContainer::TypeID()
{
  static const QString vTypeID("ggClassyClassContainer");
  return vTypeID;
}


const QString& ggClassyClassContainer::VTypeID() const
{
  return TypeID();
}


ggClassyClass* ggClassyClassContainer::AddClass(ggClassyClass* aClass)
{
  // only add, if name is unique
  if (aClass != nullptr) {
    if (FindClass(aClass->GetName()) == nullptr) {
      mClasses.insert(aClass);
      Notify();
      return aClass;
    }
  }
  return nullptr;
}


ggClassyClass* ggClassyClassContainer::FindClass(const QString& aClassName)
{
  ggClassyClass vClass(aClassName);
  std::set<ggClassyClass*, ggClassyClass>::iterator vClassesIterator = mClasses.find(&vClass);
  if (vClassesIterator != mClasses.end()) return *vClassesIterator;
  else return nullptr;
}


const ggClassyClass* ggClassyClassContainer::FindClass(const QString& aClassName) const
{
  ggClassyClass vClass(aClassName);
  std::set<ggClassyClass*, ggClassyClass>::const_iterator vClassesIterator = mClasses.find(&vClass);
  if (vClassesIterator != mClasses.end()) return *vClassesIterator;
  else return nullptr;
}


bool ggClassyClassContainer::RenameClass(const QString& aOldClassName,
                                         const QString& aNewClassName)
{
  // if the name is unchanged, renaming is obsolete (success)
  if (aNewClassName == aOldClassName) return true;
  // can't rename, if there is no class with the "old" name (fail)
  if (FindClass(aOldClassName) == nullptr) return false;
  // can't rename, if there is another class with the "new" name (fail)
  if (FindClass(aNewClassName) != nullptr) return false;

  // class must not be renamed, while it's stored in a set with the name as a key
  ggClassyClass* vClassToRename = FindClass(aOldClassName);
  mClasses.erase(vClassToRename);

  // change base classes and members of all other classes
  ggWalkerT<iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    // get the class pointer
    ggClassyClass* vClass = *vClassesWalker;
    // rename the class or it's members
    vClass->RenameClass(aOldClassName, aNewClassName);
  }

  // now rename the class, and re-insert it
  vClassToRename->RenameClass(aOldClassName, aNewClassName);
  mClasses.insert(vClassToRename);

  // done (success)
  Notify();
  return true;
}


ggClassyClassContainer& ggClassyClassContainer::operator = (const ggClassyClassContainer& aOther)
{
  // one collective notification (and not each individual class)
  ggSubject::cExecutorLazy vLazy(this);

  // delete all classes
  DeleteAllClasses();

  // add (and notify) copies
  ggWalkerT<ggClassyClassContainer::const_iterator> vOtherClassesWalker(aOther.mClasses);
  while (vOtherClassesWalker) {
    AddClass(new ggClassyClass(**vOtherClassesWalker));
  }

  return *this;
}


void ggClassyClassContainer::DeleteClass(const QString& aClassName)
{
  ggClassyClass vClassToFind(aClassName);
  std::set<ggClassyClass*, ggClassyClass>::iterator vClassesIterator = mClasses.find(&vClassToFind);
  if (vClassesIterator != mClasses.end()) {
    ggClassyClass* vClassToDelete = *vClassesIterator;
    mClasses.erase(vClassToDelete);
    delete vClassToDelete;
    Notify();
  }
}


void ggClassyClassContainer::DeleteAllClasses()
{
  ggWalkerT<ggClassyClassContainer::iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    delete *vClassesWalker;
  }
  mClasses.clear();
  Notify();
}


ggClassyClassContainer::iterator ggClassyClassContainer::begin()
{
  return mClasses.begin();
}


ggClassyClassContainer::iterator ggClassyClassContainer::end()
{
  return mClasses.end();
}


ggClassyClassContainer::const_iterator ggClassyClassContainer::begin() const
{
  return mClasses.begin();
}


ggClassyClassContainer::const_iterator ggClassyClassContainer::end() const
{
  return mClasses.end();
}
