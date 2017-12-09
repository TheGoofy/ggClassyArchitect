#include "ClassyData/ggClassyDataSet.h"

#include "Base/ggWalkerT.h"


ggClassyDataSet::ggClassyDataSet() :
  mFormatVersionMajor(0),
  mFormatVersionMinor(0),
  mFormatVersionPatch(1)
{
}


const QString& ggClassyDataSet::TypeID()
{
  static const QString vTypeID("ggClassyDataSet");
  return vTypeID;
}


const QString& ggClassyDataSet::VTypeID() const
{
  return TypeID();
}


QString ggClassyDataSet::GetFormatVersion() const
{
  return QString("%1.%2.%3").arg(mFormatVersionMajor).arg(mFormatVersionMinor).arg(mFormatVersionPatch);
}


QDomElement ggClassyDataSet::CreateDomElement(QDomDocument& aDocument) const
{
  QDomElement vElement = aDocument.createElement(TypeID());
  vElement.setAttribute("mFormatVersion", GetFormatVersion());

  // classes
  ggWalkerT<ggClassyClassContainer::const_iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    vElement.appendChild((*vClassesWalker)->CreateDomElement(aDocument));
  }

  // class boxes
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::const_iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    vElement.appendChild((*vClassBoxesWalker)->CreateDomElement(aDocument));
  }

  return vElement;
}


ggClassyDataSet* ggClassyDataSet::GenerateTestData()
{
  ggClassyClass* vClassA = new ggClassyClass("ggClassA");
  vClassA->mMembers.push_back(ggClassyClassMember("Ping()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("Pong()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("GetB1()", "ggClassB"));
  vClassA->mMembers.push_back(ggClassyClassMember("GetB2()", "ggClassB"));
  vClassA->mDescription = "The answer to life the universe and everything.";

  ggClassyClass* vClassB = new ggClassyClass("ggClassB");
  vClassB->mBaseClassNames.insert("ggClassA");
  vClassB->mMembers.push_back(ggClassyClassMember("Blubb()", "QString"));
  vClassB->mMembers.push_back(ggClassyClassMember("GetA()", "ggClassA"));
  vClassB->mDescription = "One fish, two fish, red fish, blue fish.";

  ggClassyClass* vClassC = new ggClassyClass("ggClassC");
  vClassC->mBaseClassNames.insert("ggClassA");
  vClassC->mBaseClassNames.insert("ggClassB");
  vClassC->mMembers.push_back(ggClassyClassMember("GetName()", "QString"));
  vClassC->mMembers.push_back(ggClassyClassMember("SayHello()", "QString"));
  vClassC->mDescription = "This is a very useful description ot the class!";

  ggClassyClassBox* vClassBoxA1 = new ggClassyClassBox(vClassA->mName);
  vClassBoxA1->mPosition = QPointF(-300, -300);

  ggClassyClassBox* vClassBoxA2 = new ggClassyClassBox(vClassA->mName);
  vClassBoxA2->mPosition = QPointF(100, -300);

  ggClassyClassBox* vClassBoxB1 = new ggClassyClassBox(vClassB->mName);
  vClassBoxB1->mPosition = QPointF(-100, -100);

  ggClassyClassBox* vClassBoxB2 = new ggClassyClassBox(vClassB->mName);
  vClassBoxB2->mPosition = QPointF(300, -100);

  ggClassyClassBox* vClassBoxC1 = new ggClassyClassBox(vClassC->mName);
  vClassBoxC1->mPosition = QPointF(-250, 0);
  vClassBoxC1->mWidth = 250.0f;

  ggClassyClassBox* vClassBoxC2 = new ggClassyClassBox(vClassC->mName);
  vClassBoxC2->mPosition = QPointF(150, 0);
  vClassBoxC2->mWidth = 250.0f;

  ggClassyDataSet* vDataSet = new ggClassyDataSet();
  vDataSet->mClasses.insert(vClassA);
  vDataSet->mClasses.insert(vClassB);
  vDataSet->mClasses.insert(vClassC);
  vDataSet->mClassBoxes.push_back(vClassBoxA1);
  vDataSet->mClassBoxes.push_back(vClassBoxA2);
  vDataSet->mClassBoxes.push_back(vClassBoxB1);
  vDataSet->mClassBoxes.push_back(vClassBoxB2);
  vDataSet->mClassBoxes.push_back(vClassBoxC1);
  vDataSet->mClassBoxes.push_back(vClassBoxC2);

  return vDataSet;
}
