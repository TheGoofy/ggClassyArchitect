#ifndef GGCLASSYFRAME_H
#define GGCLASSYFRAME_H

#include <QString>
#include <QRectF>

#include "Base/ggSubject.h"

class ggClassyFrame :
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QString mText;
  Qt::Alignment mTextAlignment;
  QString mCollectionName;
  QRectF mRect;

};

#endif // GGCLASSYFRAME_H
