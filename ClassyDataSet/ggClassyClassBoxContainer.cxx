// 0) include own header
#include "ggClassyClassBoxContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "Base/ggTypes.h"
#include "Base/ggUtility.h"


const QString& ggClassyClassBoxContainer::TypeID()
{
  static QString vTypeID("ggClassyClassBoxContainer");
  return vTypeID;
}


const QString& ggClassyClassBoxContainer::VTypeID() const
{
  return TypeID();
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


ggClassyClassBoxContainer::const_iterator ggClassyClassBoxContainer::end() const
{
  return mClassBoxes.end();
}


bool ggClassyClassBoxContainer::Find(const tClassBoxes& aClassBoxes,
                                     const ggClassyClassBox* aClassBox) const
{
  return aClassBoxes.find(aClassBox) != aClassBoxes.end();
}


void ggClassyClassBoxContainer::UpdateIndicesZ()
{
  for (ggUSize vBoxIndex = 0; vBoxIndex < mClassBoxes.size(); vBoxIndex++) {
    mClassBoxes[vBoxIndex]->SetIndexZ(vBoxIndex);
  }
}


void ggClassyClassBoxContainer::MoveClassBoxesUp(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  if (mClassBoxes.size() < 2) return;
  for (ggUSize vBoxIndex = mClassBoxes.size()-1; vBoxIndex > 0; vBoxIndex--) {
    if (Find(aClassBoxes, mClassBoxes[vBoxIndex-1])) {
      if (!Find(aClassBoxes, mClassBoxes[vBoxIndex])) {
        ggUtility::Swap(mClassBoxes[vBoxIndex-1], mClassBoxes[vBoxIndex]);
      }
    }
  }
  UpdateIndicesZ();
  Notify();
}


void ggClassyClassBoxContainer::MoveClassBoxesDown(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  if (mClassBoxes.size() < 2) return;
  for (ggUSize vBoxIndex = 0; vBoxIndex+1 < mClassBoxes.size(); vBoxIndex++) {
    if (Find(aClassBoxes, mClassBoxes[vBoxIndex+1])) {
      if (!Find(aClassBoxes, mClassBoxes[vBoxIndex])) {
        ggUtility::Swap(mClassBoxes[vBoxIndex+1], mClassBoxes[vBoxIndex]);
      }
    }
  }
  UpdateIndicesZ();
  Notify();
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
  UpdateIndicesZ();
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
  UpdateIndicesZ();
  Notify();
}
