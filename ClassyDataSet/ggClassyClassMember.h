#ifndef GGCLASSYCLASSMEMBER_H
#define GGCLASSYCLASSMEMBER_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * @brief The ggClassyClassMember class
 */
class ggClassyClassMember
{
public:

  ggClassyClassMember(const QString& aName,
                      const QString& aClassName);

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  void SetName(const QString& aName);
  const QString& GetName() const;

  void SetClassName(const QString& aClassName);
  const QString& GetClassName() const;

  bool operator < (const ggClassyClassMember& aOther) const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

private:

  QString mName;
  QString mClassName;

};

#endif // GGCLASSYCLASSMEMBER_H
