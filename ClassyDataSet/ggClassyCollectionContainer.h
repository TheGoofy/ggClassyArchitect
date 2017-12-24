#ifndef GGCLASSYCOLLECTIONCONTAINER_H
#define GGCLASSYCOLLECTIONCONTAINER_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyCollection.h"
#include "Base/ggTypes.h"

// 3) forward declarations

/**
 * @brief The ggClassyCollectionContainer class
 */
class ggClassyCollectionContainer :
  public std::set<ggClassyCollection*, ggClassyCollection>,
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

  ggUSize GetSize() const;
  const ggClassyCollection* SearchCollection(ggUSize aIndex) const;

};

#endif // GGCLASSYCOLLECTIONCONTAINER_H
