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
  // only add, if name is unique
  if (mClasses.find(aClass) == mClasses.end()) {
    mClasses.insert(aClass);
    aClass->mDataSet = this;
    mClasses.Notify();
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

  // if the name is unchanged, renaming is obsolete (success)
  if (aNewClassName == aOldClassName) return true;
  // can't rename, if there is no class with the "old" name (fail)
  if (mClasses.FindClass(aOldClassName) == nullptr) return false;
  // can't rename, if there is another class with the "new" name (fail)
  if (mClasses.FindClass(aNewClassName) != nullptr) return false;

  // change base classes and members of all other classes
  ggWalkerT<ggClassyClassContainer::iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    // get the class pointer
    ggClassyClass* vClass = *vClassesWalker;
    // be lazy: collect multiple notifications and only notify once
    ggClassyClass::cExecutorLazy vExecutorLazy(vClass);
    // replace a base class name (if there is)
    if (vClass->mBaseClassNames.erase(aOldClassName)) {
      vClass->mBaseClassNames.insert(aNewClassName);
      vClass->Notify();
    }
    // replace member class name(s)
    ggWalkerT<ggClassyClass::tMembers::iterator> vMembersWalker(vClass->mMembers);
    while (vMembersWalker) {
      ggClassyClassMember& vMember = *vMembersWalker;
      if (vMember.GetClassName() == aOldClassName) {
        vMember.SetClassName(aNewClassName);
        vClass->Notify();
      }
    }
  }

  // change class boxes
  typedef std::vector<ggClassyClassBox*> tClassBoxes;
  ggWalkerT<tClassBoxes::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    // get the class box pointer
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (vClassBox->GetClassName() == aOldClassName) {
      vClassBox->SetClassName(aNewClassName);
    }
  }

  // Since "aOldClassName" is passed as a reference, we have to change the actual
  // class name "vClass->mClassName" at the very last. It (may) happens that "aOldClassName"
  // magically changes to the new class name! ... AND the class can't be renamed, while
  // it is stored in a set, because the name is used as the key.
  ggClassyClass* vClass = mClasses.FindClass(aOldClassName);
  // temporarily remove it from the set
  mClasses.erase(vClass);
  // rename it (unsing friend rights, prevent recursion)
  vClass->mName = aNewClassName;
  // insert it again
  mClasses.insert(vClass);
  // notify the change
  vClass->Notify();

  // re-link classes
  mSubjectConnections.Notify();

  // done (success)
  return true;
}


ggClassyClassBox* ggClassyDataSet::AddClassBox(ggClassyClassBox* aClassBox)
{
  if (aClassBox != nullptr) {
    aClassBox->SetIndexZ(mClassBoxes.size());
    mClassBoxes.push_back(aClassBox);
    mClassBoxes.Notify();
    mSubjectConnections.Notify();
  }
  return aClassBox;
}


bool ggClassyDataSet::Find(const tClassBoxes& aClassBoxes,
                           const ggClassyClassBox* aClassBox) const
{
  return aClassBoxes.find(aClassBox) != aClassBoxes.end();
}


void ggClassyDataSet::MoveClassBoxesUp(const tClassBoxes& aClassBoxes)
{
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo";
}


void ggClassyDataSet::MoveClassBoxesDown(const tClassBoxes& aClassBoxes)
{
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo";
}


void ggClassyDataSet::MoveClassBoxesTop(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  std::vector<ggClassyClassBox*> vClassBoxesA;
  std::vector<ggClassyClassBox*> vClassBoxesB;
  ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (Find(aClassBoxes, vClassBox)) vClassBoxesA.push_back(vClassBox);
    else vClassBoxesB.push_back(vClassBox);
  }
  mClassBoxes.clear();
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesB.begin(), vClassBoxesB.end());
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesA.begin(), vClassBoxesA.end());
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo: adjust mIndexZ";
  mClassBoxes.Notify();
}


void ggClassyDataSet::MoveClassBoxesBottom(const tClassBoxes& aClassBoxes)
{
  if (aClassBoxes.empty()) return;
  std::vector<ggClassyClassBox*> vClassBoxesA;
  std::vector<ggClassyClassBox*> vClassBoxesB;
  ggWalkerT<ggClassyClassBoxContainer::iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    ggClassyClassBox* vClassBox = *vClassBoxesWalker;
    if (Find(aClassBoxes, vClassBox)) vClassBoxesA.push_back(vClassBox);
    else vClassBoxesB.push_back(vClassBox);
  }
  mClassBoxes.clear();
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesA.begin(), vClassBoxesA.end());
  mClassBoxes.insert(mClassBoxes.end(), vClassBoxesB.begin(), vClassBoxesB.end());
  qDebug() << __PRETTY_FUNCTION__ << "goofy todo: adjust mIndexZ";
  mClassBoxes.Notify();
}


const ggClassyClassContainer& ggClassyDataSet::GetClasses() const
{
  return mClasses;
}


const ggClassyClassBoxContainer& ggClassyDataSet::GetClassBoxes() const
{
  return mClassBoxes;
}


ggClassyDataSet* ggClassyDataSet::CreateTestDataSet()
{
  ggClassyDataSet* vDataSet = new ggClassyDataSet();

  ggClassyClass* vClassA = vDataSet->AddClass(new ggClassyClass("ggClassA"));
  vClassA->mMembers.push_back(ggClassyClassMember("Ping()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("Pong()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("GetB1()", "ggClassB"));
  vClassA->mMembers.push_back(ggClassyClassMember("GetB2()", "ggClassB"));
  vClassA->mDescription = "The answer to life the universe and everything.";

  ggClassyClass* vClassB = vDataSet->AddClass(new ggClassyClass("ggClassB"));
  vClassB->mBaseClassNames.insert("ggClassA");
  vClassB->mMembers.push_back(ggClassyClassMember("Blubb()", "QString"));
  vClassB->mMembers.push_back(ggClassyClassMember("GetA()", "ggClassA"));
  vClassB->mDescription = "One fish, two fish, red fish, blue fish.";

  ggClassyClass* vClassC = vDataSet->AddClass(new ggClassyClass("ggClassC"));
  vClassC->mBaseClassNames.insert("ggClassA");
  vClassC->mBaseClassNames.insert("ggClassB");
  vClassC->mMembers.push_back(ggClassyClassMember("GetName()", "QString"));
  vClassC->mMembers.push_back(ggClassyClassMember("SayHello()", "QString"));
  vClassC->mDescription = "This is a very useful description ot the class!";

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
