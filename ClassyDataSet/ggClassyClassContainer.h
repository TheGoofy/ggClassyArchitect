#ifndef GGCLASSYCLASSCONTAINER_H
#define GGCLASSYCLASSCONTAINER_H

#include <QDomElement>

#include "Base/ggSubject.h"
#include "ClassyDataSet/ggClassyClass.h"

class ggClassyClassContainer :
  public std::set<ggClassyClass*, ggClassyClass>,
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  ggClassyClass* FindClass(const QString& aClassName) const;

};

#endif // GGCLASSYCLASSCONTAINER_H
