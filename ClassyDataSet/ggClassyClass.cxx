// 0) include own header
#include "ggClassyClass.h"

// 1) include system or QT
#include <QStringList>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyClass::ggClassyClass(ggClassyDataSet* aDataSet) :
  mDataSet(aDataSet)
{
}


ggClassyClass::ggClassyClass(const QString& aName,
                             ggClassyDataSet* aDataSet) :
  mName(aName),
  mDataSet(aDataSet)
{
}


ggClassyClass::ggClassyClass(const ggClassyClass& aOther) :
  mName(aOther.mName),
  mBaseClassNames(aOther.mBaseClassNames),
  mMembers(aOther.mMembers),
  mDescription(aOther.mDescription),
  mCollectionName(aOther.mCollectionName),
  mDataSet(nullptr)
{
}


ggClassyClass::~ggClassyClass()
{
}


const QString& ggClassyClass::TypeID()
{
  static const QString vTypeID("ggClassyClass");
  return vTypeID;
}


const QString& ggClassyClass::VTypeID() const
{
  return TypeID();
}


const QString& ggClassyClass::ClassyBaseClassTypeID()
{
  static const QString vTypeID("ggClassyBaseClass");
  return vTypeID;
}


QDomElement ggClassyClass::CreateBaseClassDomElement(QDomDocument& aDocument,
                                                     const QString& aBaseClassName) const
{
  QDomElement vElement = aDocument.createElement(ClassyBaseClassTypeID());
  vElement.setAttribute("mClassName", aBaseClassName);
  return vElement;
}


QString ggClassyClass::CreateBaseClassName(const QDomElement& aElement)
{
  QString vBaseClassName = "";
  if (aElement.tagName() == ClassyBaseClassTypeID()) {
    vBaseClassName = aElement.attribute("mClassName");
  }
  return vBaseClassName;
}


QDomElement ggClassyClass::CreateDomElement(QDomDocument& aDocument) const
{
  // main node
  QDomElement vElement = aDocument.createElement(TypeID());

  // name and collection
  vElement.setAttribute("mName", mName);
  vElement.setAttribute("mCollectionName", mCollectionName);

  // base classes
  ggWalkerT<ggStringSet::iterator> vBaseClassNamesIterator(mBaseClassNames);
  while (vBaseClassNamesIterator) {
    vElement.appendChild(CreateBaseClassDomElement(aDocument, *vBaseClassNamesIterator));
  }

  // members
  ggWalkerT<tMembers::const_iterator> vMembersWalker(mMembers);
  while (vMembersWalker) {
    vElement.appendChild((*vMembersWalker).CreateDomElement(aDocument));
  }

  // description
  vElement.appendChild(mDescription.CreateDomElement(aDocument));

  // return dom node
  return vElement;
}


ggClassyClass* ggClassyClass::Create(const QDomElement& aElement, ggClassyDataSet* aDataSet)
{
  ggClassyClass* vClass = nullptr;
  if (aElement.tagName() == TypeID()) {

    // class name and collection
    vClass = new ggClassyClass(aElement.attribute("mName"), aDataSet);
    vClass->mCollectionName = aElement.attribute("mCollectionName");

    // parse children
    QDomElement vChildElement = aElement.firstChildElement();
    while (!vChildElement.isNull()) {

      // base classes
      QString vBaseClassName = CreateBaseClassName(vChildElement);
      if (vBaseClassName != "") vClass->mBaseClassNames.insert(vBaseClassName);

      // members
      ggClassyClassMember vMember = ggClassyClassMember::Create(vChildElement);
      if (!vMember.Empty()) vClass->mMembers.push_back(vMember);

      // description
      ggClassyDescription vDescription = ggClassyDescription::Create(vChildElement);
      if (vDescription != "") vClass->mDescription = vDescription;

      // next child
      vChildElement = vChildElement.nextSiblingElement();
    }
  }
  return vClass;
}


ggClassyClass& ggClassyClass::operator = (const ggClassyClass& aOther)
{
  // copy all except "mDataSet"
  mName = aOther.mName;
  mBaseClassNames = aOther.mBaseClassNames;
  mMembers = aOther.mMembers;
  mDescription = aOther.mDescription;
  mCollectionName = aOther.mCollectionName;
  return *this;
}


const QString& ggClassyClass::GetName() const
{
  return mName;
}


bool ggClassyClass::SetName(const QString& aName)
{
  if (aName == mName) return true;
  if (mDataSet != nullptr) {
    // renames also dependent members from other classes (and notifies the changes)
    return mDataSet->RenameClass(mName, aName);
  }
  else {
    mName = aName;
    Notify();
    return true;
  }
}


const ggStringSet& ggClassyClass::GetBaseClassNames() const
{
  return mBaseClassNames;
}


void ggClassyClass::AddBaseClassName(const QString& aBaseClassName)
{
  if (mBaseClassNames.insert(aBaseClassName).second) Notify();
}


void ggClassyClass::RemoveBaseClassName(const QString& aBaseClassName)
{
  if (mBaseClassNames.erase(aBaseClassName)) Notify();
}


void ggClassyClass::RemoveAllBaseClassNames()
{
  if (!mBaseClassNames.empty()) {
    mBaseClassNames.clear();
    Notify();
  }
}


void ggClassyClass::AddMember(const QString& aName, const QString& aClassName)
{
  mMembers.push_back(ggClassyClassMember(aName, aClassName));
  Notify();
}


const ggClassyClass::tMembers& ggClassyClass::GetMembers() const
{
  return mMembers;
}


QString ggClassyClass::GetMembersText() const
{
  QString vText;
  ggWalkerT<tMembers::const_iterator> vMemberIterator(mMembers);
  while (vMemberIterator) {
    const ggClassyClassMember& vMember = *vMemberIterator;
    const QString& vMemberName = vMember.GetName();
    const QString& vMemberClassName = vMember.GetClassName();
    if (vMemberClassName != "") vText += vMemberClassName + " - " + vMemberName;
    else vText += vMemberName;
    if (!vMemberIterator.IsLast()) vText += "\n";
  }
  return vText;
}


void ggClassyClass::SetMembersText(const QString& aText)
{
  if (aText != GetMembersText()) {
    mMembers.clear();
    QStringList vMembersText = aText.split("\n");
    ggWalkerT<QStringList::iterator> vMembersTextWalker(vMembersText);
    while (vMembersTextWalker) {
      const QString& vMemberText = *vMembersTextWalker;
      QStringList vMemberNameAndClass = vMemberText.split(" - ", QString::SkipEmptyParts);
      QString vMemberName("");
      QString vMemberClassName("");
      if (vMemberNameAndClass.size() >= 1) vMemberClassName = vMemberNameAndClass.at(0).simplified();
      if (vMemberNameAndClass.size() >= 2) vMemberName = vMemberNameAndClass.at(1).simplified();
      mMembers.push_back(ggClassyClassMember(vMemberName, vMemberClassName));
    }
    Notify();
  }
}


const QString& ggClassyClass::GetDescription() const
{
  return mDescription;
}


void ggClassyClass::SetDescription(const QString& aDescription)
{
  if (aDescription != mDescription) {
    mDescription = aDescription;
    Notify();
  }
}


const QString& ggClassyClass::GetCollectionName() const
{
  return mCollectionName;
}


void ggClassyClass::SetCollectionName(const QString& aName)
{
  if (aName != mCollectionName) {
    mCollectionName = aName;
    Notify();
  }
}


const ggClassyDataSet* ggClassyClass::GetDataSet() const
{
  return mDataSet;
}


void ggClassyClass::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
    Notify();
  }
}


bool ggClassyClass::RenameClass(const QString& aOldClassName,
                                const QString& aNewClassName)
{
  // if the name is unchanged, renaming is obsolete (success)
  if (aNewClassName == aOldClassName) return true;

  // single collective notification
  ggSubject::cExecutorLazy vLazy(this);

  // change own name
  if (mName == aOldClassName) {
    mName = aNewClassName;
    Notify();
  }

  // replace a base class name (if there is)
  if (mBaseClassNames.erase(aOldClassName)) {
    mBaseClassNames.insert(aNewClassName);
    Notify();
  }

  // replace member class name(s)
  ggWalkerT<tMembers::iterator> vMembersWalker(mMembers);
  while (vMembersWalker) {
    ggClassyClassMember& vMember = *vMembersWalker;
    if (vMember.GetClassName() == aOldClassName) {
      vMember.SetClassName(aNewClassName);
      Notify();
    }
  }

  // renam succeeded
  return true;
}
