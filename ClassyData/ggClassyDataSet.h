#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include "ClassyData/ggClassyClassContainer.h"
#include "ClassyData/ggClassyCollectionContainer.h"
#include "ClassyData/ggClassyClassBox.h"
#include "ClassyData/ggClassyFrame.h"

class ggClassyDataSet :
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

  static ggClassyDataSet* GenerateTestData();

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;

};

#endif // GGCLASSYDATASET_H
