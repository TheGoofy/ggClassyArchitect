#ifndef GGCLASSYCLASSBOX_H
#define GGCLASSYCLASSBOX_H

// 1) include system or QT
#include <QDomElement>
#include <QPointF>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassBox class
 */
class ggClassyClassBox :
  public ggSubject
{
public:

  ggClassyClassBox(const QString& aClassName);

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

  QString mClassName;
  QPointF mPosition;
  float mWidth;
  bool mMembersVisible;
  bool mDescriptionVisible;
};

#endif // GGCLASSYCLASSBOX_H
