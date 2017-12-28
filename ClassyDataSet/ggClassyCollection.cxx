// 0) include own header
#include "ggClassyCollection.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "BaseWidgets/ggUtilityQt.h"
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyCollection::ggClassyCollection(const QString& aName) :
  mName(aName),
  mDataSet(nullptr)
{
  Construct();
}


ggClassyCollection::ggClassyCollection(const ggClassyCollection& aOther)
{
  *this = aOther;
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


QDomElement CreateDomElement(QDomDocument& aDocument,
                             const QColor& aColor,
                             const QString& aTagName = "QColor")
{
  QDomElement vElement = aDocument.createElement(aTagName);
  vElement.setAttribute("QColorARGB", ggUtilityQt::ToString(aColor));
  return vElement;
}


bool ParseDomElement(const QDomElement& aElement,
                     QColor& aColor,
                     const QString& aTagName = "QColor")
{
  if (aElement.tagName() == aTagName) {
    ggUtilityQt::FromString(aElement.attribute("QColorARGB"), aColor);
    return true;
  }
  return false;
}


QDomElement CreateDomElement(QDomDocument& aDocument,
                             const QBrush& aBrush,
                             const QString& aTagName = "QBrush")
{
  QDomElement vElement = aDocument.createElement(aTagName);
  vElement.setAttribute("QBrushColorARGB", ggUtilityQt::ToString(aBrush.color()));
  vElement.setAttribute("QBrushStyle", aBrush.style());
  return vElement;
}


bool ParseDomElement(const QDomElement& aElement,
                     QBrush& aBrush,
                     const QString& aTagName = "QBrush")
{
  if (aElement.tagName() == aTagName) {
    bool vOK = false;
    QColor vColor = ggUtilityQt::FromString<QColor>(aElement.attribute("QBrushColorARGB"), &vOK);
    if (vOK) aBrush.setColor(vColor);
    Qt::BrushStyle vStyle = static_cast<Qt::BrushStyle>(aElement.attribute("QBrushStyle").toUInt(&vOK));
    if (vOK) aBrush.setStyle(vStyle);
    return true;
  }
  return false;
}


QDomElement CreateDomElement(QDomDocument& aDocument,
                             const QPen& aPen,
                             const QString& aTagName = "QPen")
{
  QDomElement vElement = aDocument.createElement(aTagName);
  vElement.setAttribute("QPenColorARGB", ggUtilityQt::ToString(aPen.color()));
  vElement.setAttribute("QPenWidthF", QString::number(aPen.widthF()));
  return vElement;
}


bool ParseDomElement(const QDomElement& aElement,
                     QPen& aPen,
                     const QString& aTagName = "QPen")
{
  if (aElement.tagName() == aTagName) {
    bool vOK = false;
    QColor vColor = ggUtilityQt::FromString<QColor>(aElement.attribute("QPenColorARGB"), &vOK);
    if (vOK) aPen.setColor(vColor);
    qreal vWidth = aElement.attribute("QPenWidthF").toDouble(&vOK);
    if (vOK) aPen.setWidthF(vWidth);
    return true;
  }
  return false;
}


QDomElement CreateDomElement(QDomDocument& aDocument,
                             const QFont& aFont,
                             const QString& aTagName = "QFont")
{
  QDomElement vElement = aDocument.createElement(aTagName);
  vElement.setAttribute("QFont", aFont.toString());
  return vElement;
}


bool ParseDomElement(const QDomElement& aElement,
                     QFont& aFont,
                     const QString& aTagName = "QFont")
{
  if (aElement.tagName() == aTagName) {
    aFont.fromString(aElement.attribute("QFont"));
    return true;
  }
  return false;
}


QDomElement ggClassyCollection::CreateDomElement(QDomDocument& aDocument) const
{
  // main node
  QDomElement vElement = aDocument.createElement(TypeID());

  // name
  vElement.setAttribute("mName", mName);

  // properties
  vElement.appendChild(::CreateDomElement(aDocument, mBoxBorder, "mBoxBorder"));
  vElement.appendChild(::CreateDomElement(aDocument, mConnectionLines, "mConnectionLines"));
  vElement.appendChild(::CreateDomElement(aDocument, mNameFont, "mNameFont"));
  vElement.appendChild(::CreateDomElement(aDocument, mNameColor, "mNameColor"));
  vElement.appendChild(::CreateDomElement(aDocument, mNameBackground, "mNameBackground"));
  vElement.appendChild(::CreateDomElement(aDocument, mMembersFont, "mMembersFont"));
  vElement.appendChild(::CreateDomElement(aDocument, mMembersColor, "mMembersColor"));
  vElement.appendChild(::CreateDomElement(aDocument, mMembersBackground, "mMembersBackground"));
  vElement.appendChild(::CreateDomElement(aDocument, mDescriptionFont, "mDescriptionFont"));
  vElement.appendChild(::CreateDomElement(aDocument, mDescriptionColor, "mDescriptionColor"));
  vElement.appendChild(::CreateDomElement(aDocument, mDescriptionBackground, "mDescriptionBackground"));

  // return dom node
  return vElement;
}


ggClassyCollection* ggClassyCollection::Create(const QDomElement& aElement)
{
  ggClassyCollection* vCollection = nullptr;
  if (aElement.tagName() == TypeID()) {

    // create collection with name
    vCollection = new ggClassyCollection(aElement.attribute("mName"));

    // parse children (properties)
    QDomElement vChildElement = aElement.firstChildElement();
    while (!vChildElement.isNull()) {

      // try to match property
      bool vOK = false;
      vOK |= ::ParseDomElement(vChildElement, vCollection->mBoxBorder, "mBoxBorder");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mConnectionLines, "mConnectionLines");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mNameFont, "mNameFont");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mNameColor, "mNameColor");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mNameBackground, "mNameBackground");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mMembersFont, "mMembersFont");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mMembersColor, "mMembersColor");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mMembersBackground, "mMembersBackground");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mDescriptionFont, "mDescriptionFont");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mDescriptionColor, "mDescriptionColor");
      vOK |= ::ParseDomElement(vChildElement, vCollection->mDescriptionBackground, "mDescriptionBackground");
      if (!vOK) qDebug() << __PRETTY_FUNCTION__ << "unknown element" << vChildElement.tagName();

      // next child
      vChildElement = vChildElement.nextSiblingElement();
    }
  }
  return vCollection;
}


void ggClassyCollection::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
    Notify();
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
