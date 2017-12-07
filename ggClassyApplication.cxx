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

  QDomElement vDataSetElement = vDomDocument.createElement("ggClassyDataSet");
  vRootElement.appendChild(vDataSetElement);

  //
  // classes
  //

  QDomElement vClassesElement = vDomDocument.createElement("ggClassyClassContainer");
  vDataSetElement.appendChild(vClassesElement);

  ggWalkerT<ggClassyClassContainer::iterator> vClassesIterator(mDataSet->mClasses);
  while (vClassesIterator) {

    ggClassyClass* vClass = *vClassesIterator;

    // class name
    QDomElement vClassElement = vDomDocument.createElement("ggClassyClass");
    vClassElement.setAttribute("mClassName", vClass->mClassName);
    vClassesElement.appendChild(vClassElement);

    // base classes
    ggWalkerT<ggStringSet::iterator> vBaseClassNamesIterator(vClass->mBaseClassNames);
    while (vBaseClassNamesIterator) {
      QDomElement vBaseClassElement = vDomDocument.createElement("mBaseClass");
      vBaseClassElement.setAttribute("mClassName", *vBaseClassNamesIterator);
      vClassElement.appendChild(vBaseClassElement);
    }

    // members
    ggWalkerT<ggClassyClass::tMembers::iterator> vMembersWalker(vClass->mMembers);
    while (vMembersWalker) {
      const ggClassyClassMember& vMember = *vMembersWalker;
      QDomElement vClassMemberElement = vDomDocument.createElement("mMember");
      vClassElement.appendChild(vClassMemberElement);
      vClassMemberElement.setAttribute("mClassName", vMember.GetClassName());
      QDomText vMemberNameText = vDomDocument.createTextNode(vMember.GetName());
      vClassMemberElement.appendChild(vMemberNameText);
    }

    // description
    QDomElement vClassDescriptionElement = vDomDocument.createElement("mDescription");
    vClassElement.appendChild(vClassDescriptionElement);
    QDomText vDescriptionText = vDomDocument.createTextNode(vClass->mDescription);
    vClassDescriptionElement.appendChild(vDescriptionText);

  }

  //
  // class boxes
  //

  QDomElement vClassBoxesElement = vDomDocument.createElement("mClassBoxes");
  vDataSetElement.appendChild(vClassBoxesElement);

  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesWalker(mDataSet->mClassBoxes);
  while (vClassBoxesWalker) {

    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    QDomElement vClassBoxElement = vDomDocument.createElement("ggClassyClassBox");
    vClassBoxElement.setAttribute("mClassName", vClassBox->mClassName);
    vClassBoxElement.setAttribute("mPosition.x", vClassBox->mPosition.x());
    vClassBoxElement.setAttribute("mPosition.y", vClassBox->mPosition.y());
    vClassBoxElement.setAttribute("mWidth", vClassBox->mWidth);
    vClassBoxElement.setAttribute("mMembersVisible", vClassBox->mMembersVisible);
    vClassBoxElement.setAttribute("mDescriptionVisible", vClassBox->mDescriptionVisible);
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
