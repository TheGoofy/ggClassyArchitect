// 0) include own header
#include "ggClassySettingsDockWidget.h"
#include "ui_ggClassySettingsDockWidget.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassySettings.h"


ggClassySettingsDockWidget::ggClassySettingsDockWidget(QWidget* aParent) :
  QDockWidget(aParent),
  ui(new Ui::ggClassySettingsDockWidget)
{
  ui->setupUi(this);

  ui->mSelectionColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetSelectionColor());
  ui->mHighlightColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetHighlightColor());
  ui->mShadowColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetShadowColor());
  ui->mShadowWidthSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowWidth());
  ui->mShadowOffsetXSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowOffset().x());
  ui->mShadowOffsetYSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowOffset().y());
}


ggClassySettingsDockWidget::~ggClassySettingsDockWidget()
{
  delete ui;
}
