#include "ggClassyGraphicsScene.h"

#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyGraphicsPathItem.h"


ggClassyGraphicsScene::ggClassyGraphicsScene(QObject* aParent) :
  QGraphicsScene(aParent),
  mBoxItems(nullptr)
{
  mBoxItems = new ggClassyGraphicsBoxItems();
}


ggClassyGraphicsScene::~ggClassyGraphicsScene()
{
  delete mBoxItems;
}


void ggClassyGraphicsScene::addItem(ggClassyGraphicsBoxItem* aBoxItem)
{
  mBoxItems->AddItem(aBoxItem);
  QGraphicsScene::addItem(aBoxItem);
}


void ggClassyGraphicsScene::AddClassBoxItems(ggClassyDataSet* aDataSet)
{
  // only notify box items change, when all boxes are added
  ggBehaviorLazy::Executor vLazy(mBoxItems);

  // loop over box items
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(aDataSet->mClassBoxes);
  while (vClassBoxesIterator) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    addItem(new ggClassyGraphicsBoxItem(vClassBox));
  }
}


void ggClassyGraphicsScene::AddLineItems(ggClassyDataSet* aDataSet)
{
  // only notify box items, when all are added
  ggBehaviorLazy::Executor vLazy(mBoxItems);

  // loop over classes
  typedef std::set<ggClassyClass*> tClasses;
  ggWalkerT<tClasses::const_iterator> vClassesWalker(aDataSet->mClasses);
  while (vClassesWalker) {
    const ggClassyClass* vClass = *vClassesWalker;

    // loop over base classes
    ggWalkerT<ggStrings::const_iterator> vBaseClassNameWalker(vClass->mBaseNames);
    while (vBaseClassNameWalker) {

      // lines between derived class and base class(es)
      const QString& vClassName = vClass->mName;
      const QString& vBaseClassNames = *vBaseClassNameWalker;
      ggClassyGraphicsPathItem* vLine = new ggClassyGraphicsPathItem();
      vLine->SetBoxItems(mBoxItems);
      vLine->SetConnection(vClassName, vBaseClassNames);
      QGraphicsScene::addItem(vLine);
    }
  }
}
