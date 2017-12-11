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
  mDataSet(aDataSet),
  mName(aName)
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


bool ggClassyClass::operator() (const ggClassyClass* aClassA,
                                const ggClassyClass* aClassB) const
{
  if (aClassA == nullptr || aClassB == nullptr)
    return aClassA < aClassB;
  else
    return aClassA->mName < aClassB->mName;
}


const QString& ggClassyClass::GetName() const
{
  return mName;
}

#include <QDebug>
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
    vText += vMemberName;
    if (vMemberClassName != "") vText += "\t" + vMemberClassName;
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
      QStringList vMemberNameAndClass = vMemberText.split("\t", QString::SkipEmptyParts);
      QString vMemberName("");
      QString vMemberClassName("");
      if (vMemberNameAndClass.size() >= 1) vMemberName = vMemberNameAndClass.at(0).simplified();
      if (vMemberNameAndClass.size() >= 2) vMemberClassName = vMemberNameAndClass.at(1).simplified();
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
  mDescription = aDescription;
  Notify();
}
