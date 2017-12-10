#ifndef GGCLASSYGRAPHICSSCENE_H
#define GGCLASSYGRAPHICSSCENE_H

// 1) include system or QT
#include <QGraphicsScene>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"

// 3) forward declarations
class ggClassyGraphicsBoxItem;
class ggClassyGraphicsBoxItems;
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

  void AddItem(ggClassyGraphicsBoxItem* aBoxItem);

  void SetDataSet(ggClassyDataSet* aDataSet);

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  void ClearAll();

  void UpdateBoxItems();
  void UpdateLineItems();

  void AddTestConnections();

  ggClassyDataSet* mDataSet;
  ggClassyGraphicsBoxItems* mBoxItems;

};

#endif // GGCLASSYGRAPHICSSCENE_H
