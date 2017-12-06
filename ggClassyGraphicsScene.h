#ifndef GGCLASSYGRAPHICSSCENE_H
#define GGCLASSYGRAPHICSSCENE_H

#include <QGraphicsScene>

class ggClassyGraphicsBoxItem;
class ggClassyGraphicsBoxItems;
class ggClassyDataSet;
class ggClassyClass;

class ggClassyGraphicsScene :
  public QGraphicsScene
{

public:

  ggClassyGraphicsScene(QObject* aParent = nullptr);
  virtual ~ggClassyGraphicsScene();

  void addItem(ggClassyGraphicsBoxItem* aBoxItem);

  void AddClassBoxItems(ggClassyDataSet* aDataSet);
  void AddLineItems();

  void AddTestConnections();

private:

  ggClassyGraphicsBoxItems* mBoxItems;

};

#endif // GGCLASSYGRAPHICSSCENE_H
