// 0) include own header
#include "ggClassyFrameContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "Base/ggUtility.h"
#include "ClassyDataSet/ggClassyFrame.h"


ggClassyFrameContainer::ggClassyFrameContainer()
{
}


ggClassyFrameContainer::ggClassyFrameContainer(const ggClassyFrameContainer& aOther)
{
  *this = aOther;
}


ggClassyFrameContainer::~ggClassyFrameContainer()
{
  DeleteAllFrames();
}



const QString& ggClassyFrameContainer::TypeID()
{
  static const QString vTypeID("ggClassyFrameContainer");
  return vTypeID;
}


const QString& ggClassyFrameContainer::VTypeID() const
{
  return TypeID();
}


ggClassyFrameContainer& ggClassyFrameContainer::operator = (const ggClassyFrameContainer& aOther)
{
  // one collective notification (and not each individual frame)
  ggSubject::cExecutorLazy vLazy(this);

  // delete all frames
  DeleteAllFrames();

  // add (and notify) copies
  ggWalkerT<tFrameVector::const_iterator> vOtherFramesWalker(aOther.mFrames);
  while (vOtherFramesWalker) {
    AddFrame(new ggClassyFrame(**vOtherFramesWalker));
  }

  return *this;
}


ggClassyFrame* ggClassyFrameContainer::AddFrame(ggClassyFrame* aFrame)
{
  if (aFrame != nullptr) {
    aFrame->SetIndexZ(mFrames.size());
    mFrames.push_back(aFrame);
    Notify();
  }
  return aFrame;
}


ggClassyFrame* ggClassyFrameContainer::RemoveFrame(const ggClassyFrame* aFrame)
{
  if (aFrame != nullptr) {

    // potentially index-z points to the right location
    const int vIndexZ = aFrame->GetIndexZ();
    if (0 <= vIndexZ && vIndexZ < (int)mFrames.size()) {
      ggClassyFrame* vFrame = mFrames[vIndexZ];
      if (vFrame == aFrame) {
        mFrames.erase(mFrames.begin() + vIndexZ);
        UpdateIndicesZ();
        Notify();
        return vFrame;
      }
    }

    // need to search it
    auto vFramesIterator = mFrames.begin();
    while (vFramesIterator != mFrames.end()) {
      ggClassyFrame* vFrame = *vFramesIterator;
      if (vFrame == aFrame) {
        mFrames.erase(vFramesIterator);
        UpdateIndicesZ();
        Notify();
        return vFrame;
      }
      ++vFramesIterator;
    }
  }

  // didn't find and remove it
  return nullptr;
}


void ggClassyFrameContainer::DeleteFrame(const ggClassyFrame* aFrame)
{
  RemoveFrame(aFrame);
  delete aFrame;
}


void ggClassyFrameContainer::DeleteAllFrames()
{
  ggWalkerT<tFrameVector::iterator> vFramesWalker(mFrames);
  while (vFramesWalker) {
    delete *vFramesWalker;
  }
  mFrames.clear();
  Notify();
}


ggClassyFrameContainer::iterator ggClassyFrameContainer::begin()
{
  return mFrames.begin();
}


ggClassyFrameContainer::iterator ggClassyFrameContainer::end()
{
  return mFrames.end();
}


ggClassyFrameContainer::const_iterator ggClassyFrameContainer::begin() const
{
  return mFrames.begin();
}


ggClassyFrameContainer::const_iterator ggClassyFrameContainer::end() const
{
  return mFrames.end();
}


void ggClassyFrameContainer::UpdateIndicesZ()
{
  for (ggUSize vIndex = 0; vIndex < mFrames.size(); vIndex++) {
    mFrames[vIndex]->SetIndexZ(vIndex);
  }
}


void ggClassyFrameContainer::MoveFramesUp(const tFrameSet& aFrames)
{
  ggUtility::MoveUp(mFrames, aFrames);
  UpdateIndicesZ();
  Notify();
}


void ggClassyFrameContainer::MoveFramesDown(const tFrameSet& aFrames)
{
  ggUtility::MoveDown(mFrames, aFrames);
  UpdateIndicesZ();
  Notify();
}


void ggClassyFrameContainer::MoveFramesTop(const tFrameSet& aFrames)
{
  ggUtility::MoveTop(mFrames, aFrames);
  UpdateIndicesZ();
  Notify();
}


void ggClassyFrameContainer::MoveFramesBottom(const tFrameSet& aFrames)
{
  ggUtility::MoveBottom(mFrames, aFrames);
  UpdateIndicesZ();
  Notify();
}
