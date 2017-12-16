#include "ggClassyDataBrowserDockWidget.h"
#include "ui_ggClassyDataBrowserDockWidget.h"


ggClassyDataBrowserDockWidget::ggClassyDataBrowserDockWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::ggClassyDataBrowserDockWidget)
{
  ui->setupUi(this);
}


ggClassyDataBrowserDockWidget::~ggClassyDataBrowserDockWidget()
{
  delete ui;
}
