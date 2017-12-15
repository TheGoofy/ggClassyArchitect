#ifndef GGCLASSYCOLLECTIONCONTAINER_H
#define GGCLASSYCOLLECTIONCONTAINER_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyCollection.h"

// 3) forward declarations

/**
 * @brief The ggClassyCollectionContainer class
 */
class ggClassyCollectionContainer :
  public std::set<ggClassyCollection*>
{

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

};

#endif // GGCLASSYCOLLECTIONCONTAINER_H
