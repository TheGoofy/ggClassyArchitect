#ifndef GGCLASSYAPPLICATION_H
#define GGCLASSYAPPLICATION_H

#include <QApplication>
#include "ggSubjectT.h"

class ggClassyApplication : public QApplication
{
  Q_OBJECT

public:

  ggClassyApplication(int &argc, char **argv);
  virtual ~ggClassyApplication();

  static ggClassyApplication& GetInstance();

  ggSubjectFloat& Zoom();
  const ggSubjectFloat& Zoom() const;

private:

  ggSubjectFloat mZoom;

};

#endif // GGCLASSYAPPLICATION_H
