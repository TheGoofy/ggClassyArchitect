#ifndef GGCLASSYCOLLECTIONCONTAINER_H
#define GGCLASSYCOLLECTIONCONTAINER_H

#include "ClassyDataSet/ggClassyCollection.h"

class ggClassyCollectionContainer :
  public std::set<ggClassyCollection*>
{

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

};

#endif // GGCLASSYCOLLECTIONCONTAINER_H