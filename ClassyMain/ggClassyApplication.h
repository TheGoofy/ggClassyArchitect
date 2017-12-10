#ifndef GGCLASSYAPPLICATION_H
#define GGCLASSYAPPLICATION_H

#include <QApplication>

#include "Base/ggSubjectT.h"

class ggClassyDataSet;
class QIODevice;

class ggClassyApplication : public QApplication
{
  Q_OBJECT

public:

  ggClassyApplication(int &argc, char **argv);
  virtual ~ggClassyApplication();

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  QString GetVersion() const;
  ggUInt32 GetVersionMajor() const;
  ggUInt32 GetVersionMinor() const;
  ggUInt32 GetVersionPatch() const;

  static ggClassyApplication& GetInstance();

  ggClassyDataSet* GetDataSet();
  const ggClassyDataSet* GetDataSet() const;
  void SaveDataSet(QIODevice* aDevice);

private:

  const ggUInt32 mVersionMajor;
  const ggUInt32 mVersionMinor;
  const ggUInt32 mVersionPatch;

  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYAPPLICATION_H
