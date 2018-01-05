#ifndef GGCLASSYCOLLECTIONCONTAINER_H
#define GGCLASSYCOLLECTIONCONTAINER_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "Base/ggVectorSetT.h"
#include "ClassyDataSet/ggClassyItem.h"
#include "ClassyDataSet/ggClassyCollection.h"

// 3) forward declarations

/**
 * @brief The ggClassyCollectionContainer class
 */
class ggClassyCollectionContainer :
  public ggClassyItem,
  public ggSubject
{
public:

  ggClassyCollectionContainer();
  ggClassyCollectionContainer(const ggClassyCollectionContainer& aOther);

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const override;

  ggClassyCollectionContainer& operator = (const ggClassyCollectionContainer& aOther);

  ggClassyCollection* AddCollection(ggClassyCollection* aCollection);
  ggClassyCollection* RemoveCollection(const QString& aName);
  ggClassyCollection* FindCollection(const QString& aName);
  const ggClassyCollection* FindCollection(const QString& aName) const;
  ggClassyCollection* GetCollection(ggUSize aIndex);
  const ggClassyCollection* GetCollection(ggUSize aIndex) const;
  void DeleteCollection(const QString& aName);
  void DeleteAllCollections();
  ggUSize GetSize() const;

private:

  // compares two pointers by comparing their "mName"
  struct cLess {
    bool operator() (const ggClassyCollection* aCollectionA,
                     const ggClassyCollection* aCollectionB) const;
  };

  typedef ggVectorSetT<ggClassyCollection*, cLess> tCollections;
  tCollections mCollections;

};

#endif // GGCLASSYCOLLECTIONCONTAINER_H
