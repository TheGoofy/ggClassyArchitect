#ifndef GGCLASSYCLASSCONTAINER_H
#define GGCLASSYCLASSCONTAINER_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "ClassyDataSet/ggClassyClass.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassContainer class
 */
class ggClassyClassContainer :
  public ggSubject
{
public:

  // construction / destruction
  ggClassyClassContainer();
  ggClassyClassContainer(const ggClassyClassContainer& aOther);
  virtual ~ggClassyClassContainer();

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  // assignment operator
  ggClassyClassContainer& operator = (const ggClassyClassContainer& aOther);

  ggClassyClass* AddClass(ggClassyClass* aClass);
  ggClassyClass* FindClass(const QString& aClassName);
  const ggClassyClass* FindClass(const QString& aClassName) const;
  bool RenameClass(const QString& aOldClassName, const QString& aNewClassName);
  void DeleteClass(const QString& aClassName);
  void DeleteAllClasses();

  ggUSize GetSize() const;
  ggClassyClass* SearchClass(ggUSize aIndex) const;
  ggUSize SearchIndex(const ggClassyClass* aClass) const;

  typedef std::set<ggClassyClass*, ggClassyClass>::iterator iterator;
  iterator begin();
  iterator end();

  typedef std::set<ggClassyClass*, ggClassyClass>::const_iterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;

private:

  std::set<ggClassyClass*, ggClassyClass> mClasses;

};

#endif // GGCLASSYCLASSCONTAINER_H
