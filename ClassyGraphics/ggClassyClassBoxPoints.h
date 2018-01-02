#ifndef GGCLASSYGRAPHICSBOXITEMS_H
#define GGCLASSYGRAPHICSBOXITEMS_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyGraphics/ggClassyGraphicsBoxItem.h"

// 3) forward declarations

/**
 * @brief The ggClassyGraphicsBoxItems class
 */
class ggClassyClassBoxPoints

{

public:

  ggClassyClassBoxPoints();

  typedef std::vector<const ggClassyGraphicsBoxItem*> tBoxItemsVec;
  typedef std::set<const ggClassyGraphicsBoxItem*> tBoxItemsSet;
  typedef std::vector<const ggSubjectConnectionPoint*> tPointsVec;
  typedef std::set<const ggSubjectConnectionPoint*> tPointsSet;

  void AddBoxItem(const ggClassyGraphicsBoxItem* aBoxItem);

  const tBoxItemsSet& GetBoxItems() const;
  const tBoxItemsVec& GetBoxItems(const QString& aClassName) const;

  tPointsSet GetClassPointsTop(const QString& aClassName) const;
  tPointsSet GetClassPointsBottom(const QString& aClassName) const;
  tPointsSet GetClassPointsLeftRight(const QString& aClassName) const;

  void Clear();
  void Refresh();

private:

  tBoxItemsSet mBoxItems;

  typedef std::map<QString, tBoxItemsVec> tBoxItemsMap;
  tBoxItemsMap mBoxItemsMap;

};

#endif // GGCLASSYGRAPHICSBOXITEMS_H
