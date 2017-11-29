#include "ggClassyApplication.h"


ggClassyApplication::ggClassyApplication(int &argc, char **argv)
: QApplication(argc, argv)
{
}


ggClassyApplication::~ggClassyApplication()
{
}


ggSubjectFloat& ggClassyApplication::Zoom()
{
  return mZoom;
}


const ggSubjectFloat& ggClassyApplication::Zoom() const
{
  return mZoom;
}


ggClassyApplication& ggClassyApplication::GetInstance()
{
  return dynamic_cast<ggClassyApplication&>(*instance());
}
