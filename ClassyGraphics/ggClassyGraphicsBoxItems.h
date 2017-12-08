#ifndef GGCLASSYGRAPHICSBOXITEMS_H
#define GGCLASSYGRAPHICSBOXITEMS_H

#include "ClassyGraphics/ggClassyGraphicsBoxItem.h"
#include "ClassyData/ggClassyDataSet.h"

class ggClassyGraphicsBoxItems :
  public ggSubject

{

public:

  ggClassyGraphicsBoxItems();

  typedef std::vector<const ggClassyGraphicsBoxItem*> tBoxItemsVec;
  typedef std::set<const ggClassyGraphicsBoxItem*> tBoxItemsSet;
  typedef std::vector<const ggSubjectConnectionPoint*> tPointsVec;
  typedef std::set<const ggSubjectConnectionPoint*> tPointsSet;

  void AddItem(ggClassyGraphicsBoxItem* aBoxItem);

  const tBoxItemsSet& GetBoxItems() const;
  const tBoxItemsVec& GetBoxItems(const QString& aClassName) const;

  tPointsSet GetClassPointsTop(const QString& aClassName) const;
  tPointsSet GetClassPointsBottom(const QString& aClassName) const;
  tPointsSet GetClassPointsLeftRight(const QString& aClassName) const;
  tPointsSet GetClassPointsMembers(const QString& aClassName, ggUSize aMemberIndex) const;

private:

  tBoxItemsSet mBoxItems;

  typedef std::map<QString, tBoxItemsVec> tBoxItemsMap;
  tBoxItemsMap mBoxItemsMap;

};

#endif // GGCLASSYGRAPHICSBOXITEMS_H
