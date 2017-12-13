#ifndef GGCLASSYCLASSCONTAINER_H
#define GGCLASSYCLASSCONTAINER_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyClass.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassContainer class
 */
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
