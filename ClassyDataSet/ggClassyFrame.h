#ifndef GGCLASSYFRAME_H
#define GGCLASSYFRAME_H

// 1) include system or QT
#include <QString>
#include <QRectF>
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"
#include "ClassyDataSet/ggClassyDescription.h"

// 3) forward declarations
class ggClassyDataSet;
class ggClassyCollection;

/**
 * @brief The ggClassyFrame class
 */
class ggClassyFrame :
  public ggClassyItem,
  public ggSubject
{
public:

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const override;

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyFrame* Create(const QDomElement& aElement);

  const QString& GetDescription() const;
  void SetDescription(const QString& aDescription);
  const QString& GetCollectionName() const;
  void SetCollectionName(const QString& aName);
  ggClassyDataSet* GetDataSet();
  const ggClassyDataSet* GetDataSet() const;
  void SetDataSet(ggClassyDataSet* aDataSet);
  const ggClassyCollection* GetCollection() const;
  int GetIndexZ() const;
  void SetIndexZ(int aIndexZ);
  const QRectF& GetRect() const;
  void SetRect(const QRectF& aRect);
  Qt::Alignment GetAlignment() const;
  void SetAlignment(Qt::Alignment aAlignment);

private:

  ggClassyDescription mDescription;
  Qt::Alignment mAlignment;
  QString mCollectionName;
  QRectF mRect;
  int mIndexZ;

  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYFRAME_H
