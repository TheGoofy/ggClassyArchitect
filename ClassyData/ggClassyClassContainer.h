#ifndef GGCLASSYCLASSCONTAINER_H
#define GGCLASSYCLASSCONTAINER_H

#include <QDomElement>

#include "Base/ggSubject.h"
#include "ClassyData/ggClassyClass.h"

class ggClassyClassContainer :
  public std::set<ggClassyClass*, ggClassyClass>,
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

  ggClassyClass* FindClass(const QString& aClassName) const;

  bool RenameClass(const QString& aOldClassName,
                   const QString& aNewClassName);

};

#endif // GGCLASSYCLASSCONTAINER_H
