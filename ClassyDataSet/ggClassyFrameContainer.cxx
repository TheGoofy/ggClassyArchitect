// 0) include own header
#include "ggClassyFrameContainer.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
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


ggClassyFrameContainer& ggClassyFrameContainer::operator = (const ggClassyFrameContainer& aOther)
{
  // one collective notification (and not each individual frame)
  ggSubject::cExecutorLazy vLazy(this);

  // delete all frames
  DeleteAllFrames();

  // add (and notify) copies
  ggWalkerT<tFrames::const_iterator> vOtherFramesWalker(aOther.mFrames);
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


void ggClassyFrameContainer::DeleteFrame(const ggClassyFrame* aFrame)
{
  if (aFrame != nullptr) {
    // goofy todo
  }
}


void ggClassyFrameContainer::DeleteAllFrames()
{
  ggWalkerT<tFrames::iterator> vFramesWalker(mFrames);
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
