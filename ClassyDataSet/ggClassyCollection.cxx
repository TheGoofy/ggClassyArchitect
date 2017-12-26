// 0) include own header
#include "ggClassyCollection.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyCollection::ggClassyCollection() :
  mName("unknown"),
  mDataSet(nullptr)
{
  Construct();
}


ggClassyCollection::ggClassyCollection(const QString& aName) :
  mName(aName),
  mDataSet(nullptr)
{
  Construct();
}


void ggClassyCollection::Construct()
{
  mNameBackground = QColor(150, 75, 0, 255);
}


ggClassyCollection::~ggClassyCollection()
{
  if (mDataSet != nullptr) mDataSet->RemoveCollection(mName);
}


const QString& ggClassyCollection::TypeID()
{
  static const QString vTypeID("ggClassyCollection");
  return vTypeID;
}


const QString& ggClassyCollection::VTypeID() const
{
  return TypeID();
}


QDomElement ggClassyCollection::CreateDomElement(QDomDocument& aDocument) const
{
  // main node
  QDomElement vElement = aDocument.createElement(TypeID());

  // name
  vElement.setAttribute("mName", mName);

  // properties
  QString vString;
  QDebug vDebug(&vString);
  vDebug.nospace();
  vDebug.noquote();
  vDebug << mNameBackground;
  QDomElement vElementNameBackground = aDocument.createElement("mNameBackground");
  vElementNameBackground.setAttribute("QBrush", vString);
  vElement.appendChild(vElementNameBackground);

  // return dom node
  return vElement;
}


ggClassyCollection* ggClassyCollection::Create(const QDomElement& aElement,
                                               ggClassyDataSet* aDataSet)
{
  // goofy todo
  return nullptr;
}


void ggClassyCollection::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
  }
}


ggClassyDataSet* ggClassyCollection::GetDataSet() const
{
  return mDataSet;
}


const QString& ggClassyCollection::GetName() const
{
  return mName;
}


void ggClassyCollection::SetName(const QString& aName)
{
  if (aName != mName) {
    mName = aName;
  }
}


const QBrush& ggClassyCollection::GetNameBackground() const
{
  return mNameBackground;
}


void ggClassyCollection::SetNameBackground(const QBrush& aBrush)
{
  if (aBrush != mNameBackground) {
    mNameBackground = aBrush;
    Notify();
  }
}
