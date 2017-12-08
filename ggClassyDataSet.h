#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include "ggClassyClassContainer.h"
#include "ggClassyCollectionContainer.h"
#include "ggClassyClassBox.h"
#include "ggClassyFrame.h"

class ggClassyDataSet :
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  static ggClassyDataSet* GenerateTestData();

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;

};

#endif // GGCLASSYDATASET_H
