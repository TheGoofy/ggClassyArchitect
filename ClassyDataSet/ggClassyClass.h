#ifndef GGCLASSYCLASS_H
#define GGCLASSYCLASS_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "Base/ggString.h"
#include "Base/ggSubject.h"
#include "ClassyDataSet/ggClassyClassMember.h"
#include "ClassyDataSet/ggClassyDescription.h"

// 3) forward declarations
class ggClassyDataSet;

/**
 * @brief The ggClassyClass class
 */
class ggClassyClass :
  public ggSubject
{
public:

  ggClassyClass(ggClassyDataSet* aDataSet = nullptr);

  ggClassyClass(const QString& aName,
                ggClassyDataSet* aDataSet = nullptr);

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  bool operator() (const ggClassyClass* aClassA,
                   const ggClassyClass* aClassB) const;

  const QString& GetName() const;
  bool SetName(const QString& aName);

  QString GetMembersText() const;
  void SetMembersText(const QString& aText);

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

private:

  static const QString& ClassyBaseClassTypeID();
  QDomElement CreateBaseClassDomElement(QDomDocument& aDocument,
                                        const QString& aBaseClassName) const;

public:

  typedef std::vector<ggClassyClassMember> tMembers;

  ggStringSet mBaseClassNames;
  tMembers mMembers;
  ggClassyDescription mDescription;
  QString mCollectionName;

private:

  // exceptional access for dataset
  friend class ggClassyDataSet;
  ggClassyDataSet* mDataSet;

  QString mName;

};

#endif // GGCLASSYCLASS_H
