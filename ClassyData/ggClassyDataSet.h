#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include "Base/ggTypes.h"
#include "ClassyData/ggClassyClassContainer.h"
#include "ClassyData/ggClassyCollectionContainer.h"
#include "ClassyData/ggClassyClassBox.h"
#include "ClassyData/ggClassyFrame.h"

class ggClassyDataSet :
  public ggSubject
{
public:

  ggClassyDataSet();

  static const QString& TypeID();
  virtual const QString& VTypeID() const;
  QString GetFormatVersion() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

  static ggClassyDataSet* GenerateTestData();

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;

private:

  const ggUInt32 mFormatVersionMajor;
  const ggUInt32 mFormatVersionMinor;
  const ggUInt32 mFormatVersionPatch;

};

#endif // GGCLASSYDATASET_H
