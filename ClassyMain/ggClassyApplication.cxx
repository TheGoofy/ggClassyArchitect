#include "ClassyMain/ggClassyApplication.h"

#include <QDomDocument>
#include <QTextStream>
#include <QDateTime>

#include "ClassyDataSet/ggClassyDataSet.h"
#include "Base/ggWalkerT.h"


ggClassyApplication::ggClassyApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  mVersionMajor(0),
  mVersionMinor(0),
  mVersionPatch(1),
  mDataSet(nullptr)
{
  mDataSet = ggClassyDataSet::GenerateTestData();
}


ggClassyApplication::~ggClassyApplication()
{
}


const QString& ggClassyApplication::TypeID()
{
  static const QString vTypeID("ggClassyApplication");
  return vTypeID;
}


const QString& ggClassyApplication::VTypeID() const
{
  return TypeID();
}


QString ggClassyApplication::GetVersion() const
{
  return QString("%1.%2.%3").arg(mVersionMajor).arg(mVersionMinor).arg(mVersionPatch);
}


ggUInt32 ggClassyApplication::GetVersionMajor() const
{
  return mVersionMajor;
}


ggUInt32 ggClassyApplication::GetVersionMinor() const
{
  return mVersionMinor;
}


ggUInt32 ggClassyApplication::GetVersionPatch() const
{
  return mVersionPatch;
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

  QDomElement vAppInfoElement = vDomDocument.createElement("ggApplicationInfo");
  vAppInfoElement.setAttribute("mName", TypeID());
  vAppInfoElement.setAttribute("mVersion", GetVersion());
  vAppInfoElement.setAttribute("mSaveDate", QDateTime::currentDateTimeUtc().toString());

  QDomElement vRootElement = GetDataSet()->CreateDomElement(vDomDocument);
  vRootElement.appendChild(vAppInfoElement);
  vDomDocument.appendChild(vRootElement);

  //
  // write file
  //
  const int vIndentSize = 2;
  QTextStream vTextStream(aDevice);
  vDomDocument.save(vTextStream, vIndentSize);
}


ggClassyApplication& ggClassyApplication::GetInstance()
{
  return dynamic_cast<ggClassyApplication&>(*instance());
}
