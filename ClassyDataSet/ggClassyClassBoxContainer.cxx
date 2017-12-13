// 0) include own header
#include "ggClassyClassBoxContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


const QString& ggClassyClassBoxContainer::TypeID()
{
  static QString vTypeID("ggClassyClassBoxContainer");
  return vTypeID;
}


const QString& ggClassyClassBoxContainer::VTypeID() const
{
  return TypeID();
}


ggClassyClassBoxContainer::const_iterator ggClassyClassBoxContainer::end() const
{
  return mClassBoxes.end();
}


ggClassyClassBox* ggClassyClassBoxContainer::AddClassBox(ggClassyClassBox* aClassBox)
{
  if (aClassBox != nullptr) {
    aClassBox->SetIndexZ(mClassBoxes.size());
    mClassBoxes.push_back(aClassBox);
    Notify();
  }
  return aClassBox;
}


bool ggClassyClassBoxContainer::Find(const tClassBoxes& aClassBoxes,
                                     const ggClassyClassBox* aClassBox) const
{
  return aClassBoxes.find(aClassBox) != aClassBoxes.end();
}


ggClassyClassBoxContainer::iterator ggClassyClassBoxContainer::begin()
{
  return mClassBoxes.begin();
}


ggClassyClassBoxContainer::iterator ggClassyClassBoxContainer::end()
{
  return mClassBoxes.end();
}


ggClassyClassBoxContainer::const_iterator ggClassyClassBoxContainer::begin() const
{
  return mClassBoxes.begin();
}


void ggClassyClassBoxContainer::MoveClassBoxesUp(const tClassBoxes& aClassBoxes)
{
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo";
}


void ggClassyClassBoxContainer::MoveClassBoxesDown(const tClassBoxes& aClassBoxes)
{
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo";
}


void ggClassyClassBoxContainer::MoveClassBoxesTop(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  std::vector<ggClassyClassBox*> vClassBoxesA;
  std::vector<ggClassyClassBox*> vClassBoxesB;
  ggWalkerT<std::vector<ggClassyClassBox*>::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (Find(aClassBoxes, vClassBox)) vClassBoxesA.push_back(vClassBox);
    else vClassBoxesB.push_back(vClassBox);
  }
  mClassBoxes.clear();
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesB.begin(), vClassBoxesB.end());
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesA.begin(), vClassBoxesA.end());
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo: adjust mIndexZ";
  Notify();
}


void ggClassyClassBoxContainer::MoveClassBoxesBottom(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  std::vector<ggClassyClassBox*> vClassBoxesA;
  std::vector<ggClassyClassBox*> vClassBoxesB;
  ggWalkerT<std::vector<ggClassyClassBox*>::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (Find(aClassBoxes, vClassBox)) vClassBoxesA.push_back(vClassBox);
    else vClassBoxesB.push_back(vClassBox);
  }
  mClassBoxes.clear();
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesA.begin(), vClassBoxesA.end());
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesB.begin(), vClassBoxesB.end());
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo: adjust mIndexZ";
  Notify();
}
