#ifndef GGCLASSYCLASSDESCRIPTION_H
#define GGCLASSYCLASSDESCRIPTION_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyItem.h"

// 3) forward declarations

/**
 * @brief The ggClassyDescription class
 */
class ggClassyDescription :
  public ggClassyItem,
  public QString
{
public:

  template <typename T>
  ggClassyDescription& operator = (const T& aOther) {
    QString::operator = (aOther);
    return *this;
  }

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const override;

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyDescription Create(const QDomElement& aElement);

};

#endif // GGCLASSYCLASSDESCRIPTION_H
