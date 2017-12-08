#ifndef GGCLASSYCLASSDESCRIPTION_H
#define GGCLASSYCLASSDESCRIPTION_H

#include <QDomElement>

class ggClassyDescription :
  public QString
{
public:

  template <typename T>
  ggClassyDescription& operator = (const T& aOther) {
    QString::operator = (aOther);
    return *this;
  }

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

};

#endif // GGCLASSYCLASSDESCRIPTION_H
