// 0) include own header
#include "ggClassyDataSet.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


#define GG_FORMAT_VERSION_MAJOR 0
#define GG_FORMAT_VERSION_MINOR 0
#define GG_FORMAT_VERSION_PATCH 1


ggClassyDataSet::ggClassyDataSet() :
  mFormatVersionMajor(GG_FORMAT_VERSION_MAJOR),
  mFormatVersionMinor(GG_FORMAT_VERSION_MINOR),
  mFormatVersionPatch(GG_FORMAT_VERSION_PATCH)
{
}


ggClassyDataSet::ggClassyDataSet(const ggClassyDataSet& aOther):
  mFormatVersionMajor(GG_FORMAT_VERSION_MAJOR),
  mFormatVersionMinor(GG_FORMAT_VERSION_MINOR),
  mFormatVersionPatch(GG_FORMAT_VERSION_PATCH)
{
  *this = aOther;
}


ggClassyDataSet::~ggClassyDataSet()
{
  Clear();
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
  // create a som-element for the dataset
  QDomElement vElement = aDocument.createElement(TypeID());
  vElement.setAttribute("mFormatVersion", GetFormatVersion());

  // add collections as direct children
  for (ggUSize vCollectionIndex = 0; vCollectionIndex < mCollections.GetSize(); vCollectionIndex++) {
    vElement.appendChild(mCollections.GetCollection(vCollectionIndex)->CreateDomElement(aDocument));
  }

  // add classes as direct children
  ggWalkerT<ggClassyClassContainer::const_iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    vElement.appendChild((*vClassesWalker)->CreateDomElement(aDocument));
  }

  // add class boxes as direct children
  ggWalkerT<ggClassyClassBoxContainer::const_iterator> vClassBoxesWalker(mClassBoxes);
  while (vClassBoxesWalker) {
    vElement.appendChild((*vClassBoxesWalker)->CreateDomElement(aDocument));
  }

  return vElement;
}


ggClassyDataSet* ggClassyDataSet::Create(const QDomElement& aElement)
{
  ggClassyDataSet* vDataSet = nullptr;

  // create a dataset, if the dome-element is a dataset
  if (aElement.tagName() == TypeID()) {
    vDataSet = new ggClassyDataSet();

    // loop over children
    QDomElement vChildElement = aElement.firstChildElement();
    while (!vChildElement.isNull()) {

      // try to create and add a class
      ggClassyClass* vClass = ggClassyClass::Create(vChildElement, vDataSet);
      if (vClass != nullptr) vDataSet->AddClass(vClass);

      // try to create and add a class box
      ggClassyClassBox* vClassBox = ggClassyClassBox::Create(vChildElement);
      if (vClassBox != nullptr) vDataSet->AddClassBox(vClassBox);

      // nect child
      vChildElement = vChildElement.nextSiblingElement();
    }
  }

  return vDataSet;
}


ggClassyDataSet& ggClassyDataSet::operator = (const ggClassyDataSet& aOther)
{
  // only notify when all members of ggDataSet are re-assigned
  ggSubject::cExecutorLazy vClassesLazy(&mClasses);
  ggSubject::cExecutorLazy vClassBoxesLazy(&mClassBoxes);

  // delete all members
  Clear();

  // re-assign the members
  mCollections = aOther.mCollections;
  mClasses = aOther.mClasses;
  mClassBoxes = aOther.mClassBoxes;
  mFrames = aOther.mFrames;

  // tell all classes that I'm the "master"
  ggWalkerT<ggClassyClassContainer::iterator> vClassesWalker(mClasses);
  while (vClassesWalker) {
    ggClassyClass* vClass = *vClassesWalker;
    vClass->SetDataSet(this);
  }

  return *this;
}


const ggSubject* ggClassyDataSet::GetSubjectConnections() const
{
  return &mSubjectConnections;
}


ggClassyCollection* ggClassyDataSet::AddCollection(ggClassyCollection* aCollection)
{
  // don't add nothing
  if (aCollection != nullptr) {
    // only need to change something, if the dataset is different
    if (aCollection->GetDataSet() != this) {
      // add collection to own dataset
      if (mCollections.AddCollection(aCollection) == aCollection) {
        // remove it from the other dataset (collection can only be subscribed to one dataset)
        if (aCollection->GetDataSet() != nullptr) {
          aCollection->GetDataSet()->RemoveCollection(aCollection->GetName());
        }
        // register own dataset
        aCollection->SetDataSet(this);
        mClassBoxes.Notify();
        return aCollection;
      }
    }
  }
  // obviously it was not possible to add the collection
  return nullptr;
}


ggClassyCollection* ggClassyDataSet::RemoveCollection(const QString& aName)
{
  ggClassyCollection* vCollection = mCollections.RemoveCollection(aName);
  if (vCollection != nullptr) {
    vCollection->SetDataSet(nullptr);
    mClassBoxes.Notify();
  }
  return vCollection;
}


ggClassyCollectionContainer& ggClassyDataSet::GetCollections()
{
  return mCollections;
}


const ggClassyCollectionContainer& ggClassyDataSet::GetCollections() const
{
  return mCollections;
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


void ggClassyDataSet::DeleteClass(const QString& aClassName)
{
  mClasses.DeleteClass(aClassName);
  mClassBoxes.DeleteClassBoxes(aClassName);
}


ggClassyClassBox* ggClassyDataSet::AddClassBox(ggClassyClassBox* aClassBox)
{
  mClassBoxes.AddClassBox(aClassBox);
  mSubjectConnections.Notify();
  return aClassBox;
}


ggClassyClassContainer& ggClassyDataSet::GetClasses()
{
  return mClasses;
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


void ggClassyDataSet::Clear()
{
  // only notify when all members of ggDataSet are cleared
  ggSubject::cExecutorLazy vClassesLazy(&mClasses);
  ggSubject::cExecutorLazy vClassBoxesLazy(&mClassBoxes);

  // clear the containers
  mCollections.DeleteAllCollections();
  mClasses.DeleteAllClasses();
  mClassBoxes.DeleteAllClassBoxes();
  
  ggWalkerT<std::vector<ggClassyFrame*>::iterator> vFramesWalker(mFrames);
  while (vFramesWalker) delete *vFramesWalker;
  mFrames.clear();
}


ggClassyDataSet* ggClassyDataSet::CreateTestDataSet()
{
  ggClassyDataSet* vDataSet = new ggClassyDataSet();

  ggClassyCollection* vCollectionA = vDataSet->AddCollection(new ggClassyCollection("ggCollectionA"));
  ggClassyCollection* vCollectionB = vDataSet->AddCollection(new ggClassyCollection("ggCollectionB"));
  ggClassyCollection* vCollectionC = vDataSet->AddCollection(new ggClassyCollection("ggCollectionC"));

  ggClassyClass* vClassA = vDataSet->AddClass(new ggClassyClass("ggClassA"));
  vClassA->AddMember("Ping()", "void");
  vClassA->AddMember("Pong()", "void");
  vClassA->AddMember("GetB1()", "ggClassB");
  vClassA->AddMember("GetB2()", "ggClassB");
  vClassA->SetDescription("The answer to life the universe and everything.");
  vClassA->SetCollectionName(vCollectionA->GetName());

  ggClassyClass* vClassB = vDataSet->AddClass(new ggClassyClass("ggClassB"));
  vClassB->AddBaseClassName("ggClassA");
  vClassB->AddMember("Blubb()", "QString");
  vClassB->AddMember("GetA()", "ggClassA");
  vClassB->SetDescription("One fish, two fish, red fish, blue fish.");
  vClassB->SetCollectionName(vCollectionA->GetName());

  ggClassyClass* vClassC = vDataSet->AddClass(new ggClassyClass("ggClassC"));
  vClassC->AddBaseClassName("ggClassA");
  vClassC->AddBaseClassName("ggClassB");
  vClassC->AddMember("GetName()", "QString");
  vClassC->AddMember("SayHello()", "QString");
  vClassC->SetDescription("This is a very useful description ot the class!");
  vClassC->SetCollectionName(vCollectionC->GetName());

  ggClassyClass* vClassD = vDataSet->AddClass(new ggClassyClass("ggClassD"));
  vClassD->AddMember("YouWannaDance()", "QString");
  vClassD->AddMember("DamnImLookingGood()", "QString");
  vClassD->SetDescription("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                          "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

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
