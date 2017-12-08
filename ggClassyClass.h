#ifndef GGCLASSYCLASS_H
#define GGCLASSYCLASS_H

#include <QDomElement>

#include "ggTypes.h"
#include "ggSubject.h"
#include "ggClassyClassMember.h"
#include "ggClassyClassDescription.h"

class ggClassyClass : public ggSubject
{
public:

  ggClassyClass();
  ggClassyClass(const QString& aClassName);

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  bool operator() (const ggClassyClass* aClassA, const ggClassyClass* aClassB) const;

  QString GetMembersText() const;
  void SetMembersText(const QString& aText);

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

private:

  static const QString& ClassyBaseClassTypeID();
  QDomElement CreateBaseClassDomElement(QDomDocument& aDocument, const QString& aBaseClassName) const;

public:

  typedef std::vector<ggClassyClassMember> tMembers;

  ggString mClassName;
  ggStringSet mBaseClassNames;
  tMembers mMembers;
  ggClassyDescription mDescription;
  QString mCollectionName;

};

#endif // GGCLASSYCLASS_H
