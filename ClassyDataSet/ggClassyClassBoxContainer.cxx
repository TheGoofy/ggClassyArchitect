// 0) include own header
#include "ggClassyClassBoxContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "Base/ggTypes.h"
#include "Base/ggUtility.h"


ggClassyClassBoxContainer::ggClassyClassBoxContainer()
{
}


ggClassyClassBoxContainer::ggClassyClassBoxContainer(const ggClassyClassBoxContainer& aOther)
{
  // copy contents from other
  *this = aOther;
}



ggClassyClassBoxContainer::~ggClassyClassBoxContainer()
{
  DeleteAllClassBoxes();
}


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


ggClassyClassBoxContainer& ggClassyClassBoxContainer::operator = (const ggClassyClassBoxContainer& aOther)
{
  // one collective notification (and not each individual class)
  ggSubject::cExecutorLazy vLazy(this);

  // delete all classes
  DeleteAllClassBoxes();

  // add (and notify) copies
  ggWalkerT<ggClassyClassBoxContainer::const_iterator> vOtherClassBoxesWalker(aOther.mClassBoxes);
  while (vOtherClassBoxesWalker) {
    AddClassBox(new ggClassyClassBox(**vOtherClassBoxesWalker));
  }

  return *this;
}


void ggClassyClassBoxContainer::DeleteClassBox(const ggClassyClassBox* aClassBox)
{
  // don't like nullptr
  if (aClassBox == nullptr) return;

  // if index-z was properly updated, it's simple
  ggUSize vIndex = (ggUSize)aClassBox->GetIndexZ();
  if (vIndex < mClassBoxes.size() && mClassBoxes[vIndex] == aClassBox) {
    mClassBoxes.erase(mClassBoxes.begin() + vIndex);
    delete aClassBox;
    Notify();
    return;
  }

  // search the class
  ggClassyClassBoxContainer::iterator vClassBoxesIterator = mClassBoxes.begin();
  while (vClassBoxesIterator != mClassBoxes.end()) {
    ggClassyClassBox* vClassBox = *vClassBoxesIterator;
    if (vClassBox == aClassBox) {
      mClassBoxes.erase(vClassBoxesIterator);
      delete vClassBox;
      Notify();
      return;
    }
    ++vClassBoxesIterator;
  }
}


void ggClassyClassBoxContainer::DeleteClassBoxes(const QString& aClassName)
{
  // make a copy of all boxes
  std::vector<ggClassyClassBox*> vClassBoxes = mClassBoxes;
  mClassBoxes.clear();

  // re-add the class-boxes, unless a class-box needs to be deleted
  ggWalkerT<std::vector<ggClassyClassBox*>::iterator> vClassBoxesWalker(vClassBoxes);
  while (vClassBoxesWalker) {
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (vClassBox->GetClassName() != aClassName) {
      vClassBox->SetIndexZ(mClassBoxes.size());
      mClassBoxes.push_back(vClassBox);
    }
    else {
      delete vClassBox;
    }
  }

  // maybe there was nothing deleted? notify only, if there was a change
  if (mClassBoxes.size() != vClassBoxes.size()) Notify();
}


void ggClassyClassBoxContainer::DeleteAllClassBoxes()
{
  ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    delete *vClassBoxesWalker;
  }
  mClassBoxes.clear();
  Notify();
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


void ggClassyClassBoxContainer::UpdateIndicesZ()
{
  for (ggUSize vBoxIndex = 0; vBoxIndex < mClassBoxes.size(); vBoxIndex++) {
    mClassBoxes[vBoxIndex]->SetIndexZ(vBoxIndex);
  }
}


void ggClassyClassBoxContainer::MoveClassBoxesUp(const tClassBoxes& aClassBoxes)
{
  ggUtility::MoveUp(mClassBoxes, aClassBoxes);
  UpdateIndicesZ();
  Notify();
}


void ggClassyClassBoxContainer::MoveClassBoxesDown(const tClassBoxes& aClassBoxes)
{
  ggUtility::MoveDown(mClassBoxes, aClassBoxes);
  UpdateIndicesZ();
  Notify();
}


void ggClassyClassBoxContainer::MoveClassBoxesTop(const tClassBoxes& aClassBoxes)
{
  ggUtility::MoveTop(mClassBoxes, aClassBoxes);
  UpdateIndicesZ();
  Notify();
}


void ggClassyClassBoxContainer::MoveClassBoxesBottom(const tClassBoxes& aClassBoxes)
{
  ggUtility::MoveBottom(mClassBoxes, aClassBoxes);
  UpdateIndicesZ();
  Notify();
}
