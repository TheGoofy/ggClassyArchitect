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
  ggBehaviorLazy::Executor vLazy(mBoxItems);
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(aDataSet->mClassBoxes);
  while (vClassBoxesIterator) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    addItem(new ggClassyGraphicsBoxItem(vClassBox));
  }
}


void ggClassyGraphicsScene::AddLineItems(ggClassyDataSet* aDataSet)
{
  ggBehaviorLazy::Executor vLazy(mBoxItems);
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesIterator(aDataSet->mClassBoxes);
  while (vClassBoxesIterator) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    if (vClassBox == nullptr) continue;
    if (vClassBox->mClass == nullptr) continue;
    ggWalkerT<ggStrings::iterator> vBaseClassNameWalker(vClassBox->mClass->mBaseClassNames);
    while (vBaseClassNameWalker) {
      const QString& vClassName = vClassBox->mClass->mName;
      const QString& vBaseClassNames = *vBaseClassNameWalker;
      ggClassyGraphicsPathItem* vLine = new ggClassyGraphicsPathItem();
      vLine->SetBoxItems(mBoxItems);
      vLine->SetConnection(vClassName, vBaseClassNames);
      QGraphicsScene::addItem(vLine);
    }
  }
}
