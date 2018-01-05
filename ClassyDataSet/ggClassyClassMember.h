#ifndef GGCLASSYCLASSMEMBER_H
#define GGCLASSYCLASSMEMBER_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyItem.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassMember class
 */
class ggClassyClassMember :
  public ggClassyItem
{
public:

  ggClassyClassMember();
  ggClassyClassMember(const QString& aName,
                      const QString& aClassName);

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const override;

  // main members
  void SetName(const QString& aName);
  const QString& GetName() const;
  void SetClassName(const QString& aClassName);
  const QString& GetClassName() const;

  // check or compare content
  bool Empty() const;
  bool operator < (const ggClassyClassMember& aOther) const;

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyClassMember Create(const QDomElement& aElement);

private:

  QString mName;
  QString mClassName;

};

#endif // GGCLASSYCLASSMEMBER_H
