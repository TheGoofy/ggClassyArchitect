#ifndef GGCLASSYCOLLECTION_H
#define GGCLASSYCOLLECTION_H

#include <QPen>
#include <QFont>

#include "Base/ggSubject.h"

class ggClassyCollection :
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  // name
  QString mName;
  // box frame and connections
  QPen mBoxBorder;
  QPen mConnectionLines;
  // class name
  QFont mNameFont;
  QColor mNameColor;
  QBrush mNameBackground;
  // member functions
  QFont mMembersFont;
  QColor mMembersColor;
  QBrush mMembersBackground;
  // description
  QFont mDescriptionFont;
  QColor mDescriptionColor;
  QBrush mDescriptionBackground;

};

#endif // GGCLASSYCOLLECTION_H
