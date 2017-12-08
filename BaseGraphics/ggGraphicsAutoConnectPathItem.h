#ifndef GGGRAPHICSDECORATEDSHORTESTPATHITEM_H
#define GGGRAPHICSDECORATEDSHORTESTPATHITEM_H

#include "BaseGraphics/ggGraphicsDecoratedPathItem.h"
#include "Base/ggObserver.h"

class ggSubjectConnectionPoint;

class ggGraphicsAutoConnectPathItem :
  public ggGraphicsDecoratedPathItem,
  private ggObserver
{
public:

  ggGraphicsAutoConnectPathItem(QGraphicsItem* aParent = nullptr);

  typedef std::set<const ggSubjectConnectionPoint*> tPointSet;

  void SetConnection(const ggSubjectConnectionPoint* aPointSrc,
                     const ggSubjectConnectionPoint* aPointDst);

  void ClearPointsSrc();
  void ClearPointsDst();
  void InsertPointSrc(const ggSubjectConnectionPoint* aPoint);
  void InsertPointDst(const ggSubjectConnectionPoint* aPoint);
  void InsertPointsSrc(const tPointSet& aPoints);
  void InsertPointsDst(const tPointSet& aPoints);

  void SetDecorationSrc(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        ggDecoration::cFill aFill = ggDecoration::cFill::eSolid);

  void SetDecorationDst(ggDecoration::cType aType,
                        float aLength = 12.0f,
                        ggDecoration::cFill aFill = ggDecoration::cFill::eSolid);

protected:

  virtual void Update(const ggSubject* aSubject) override;

private:

  void Attach(const tPointSet& aPoints);
  void Detach(const tPointSet& aPoints);

  float GetDistanceCost(const ggConnectionPoint& aPointSrc,
                        const ggConnectionPoint& aPointDst) const;

  void RebuildPath();

  // the connection points
  tPointSet mPointsSrc;
  tPointSet mPointsDst;

};

#endif // GGGRAPHICSDECORATEDSHORTESTPATHITEM_H
