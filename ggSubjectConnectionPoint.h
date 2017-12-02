#ifndef GGSUBJECTCONNECTIONPOINT_H
#define GGSUBJECTCONNECTIONPOINT_H

#include "ggSubjectValueT.h"
#include "ggConnectionPoint.h"

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
