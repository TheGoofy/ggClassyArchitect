#ifndef GGCLASSYCOLLECTIONCONTAINER_H
#define GGCLASSYCOLLECTIONCONTAINER_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "Base/ggVectorSetT.h"
#include "ClassyDataSet/ggClassyCollection.h"

// 3) forward declarations

/**
 * @brief The ggClassyCollectionContainer class
 */
class ggClassyCollectionContainer :
  public ggSubject
{
public:

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  ggClassyCollection* AddCollection(ggClassyCollection* aCollection);
  ggClassyCollection* RemoveCollection(const QString& aName);
  ggClassyCollection* FindCollection(const QString& aName);
  const ggClassyCollection* FindCollection(const QString& aName) const;
  const ggClassyCollection* GetCollection(ggUSize aIndex) const;
  void DeleteAllCollections();
  ggUSize GetSize() const;

private:

  typedef ggVectorSetT<ggClassyCollection*, ggClassyCollection> tCollections;
  tCollections mCollections;

};

#endif // GGCLASSYCOLLECTIONCONTAINER_H
