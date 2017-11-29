#include "ggClassyMainWindow.h"
#include "ggClassyApplication.h"


int main(int argc, char *argv[])
{
  ggClassyApplication vApplication(argc, argv);
  ggClassyMainWindow vMainWindow;
  vMainWindow.show();
  return vApplication.exec();
}
