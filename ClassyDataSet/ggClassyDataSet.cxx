// 0) include own header
#include "ggClassyDataSet.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
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
  ggWalkerT<ggClassyClassBoxContainer::const_iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    vElement.appendChild((*vClassBoxesWalker)->CreateDomElement(aDocument));
  }

  return vElement;
}


const ggSubject* ggClassyDataSet::GetSubjectConnections() const
{
  return &mSubjectConnections;
}


ggClassyClass* ggClassyDataSet::AddClass(ggClassyClass* aClass)
{
  if (mClasses.AddClass(aClass) != nullptr) {
    aClass->SetDataSet(this);
    mClassBoxes.Notify();
    mSubjectConnections.Notify();
    return aClass;
  }
  return nullptr;
}


ggClassyClass* ggClassyDataSet::FindClass(const QString& aClassName)
{
  return mClasses.FindClass(aClassName);
}


bool ggClassyDataSet::RenameClass(const QString& aOldClassName,
                                  const QString& aNewClassName)
{
  qDebug() << __PRETTY_FUNCTION__ << aOldClassName << "=>" << aNewClassName;

  // Since "aOldClassName" is passed as a reference, we have to becareful: if
  // the name of the "owning" class is changed, it (may) happens that "aOldClassName"
  // magically changes to the new class name! ... make a copy!
  QString vOldClassName(aOldClassName);

  // can't rename, if there is another class with the "new" name (fail)
  // can't rename, if there is no class with the "old" name (fail)
  if (mClasses.RenameClass(vOldClassName, aNewClassName)) {

    // change class boxes too
    ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesWalker(mClassBoxes);
    while (vClassBoxesWalker) {
      // get the class box pointer
      ggClassyClassBox* vClassBox = *vClassBoxesWalker;
      if (vClassBox->GetClassName() == vOldClassName) {
        vClassBox->SetClassName(aNewClassName);
      }
    }

    // re-link classes
    mSubjectConnections.Notify();
    return true;
  }

  // not possible to rename
  return false;
}


ggClassyClassBox* ggClassyDataSet::AddClassBox(ggClassyClassBox* aClassBox)
{
  mClassBoxes.AddClassBox(aClassBox);
  mSubjectConnections.Notify();
  return aClassBox;
}


const ggClassyClassContainer& ggClassyDataSet::GetClasses() const
{
  return mClasses;
}


ggClassyClassBoxContainer& ggClassyDataSet::GetClassBoxes()
{
  return mClassBoxes;
}


const ggClassyClassBoxContainer& ggClassyDataSet::GetClassBoxes() const
{
  return mClassBoxes;
}


ggClassyDataSet* ggClassyDataSet::CreateTestDataSet()
{
  ggClassyDataSet* vDataSet = new ggClassyDataSet();

  ggClassyClass* vClassA = vDataSet->AddClass(new ggClassyClass("ggClassA"));
  vClassA->AddMember("Ping()", "void");
  vClassA->AddMember("Pong()", "void");
  vClassA->AddMember("GetB1()", "ggClassB");
  vClassA->AddMember("GetB2()", "ggClassB");
  vClassA->SetDescription("The answer to life the universe and everything.");

  ggClassyClass* vClassB = vDataSet->AddClass(new ggClassyClass("ggClassB"));
  vClassB->AddBaseClassName("ggClassA");
  vClassB->AddMember("Blubb()", "QString");
  vClassB->AddMember("GetA()", "ggClassA");
  vClassB->SetDescription("One fish, two fish, red fish, blue fish.");

  ggClassyClass* vClassC = vDataSet->AddClass(new ggClassyClass("ggClassC"));
  vClassC->AddBaseClassName("ggClassA");
  vClassC->AddBaseClassName("ggClassB");
  vClassC->AddMember("GetName()", "QString");
  vClassC->AddMember("SayHello()", "QString");
  vClassC->SetDescription("This is a very useful description ot the class!");

  ggClassyClassBox* vClassBoxA1 = new ggClassyClassBox(vClassA->GetName());
  vClassBoxA1->SetPosition(QPointF(-300, -300));

  ggClassyClassBox* vClassBoxA2 = new ggClassyClassBox(vClassA->GetName());
  vClassBoxA2->SetPosition(QPointF(100, -300));

  ggClassyClassBox* vClassBoxB1 = new ggClassyClassBox(vClassB->GetName());
  vClassBoxB1->SetPosition(QPointF(-100, -100));

  ggClassyClassBox* vClassBoxB2 = new ggClassyClassBox(vClassB->GetName());
  vClassBoxB2->SetPosition(QPointF(300, -100));

  ggClassyClassBox* vClassBoxC1 = new ggClassyClassBox(vClassC->GetName());
  vClassBoxC1->SetPosition(QPointF(-250, 0));
  vClassBoxC1->SetWidth(250.0f);

  ggClassyClassBox* vClassBoxC2 = new ggClassyClassBox(vClassC->GetName());
  vClassBoxC2->SetPosition(QPointF(150, 0));
  vClassBoxC2->SetWidth(250.0f);

  vDataSet->AddClassBox(vClassBoxA1);
  vDataSet->AddClassBox(vClassBoxA2);
  vDataSet->AddClassBox(vClassBoxB1);
  vDataSet->AddClassBox(vClassBoxB2);
  vDataSet->AddClassBox(vClassBoxC1);
  vDataSet->AddClassBox(vClassBoxC2);

  return vDataSet;
}
