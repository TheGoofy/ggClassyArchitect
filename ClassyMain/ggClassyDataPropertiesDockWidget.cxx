#include "ggClassyDataPropertiesDockWidget.h"
#include "ui_ggClassyDataPropertiesDockWidget.h"

ggClassyDataPropertiesDockWidget::ggClassyDataPropertiesDockWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::ggClassyDataPropertiesDockWidget)
{
  ui->setupUi(this);
}

ggClassyDataPropertiesDockWidget::~ggClassyDataPropertiesDockWidget()
{
  delete ui;
}
