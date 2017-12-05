#ifndef GGCLASSYAPPLICATION_H
#define GGCLASSYAPPLICATION_H

#include <QApplication>
#include "ggSubjectT.h"

class ggClassyDataSet;
class QIODevice;

class ggClassyApplication : public QApplication
{
  Q_OBJECT

public:

  ggClassyApplication(int &argc, char **argv);
  virtual ~ggClassyApplication();

  static ggClassyApplication& GetInstance();

  ggSubjectFloat& Zoom();
  const ggSubjectFloat& Zoom() const;

  ggClassyDataSet* GetDataSet();
  const ggClassyDataSet* GetDataSet() const;
  void SaveDataSet(QIODevice* aDevice);

private:

  ggClassyDataSet* mDataSet;

  ggSubjectFloat mZoom;

};

#endif // GGCLASSYAPPLICATION_H
