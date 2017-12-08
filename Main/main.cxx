#include "Main/ggClassyMainWindow.h"
#include "Main/ggClassyApplication.h"


int main(int argc, char *argv[])
{
  ggClassyApplication vApplication(argc, argv);
  ggClassyMainWindow vMainWindow;
  vMainWindow.show();
  return vApplication.exec();
}
