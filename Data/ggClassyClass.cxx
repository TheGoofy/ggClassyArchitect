#include "ggClassyClass.h"

#include <QStringList>

#include "Base/ggWalkerT.h"


ggClassyClass::ggClassyClass()
{
}


ggClassyClass::ggClassyClass(const QString& aClassName) :
  mClassName(aClassName)
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
  vElement.setAttribute("mClassName", mClassName);
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
    return aClassA->mClassName < aClassB->mClassName;
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
}
