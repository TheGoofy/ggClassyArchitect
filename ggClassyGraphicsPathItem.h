#ifndef GGCLASSYLINE_H
#define GGCLASSYLINE_H

#include <QGraphicsPathItem>

#include "ggClassyGraphicsBoxItems.h"

class ggClassyGraphicsPathItem :
  public QGraphicsPathItem,
  public ggObserver
{

public:

  ggClassyGraphicsPathItem(QGraphicsItem* aParent = nullptr);

  void SetBoxItems(const ggClassyGraphicsBoxItems* aBoxItems);
  void SetConnection(const QString& aClassNameSrc, const QString& aClassNameDst);
  void SetColor(const QColor& aColor);
  void SetLineWidth(float aWidth);
  void SetArrowLength(float aLength);

protected:

  void Update(const ggSubject* aSubject) override;

private:

  void UpdateBoxItems();
  void UpdatePath();

  typedef ggClassyGraphicsBoxItems::tBoxItemsVec tBoxItemsVec;
  typedef std::set<const ggSubject*> tSubjectsSet;

  void AttachSubjects(const tSubjectsSet& aSubjects);
  void DetachSubjects(const tSubjectsSet& aSubjects);
  void InsertBoxItemSubjects(const tBoxItemsVec& aBoxItems);

  // link to the rest of the world
  const ggClassyGraphicsBoxItems* mBoxItems;

  // properties
  QString mClassNameSrc;
  QString mClassNameDst;
  float mArrowLength;
  float mLineAnchorLength;

  // caching ...
  tBoxItemsVec mBoxItemsSrc;
  tBoxItemsVec mBoxItemsDst;
  tSubjectsSet mBoxItemSubjects;

};

#endif // GGCLASSYLINE_H
