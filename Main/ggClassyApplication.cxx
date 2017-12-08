#include "Main/ggClassyApplication.h"

#include <QDomDocument>
#include <QTextStream>

#include "Data/ggClassyDataSet.h"
#include "Base/ggWalkerT.h"


ggClassyApplication::ggClassyApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  mDataSet(nullptr)
{
  mDataSet = ggClassyDataSet::GenerateTestData();
}


ggClassyApplication::~ggClassyApplication()
{
}


ggClassyDataSet* ggClassyApplication::GetDataSet()
{
  return mDataSet;
}


const ggClassyDataSet* ggClassyApplication::GetDataSet() const
{
  return mDataSet;
}


void ggClassyApplication::SaveDataSet(QIODevice* aDevice)
{
  //
  // compile the DOM document
  //
  QDomDocument vDomDocument;
  QDomElement vRootElement = vDomDocument.createElement("ClassyArchitectApplication");
  vDomDocument.appendChild(vRootElement);
  vRootElement.appendChild(mDataSet->CreateDomElement(vDomDocument));

  //
  // write file
  //
  const int vIndentSize = 2;
  QTextStream vTextStream(aDevice);
  vDomDocument.save(vTextStream, vIndentSize);
}


ggSubjectFloat& ggClassyApplication::Zoom()
{
  return mZoom;
}


const ggSubjectFloat& ggClassyApplication::Zoom() const
{
  return mZoom;
}


ggClassyApplication& ggClassyApplication::GetInstance()
{
  return dynamic_cast<ggClassyApplication&>(*instance());
}
