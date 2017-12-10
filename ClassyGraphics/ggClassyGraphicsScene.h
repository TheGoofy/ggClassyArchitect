#ifndef GGCLASSYGRAPHICSSCENE_H
#define GGCLASSYGRAPHICSSCENE_H

// 1) include system or QT
#include <QGraphicsScene>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations
class ggClassyGraphicsBoxItem;
class ggClassyGraphicsBoxItems;
class ggClassyDataSet;
class ggClassyClass;

/**
 * @brief The ggClassyGraphicsScene class
 */
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
