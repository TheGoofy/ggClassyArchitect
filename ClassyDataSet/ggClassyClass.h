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

  // construction / destruction
  ggClassyClass(ggClassyDataSet* aDataSet = nullptr);
  ggClassyClass(const QString& aName,
                ggClassyDataSet* aDataSet = nullptr);
  ggClassyClass(const ggClassyClass& aOther);
  virtual ~ggClassyClass();

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  // assignment operator
  ggClassyClass& operator = (const ggClassyClass& aOther);

  // assigns a new name. if the class is part of a dataset, all items pointing to this class are adjusted too
  bool SetName(const QString& aName);
  const QString& GetName() const;

  // other members ...
  const ggStringSet& GetBaseClassNames() const;
  void AddBaseClassName(const QString& aBaseClassName);
  void RemoveBaseClassName(const QString& aBaseClassName);
  void RemoveAllBaseClassNames();
  void AddMember(const QString& aName, const QString& aClassName);
  typedef std::vector<ggClassyClassMember> tMembers;
  const tMembers& GetMembers() const;
  QString GetMembersText() const;
  void SetMembersText(const QString& aText);
  const QString& GetDescription() const;
  void SetDescription(const QString& aDescription);
  const QString& GetCollectionName() const;
  void SetCollectionName(const QString& aName);
  const ggClassyDataSet* GetDataSet() const;
  void SetDataSet(ggClassyDataSet* aDataSet);

  // renames the class, and classes of all the members (ignores the dataset)
  bool RenameClass(const QString& aOldClassName, const QString& aNewClassName);

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyClass* Create(const QDomElement& aElement, ggClassyDataSet* aDataSet = nullptr);

private:

  static const QString& ClassyBaseClassTypeID();
  QDomElement CreateBaseClassDomElement(QDomDocument& aDocument, const QString& aBaseClassName) const;
  static QString CreateBaseClassName(const QDomElement& aElement);

  QString mName;
  ggStringSet mBaseClassNames;
  tMembers mMembers;
  ggClassyDescription mDescription;
  QString mCollectionName;

  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYCLASS_H
