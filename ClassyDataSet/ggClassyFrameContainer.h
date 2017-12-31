#ifndef GGCLASSYFRAMECONTAINER_H
#define GGCLASSYFRAMECONTAINER_H

// 1) include system or QT
#include <vector>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations
class ggClassyFrame;

class ggClassyFrameContainer :
  public ggSubject
{
public:

  ggClassyFrameContainer();
  ggClassyFrameContainer(const ggClassyFrameContainer& aOther);
  virtual ~ggClassyFrameContainer();

  ggClassyFrameContainer& operator = (const ggClassyFrameContainer& aOther);

  ggClassyFrame* AddFrame(ggClassyFrame* aFrame);
  void DeleteFrame(const ggClassyFrame* aFrame);
  void DeleteAllFrames();

  typedef std::vector<ggClassyFrame*>::iterator iterator;
  iterator begin();
  iterator end();

  typedef std::vector<const ggClassyFrame*>::const_iterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;

private:

  typedef std::vector<ggClassyFrame*> tFrames;
  tFrames mFrames;

};

#endif // GGCLASSYFRAMECONTAINER_H
