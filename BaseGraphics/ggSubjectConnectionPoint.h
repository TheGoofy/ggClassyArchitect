#ifndef GGSUBJECTCONNECTIONPOINT_H
#define GGSUBJECTCONNECTIONPOINT_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubjectValueT.h"
#include "BaseGraphics/ggConnectionPoint.h"

// 3) forward declarations

/**
 * @brief The ggSubjectConnectionPoint class
 */
class ggSubjectConnectionPoint :
  public ggSubjectValueT<ggConnectionPoint>
{

public:

  ggSubjectConnectionPoint();

  void SetVisible(bool aVisible);
  bool GetVisible() const;

private:

  bool mVisible;

};

#endif // GGSUBJECTCONNECTIONPOINT_H
