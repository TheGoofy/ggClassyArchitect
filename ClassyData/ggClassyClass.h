#ifndef GGCLASSYCLASS_H
#define GGCLASSYCLASS_H

#include <QDomElement>

#include "Base/ggString.h"
#include "Base/ggSubject.h"
#include "ClassyData/ggClassyClassMember.h"
#include "ClassyData/ggClassyDescription.h"

class ggClassyClass : public ggSubject
{
public:

  ggClassyClass();
  ggClassyClass(const QString& aName);

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

  QString mName;
  ggStringSet mBaseClassNames;
  tMembers mMembers;
  ggClassyDescription mDescription;
  QString mCollectionName;

};

#endif // GGCLASSYCLASS_H
