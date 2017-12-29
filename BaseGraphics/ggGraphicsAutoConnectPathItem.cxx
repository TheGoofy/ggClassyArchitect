// 0) include own header
#include "ggGraphicsAutoConnectPathItem.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseGraphics/ggSubjectConnectionPoint.h"


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


void ggGraphicsAutoConnectPathItem::InsertPointSrc(const ggSubjectConnectionPoint* aPoint)
{
  if (aPoint != nullptr) {
    mPointsSrc.insert(aPoint);
    ggObserver::Attach(aPoint);
    RebuildPath();
  }
}


void ggGraphicsAutoConnectPathItem::InsertPointDst(const ggSubjectConnectionPoint* aPoint)
{
  if (aPoint != nullptr) {
    mPointsDst.insert(aPoint);
    ggObserver::Attach(aPoint);
    RebuildPath();
  }
}


void ggGraphicsAutoConnectPathItem::InsertPointsSrc(const tPointSet& aPoints)
{
  mPointsSrc.insert(aPoints.begin(), aPoints.end());
  Attach(aPoints);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::InsertPointsDst(const tPointSet& aPoints)
{
  mPointsDst.insert(aPoints.begin(), aPoints.end());
  Attach(aPoints);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::SetDecorationSrc(ggDecoration::cType aType,
                                                     float aLength,
                                                     ggDecoration::cFill aFill)
{
  ggGraphicsDecoratedPathItem::SetDecorationSrc(aType, aLength, aFill);
  RebuildPath();
}


void ggGraphicsAutoConnectPathItem::SetDecorationDst(ggDecoration::cType aType,
                                                     float aLength,
                                                     ggDecoration::cFill aFill)
{
  ggGraphicsDecoratedPathItem::SetDecorationDst(aType, aLength, aFill);
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


void ggGraphicsAutoConnectPathItem::Attach(const tPointSet& aPoints)
{
  ggWalkerT<tPointSet::const_iterator> vPointsWalker(aPoints);
  while (vPointsWalker) ggObserver::Attach(*vPointsWalker);
}


void ggGraphicsAutoConnectPathItem::Detach(const tPointSet& aPoints)
{
  ggWalkerT<tPointSet::const_iterator> vPointsWalker(aPoints);
  while (vPointsWalker) ggObserver::Detach(*vPointsWalker);
}


float ggGraphicsAutoConnectPathItem::GetDistanceCost(const ggConnectionPoint& aPointSrc,
                                                     const ggConnectionPoint& aPointDst) const
{
  // distance between base points
  QPointF vPointSrcBase = aPointSrc.GetControlPoint(DecorationSrc().GetLength());
  QPointF vPointDstBase = aPointDst.GetControlPoint(DecorationDst().GetLength());
  float vBaseDistance = QLineF(vPointSrcBase, vPointDstBase).length();

  // distance between control points (depends on connection direction)
  QPointF vPointSrcControl = aPointSrc.GetControlPoint(DecorationSrc().GetLength() + vBaseDistance/4.0f);
  QPointF vPointDstControl = aPointDst.GetControlPoint(DecorationDst().GetLength() + vBaseDistance/4.0f);
  float vControlDistance = QLineF(vPointSrcControl, vPointDstControl).length();

  // distance along direction (negative direction means that one point is behind the other point)
  QVector2D vDirectionSrcToDst(vPointDstBase - vPointSrcBase);
  QVector2D vDirectionDstToSrc(vPointSrcBase - vPointDstBase);
  float vDistanceDst = QVector2D::dotProduct(vDirectionSrcToDst, aPointSrc.GetDirection());
  float vDistanceSrc = QVector2D::dotProduct(vDirectionDstToSrc, aPointDst.GetDirection());

  // conditions for low costs:
  // - control points are close to each other
  // - points do not point away from each other (very bad)
  float vCost = vControlDistance;
  if (vDistanceSrc < 0.0f) vCost += -5.0f * vDistanceSrc;
  if (vDistanceDst < 0.0f) vCost += -5.0f * vDistanceDst;

  return vCost;
}


void ggGraphicsAutoConnectPathItem::RebuildPath()
{
  // connection points with the shortest distance
  float vDistanceCostsMin = std::numeric_limits<float>::max();
  const ggConnectionPoint* vPointSrc = nullptr;
  const ggConnectionPoint* vPointDst = nullptr;

  // loop over all visible src- and dst-points
  ggWalkerT<tPointSet::const_iterator> vPointsSrcWalker(mPointsSrc);
  while (vPointsSrcWalker) {

    if ((**vPointsSrcWalker).GetVisible()) {
      ggWalkerT<tPointSet::const_iterator> vPointsDstWalker(mPointsDst);
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
