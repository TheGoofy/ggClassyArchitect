#ifndef GGCLASSYCOLLECTION_H
#define GGCLASSYCOLLECTION_H

// 1) include system or QT
#include <QPen>
#include <QFont>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations
class ggClassyDataSet;

/**
 * @brief The ggClassyCollection class
 */
class ggClassyCollection :
  public ggSubject
{
public:

  ggClassyCollection();
  ggClassyCollection(const QString& aName);
  virtual ~ggClassyCollection();

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  // compares two pointers by comparing their "mName"
  bool operator() (const ggClassyCollection* aCollectionA,
                   const ggClassyCollection* aCollectionB) const;

  void SetDataSet(ggClassyDataSet* aDataSet);
  ggClassyDataSet* GetDataSet() const;

  const QString& GetName() const;
  void SetName(const QString& aName);

private:

  // name
  QString mName;

  /*
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
  */

  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYCOLLECTION_H
