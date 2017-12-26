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
  // box border and connection lines
  mBoxBorder.setCapStyle(Qt::FlatCap);
  mBoxBorder.setJoinStyle(Qt::RoundJoin);
  mBoxBorder.setColor(QColor(120, 80, 0, 100));
  mBoxBorder.setWidthF(1.0f);
  mConnectionLines.setColor(QColor(120, 80, 0, 255));
  mConnectionLines.setWidthF(1.5f);

  // class name
  mNameFont.setBold(true);
  mNameColor = QColor(255, 255, 255, 255);
  mNameBackground.setColor(QColor(120, 80, 0, 255));
  mNameBackground.setStyle(Qt::SolidPattern);

  // member functions
  mMembersFont.setFamily("Courier");
  mMembersColor = Qt::black;
  mMembersBackground.setColor(QColor(250, 240, 230, 255));
  mMembersBackground.setStyle(Qt::SolidPattern);

  // description
  mDescriptionFont.setItalic(true);
  mDescriptionColor = QColor(100, 100, 100, 255);
  mDescriptionBackground.setColor(QColor(240, 220, 200, 255));
  mDescriptionBackground.setStyle(Qt::SolidPattern);
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
