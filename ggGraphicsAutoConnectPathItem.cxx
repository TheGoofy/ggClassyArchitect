#include "ggGraphicsAutoConnectPathItem.h"

#include "ggSubjectConnectionPoint.h"
#include "ggWalkerT.h"


ggGraphicsAutoConnectPathItem::ggGraphicsAutoConnectPathItem(QGraphicsItem* aParent) :
  ggGraphicsDecoratedPathItem(aParent)
{
}


void ggGraphicsAutoConnectPathItem::SetConnection(const ggSubjectConnectionPoint* aPointSrc,
                                                  const ggSubjectConnectionPoint* aPointDst)
{
  ClearPointsSrc();
  ClearPointsDst();
  InsertPointSrc(aPointSrc);
  InsertPointDst(aPointDst);
}


void ggGraphicsAutoConnectPathItem::ClearPointsSrc()
{
  Detach(mPointsSrc);
  mPointsSrc.clear();
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::ClearPointsDst()
{
  Detach(mPointsDst);
  mPointsDst.clear();
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::InsertPointSrc(const ggSubjectConnectionPoint* aPointSrc)
{
  mPointsSrc.insert(aPointSrc);
  ggObserver::Attach(aPointSrc);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::InsertPointDst(const ggSubjectConnectionPoint* aPointDst)
{
  mPointsDst.insert(aPointDst);
  ggObserver::Attach(aPointDst);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::SetDecorationSrc(ggDecoration::cType aType,
                                                     float aLength,
                                                     bool aSolid)
{
  ggGraphicsDecoratedPathItem::SetDecorationSrc(aType, aLength, aSolid);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::SetDecorationDst(ggDecoration::cType aType,
                                                     float aLength,
                                                     bool aSolid)
{
  ggGraphicsDecoratedPathItem::SetDecorationDst(aType, aLength, aSolid);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::Update(const ggSubject* aSubject)
{
  const ggSubjectConnectionPoint* vPoint = dynamic_cast<const ggSubjectConnectionPoint*>(aSubject);
  if (vPoint != nullptr) {
    if (mPointsSrc.find(vPoint) != mPointsSrc.end() ||
        mPointsDst.find(vPoint) != mPointsDst.end()) {
      RebuildPath();
    }
  }
}


void ggGraphicsAutoConnectPathItem::Attach(const tPoints& aPoints)
{
  ggWalkerT<tPoints::const_iterator> vPointsWalker(aPoints);
  while (vPointsWalker) ggObserver::Attach(*vPointsWalker);
}


void ggGraphicsAutoConnectPathItem::Detach(const tPoints& aPoints)
{
  ggWalkerT<tPoints::const_iterator> vPointsWalker(aPoints);
  while (vPointsWalker) ggObserver::Detach(*vPointsWalker);
}


float ggGraphicsAutoConnectPathItem::GetDistanceCost(const ggConnectionPoint& aPointSrc,
                                                     const ggConnectionPoint& aPointDst) const
{
  QPointF vPointSrcBase = aPointSrc.GetControlPoint(DecorationSrc().GetLength());
  QPointF vPointDstBase = aPointDst.GetControlPoint(DecorationDst().GetLength());
  float vDistance = QLineF(vPointSrcBase, vPointDstBase).length();
  QPointF vPointSrcControl = aPointSrc.GetControlPoint(DecorationSrc().GetLength() + vDistance/4.0f);
  QPointF vPointDstControl = aPointDst.GetControlPoint(DecorationDst().GetLength() + vDistance/4.0f);
  return QLineF(vPointSrcControl, vPointDstControl).length();
}


void ggGraphicsAutoConnectPathItem::RebuildPath()
{
  // connection points with the shortest distance
  float vDistanceCostsMin = std::numeric_limits<float>::max();
  const ggConnectionPoint* vPointSrc = nullptr;
  const ggConnectionPoint* vPointDst = nullptr;

  // loop over all visible src- and dst-points
  ggWalkerT<tPoints::const_iterator> vPointsSrcWalker(mPointsSrc);
  while (vPointsSrcWalker) {

    if ((**vPointsSrcWalker).GetVisible()) {
      ggWalkerT<tPoints::const_iterator> vPointsDstWalker(mPointsDst);
      while (vPointsDstWalker) {

        if ((**vPointsDstWalker).GetVisible()) {
          float vDistanceCosts = GetDistanceCost(**vPointsSrcWalker, **vPointsDstWalker);
          if (vDistanceCosts < vDistanceCostsMin) {
            vDistanceCostsMin = vDistanceCosts;
            vPointSrc = *vPointsSrcWalker;
            vPointDst = *vPointsDstWalker;
          }
        }
      }
    }
  }

  // update path
  ClearConnection();
  if (vPointSrc != nullptr &&
      vPointDst != nullptr) {
    ggGraphicsDecoratedPathItem::SetConnection(*vPointSrc, *vPointDst);
  }
}
