#ifndef GGCLASSYCOLLECTION_H
#define GGCLASSYCOLLECTION_H

// 1) include system or QT
#include <QPen>
#include <QFont>
#include <QDomElement>

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

  void SetDataSet(ggClassyDataSet* aDataSet);
  ggClassyDataSet* GetDataSet() const;

  const QString& GetName() const;
  void SetName(const QString& aName);

  const QBrush& GetNameBackground() const;
  void SetNameBackground(const QBrush& aBrush);

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyCollection* Create(const QDomElement& aElement, ggClassyDataSet* aDataSet = nullptr);

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

private:

  void Construct();

  // name
  QString mName;

  // owning dataset (needed for checking unique collection name)
  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYCOLLECTION_H
