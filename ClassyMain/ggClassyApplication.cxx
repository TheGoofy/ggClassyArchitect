// 0) include own header
#include "ggClassyApplication.h"

// 1) include system or QT
#include <QDomDocument>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyApplication::ggClassyApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  mVersionMajor(0),
  mVersionMinor(0),
  mVersionPatch(1),
  mDataSet(nullptr)
{
  mDataSet = ggClassyDataSet::CreateTestDataSet();
}


ggClassyApplication::~ggClassyApplication()
{
  delete mDataSet;
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


void ggClassyApplication::SaveDataSet(QIODevice* aIODevice) const
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
  QTextStream vTextStream(aIODevice);
  vDomDocument.save(vTextStream, vIndentSize);
}


bool ggClassyApplication::OpenDataSet(QIODevice* aIODevice)
{
  //
  // read the file
  //
  QDomDocument vDomDocument;
  if (vDomDocument.setContent(aIODevice, true)) {

    //
    // parse the dom-elements
    //
    QDomElement vRootElement = vDomDocument.firstChildElement();
    ggClassyDataSet* vDataSet = ggClassyDataSet::Create(vRootElement);

    if (vDataSet != nullptr) {
      // copy contents (will notify all observers)
      *mDataSet = *vDataSet;
      // new dataset is no longer needed
      delete vDataSet;
      // the new dataset was successfully loaded
      return true;
    }
  }

  // something went wrong
  return false;
}


ggClassyApplication& ggClassyApplication::GetInstance()
{
  return dynamic_cast<ggClassyApplication&>(*instance());
}
