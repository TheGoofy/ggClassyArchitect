#ifndef GGCLASSYCLASSDESCRIPTION_H
#define GGCLASSYCLASSDESCRIPTION_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * @brief The ggClassyDescription class
 */
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
