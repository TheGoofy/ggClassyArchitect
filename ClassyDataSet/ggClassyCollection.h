#ifndef GGCLASSYCOLLECTION_H
#define GGCLASSYCOLLECTION_H

// 1) include system or QT
#include <QPen>
#include <QFont>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

/**
 * @brief The ggClassyCollection class
 */
class ggClassyCollection :
  public ggSubject
{
public:

  // identification
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
