#ifndef GGCLASSYGRAPHICSSCENE_H
#define GGCLASSYGRAPHICSSCENE_H

// 1) include system or QT
#include <QGraphicsScene>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"

// 3) forward declarations
class ggClassyGraphicsBoxItem;
class ggClassyClassBoxPoints;
class ggClassyDataSet;
class ggClassyClass;

/**
 * @brief The ggClassyGraphicsScene class
 */
class ggClassyGraphicsScene :
  public QGraphicsScene,
  public ggObserver
{

public:

  ggClassyGraphicsScene(QObject* aParent = nullptr);
  virtual ~ggClassyGraphicsScene();

  void SetDataSet(ggClassyDataSet* aDataSet);

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  void DeleteItems(const std::vector<QGraphicsItem*>& aItems);
  void DeleteClassBoxItems();
  void DeleteConnectionItems();

  void CreateClassBoxItems();
  void CreateConnectionItems();

  void AddTestConnections();

  ggClassyDataSet* mDataSet;

  ggClassyClassBoxPoints* mBoxPoints;

};

#endif // GGCLASSYGRAPHICSSCENE_H
