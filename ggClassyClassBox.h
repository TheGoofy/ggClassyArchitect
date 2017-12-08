#ifndef GGCLASSYCLASSBOX_H
#define GGCLASSYCLASSBOX_H

#include <QDomElement>
#include <QPointF>

#include "ggSubject.h"

class ggClassyClassBox : public ggSubject
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
