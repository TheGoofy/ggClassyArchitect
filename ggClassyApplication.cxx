#include "ggClassyApplication.h"

#include <QDomDocument>
#include <QTextStream>

#include "ggClassyDataSet.h"


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
  QDomDocument vDomDocument;

  QDomElement vRootElement = vDomDocument.createElement("ClassyArchitectApplication");
  vDomDocument.appendChild(vRootElement);

  QDomElement vDataSetElement = vDomDocument.createElement(ggClassyDataSet::TypeID());
  vRootElement.appendChild(vDataSetElement);

  //
  // classes
  //

  vDataSetElement.appendChild(mDataSet->mClasses.CreateDomElement(vDomDocument));

  //
  // class boxes
  //

  QDomElement vClassBoxesElement = vDomDocument.createElement("mClassBoxes");
  vDataSetElement.appendChild(vClassBoxesElement);

  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesWalker(mDataSet->mClassBoxes);
  while (vClassBoxesWalker) {

    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    QDomElement vClassBoxElement = vClassBox->CreateDomElement(vDomDocument);
    vClassBoxesElement.appendChild(vClassBoxElement);

  }

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
