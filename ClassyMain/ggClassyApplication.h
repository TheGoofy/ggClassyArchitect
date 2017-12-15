#ifndef GGCLASSYAPPLICATION_H
#define GGCLASSYAPPLICATION_H

// 1) include system or QT
#include <QApplication>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubjectT.h"

// 3) forward declarations
class ggClassyDataSet;
class QIODevice;

/**
 * @brief The ggClassyApplication class
 */
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
  bool OpenDataSet(QIODevice* aIODevice);
  void SaveDataSet(QIODevice* aIODevice) const;

private:

  const ggUInt32 mVersionMajor;
  const ggUInt32 mVersionMinor;
  const ggUInt32 mVersionPatch;

  ggClassyDataSet* mDataSet;

};

#endif // GGCLASSYAPPLICATION_H
