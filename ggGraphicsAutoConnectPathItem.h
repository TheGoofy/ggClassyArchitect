#ifndef GGGRAPHICSDECORATEDSHORTESTPATHITEM_H
#define GGGRAPHICSDECORATEDSHORTESTPATHITEM_H

#include "ggGraphicsDecoratedPathItem.h"
#include "ggObserver.h"

class ggSubjectConnectionPoint;

class ggGraphicsAutoConnectPathItem :
  public ggGraphicsDecoratedPathItem,
  private ggObserver
{
public:

  ggGraphicsAutoConnectPathItem(QGraphicsItem* aParent = nullptr);

  void SetConnection(const ggSubjectConnectionPoint* aPointSrc,
                     const ggSubjectConnectionPoint* aPointDst);

  void ClearPointsSrc();
  void ClearPointsDst();
  void InsertPointSrc(const ggSubjectConnectionPoint* aPointSrc);
  void InsertPointDst(const ggSubjectConnectionPoint* aPointDst);

  void SetDecorationSrc(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true);

  void SetDecorationDst(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        bool aSolid = true);

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  typedef std::set<const ggSubjectConnectionPoint*> tPoints;

  void Attach(const tPoints& aPoints);
  void Detach(const tPoints& aPoints);

  float GetDistanceCost(const ggConnectionPoint& aPointSrc,
                        const ggConnectionPoint& aPointDst) const;

  void RebuildPath();

  // the connection points
  tPoints mPointsSrc;
  tPoints mPointsDst;

};

#endif // GGGRAPHICSDECORATEDSHORTESTPATHITEM_H
