#ifndef GGCLASSYCLASSMEMBER_H
#define GGCLASSYCLASSMEMBER_H

#include <QDomElement>

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
