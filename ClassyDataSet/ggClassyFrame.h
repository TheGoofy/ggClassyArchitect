#ifndef GGCLASSYFRAME_H
#define GGCLASSYFRAME_H

// 1) include system or QT
#include <QString>
#include <QRectF>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

/**
 * @brief The ggClassyFrame class
 */
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
