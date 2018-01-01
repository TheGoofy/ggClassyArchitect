#ifndef GGCLASSYGRAPHICSSCENE_H
#define GGCLASSYGRAPHICSSCENE_H

// 1) include system or QT
#include <QGraphicsScene>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"

// 3) forward declarations
class ggClassyClassBox;
class ggClassyGraphicsBoxItem;
class ggClassyClassBoxPoints;
class ggClassyDataSet;
class ggClassyClass;
class ggClassyFrame;

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

  void MoveSelectedItemsUp();
  void MoveSelectedItemsDown();
  void MoveSelectedItemsTop();
  void MoveSelectedItemsBottom();

  typedef std::set<const ggClassyClassBox*> tClassBoxes;
  tClassBoxes GetSelectedClassBoxes() const;
  void SelectClassBoxes(const tClassBoxes& aClassBoxes);

  typedef std::set<const ggClassyFrame*> tFrames;
  tFrames GetSelectedFrames() const;
  void SelectFrames(const tFrames& aFrames);

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  void DeleteItems(const std::vector<QGraphicsItem*>& aItems);
  void DeleteClassBoxAndConnectionItems();
  void DeleteConnectionItems();
  void DeleteFrameItems();

  void CreateClassBoxItems();
  void CreateConnectionItems();
  void CreateFrameItems();

  void AddTestConnections();

  ggClassyDataSet* mDataSet;

  ggClassyClassBoxPoints* mBoxPoints;

};

#endif // GGCLASSYGRAPHICSSCENE_H
