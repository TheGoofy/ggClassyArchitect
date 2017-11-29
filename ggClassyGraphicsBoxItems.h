#ifndef GGCLASSYGRAPHICSBOXITEMS_H
#define GGCLASSYGRAPHICSBOXITEMS_H

#include "ggClassyGraphicsBoxItem.h"
#include "ggClassyDataSet.h"

class ggClassyGraphicsBoxItems :
  public ggSubject

{

public:

  ggClassyGraphicsBoxItems();

  typedef std::vector<ggClassyGraphicsBoxItem*> tBoxItemsVec;

  void AddItem(ggClassyGraphicsBoxItem* aBoxItem);
  const tBoxItemsVec& GetBoxItems(const QString& aClassName) const;

private:

  typedef std::map<QString, tBoxItemsVec> tBoxItemsMap;
  tBoxItemsMap mBoxItemsMap;

};

#endif // GGCLASSYGRAPHICSBOXITEMS_H
