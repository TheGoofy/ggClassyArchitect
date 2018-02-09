// 0) include own header
#include "ggClassySettingsDockWidget.h"
#include "ui_ggClassySettingsDockWidget.h"

// 1) include system or QT
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "BaseWidgets/ggColorDialog.h"
#include "ClassyDataSet/ggClassySettings.h"


ggClassySettingsDockWidget::ggClassySettingsDockWidget(QWidget* aParent) :
  QDockWidget(aParent),
  ui(new Ui::ggClassySettingsDockWidget),
  mEditedColorPreview(nullptr)
{
  ui->setupUi(this);

  ui->mSelectionColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetSelectionColor());
  ui->mHighlightColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetHighlightColor());
  ui->mShadowColorPreviewWidget->SetColor(ggClassySettings::GetInstance()->GetShadowColor());
  ui->mShadowWidthSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowWidth());
  ui->mShadowOffsetXSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowOffset().x());
  ui->mShadowOffsetYSpinBox->setValue(ggClassySettings::GetInstance()->GetShadowOffset().y());

  ui->mSelectionColorPreviewWidget->SetClickable(true);
  ui->mHighlightColorPreviewWidget->SetClickable(true);
  ui->mShadowColorPreviewWidget->SetClickable(true);
}


ggClassySettingsDockWidget::~ggClassySettingsDockWidget()
{
  delete ui;
}


void ggClassySettingsDockWidget::SetColor(const QColor& aColor)
{
  if (mEditedColorPreview != nullptr) {
    // update preview
    mEditedColorPreview->SetColor(aColor);
    // update the settings
    if (mEditedColorPreview == ui->mSelectionColorPreviewWidget) ggClassySettings::GetInstance()->SetSelectionColor(aColor);
    if (mEditedColorPreview == ui->mHighlightColorPreviewWidget) ggClassySettings::GetInstance()->SetHighlightColor(aColor);
    if (mEditedColorPreview == ui->mShadowColorPreviewWidget) ggClassySettings::GetInstance()->SetShadowColor(aColor);
  }
}


void ggClassySettingsDockWidget::ColorAccept()
{
  // stop editing
  mEditedColorPreview = nullptr;
}


void ggClassySettingsDockWidget::ColorReject()
{
  if (mEditedColorPreview != nullptr) {
    // restore backup color
    SetColor(mColorBackup);
    // stop editing
    mEditedColorPreview = nullptr;
  }
}


void ggClassySettingsDockWidget::EditColor(ggColorPreviewWidget* aColorPreview)
{
  mEditedColorPreview = aColorPreview;
  if (mEditedColorPreview != nullptr) {
    // remember current color
    mColorBackup = mEditedColorPreview->GetColor();
    // setup color dialog
    ggColorDialog vColorDialog(this);
    vColorDialog.setCurrentColor(mEditedColorPreview->GetColor());
    connect(&vColorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(SetColor(QColor)));
    connect(&vColorDialog, SIGNAL(rejected()), this, SLOT(ColorReject()));
    vColorDialog.exec();
  }
}


void ggClassySettingsDockWidget::on_mSelectionColorPreviewWidget_Clicked()
{
  EditColor(ui->mSelectionColorPreviewWidget);
}


void ggClassySettingsDockWidget::on_mHighlightColorPreviewWidget_Clicked()
{
  EditColor(ui->mHighlightColorPreviewWidget);
}


void ggClassySettingsDockWidget::on_mShadowColorPreviewWidget_Clicked()
{
  EditColor(ui->mShadowColorPreviewWidget);
}


void ggClassySettingsDockWidget::on_mShadowWidthSpinBox_valueChanged(double aValue)
{
  ggClassySettings::GetInstance()->SetShadowWidth(aValue);
}


void ggClassySettingsDockWidget::on_mShadowOffsetXSpinBox_valueChanged(double aValue)
{
  QPointF vShadowOffset = ggClassySettings::GetInstance()->GetShadowOffset();
  ggClassySettings::GetInstance()->SetShadowOffset(QPointF(aValue, vShadowOffset.y()));
}


void ggClassySettingsDockWidget::on_mShadowOffsetYSpinBox_valueChanged(double aValue)
{
  QPointF vShadowOffset = ggClassySettings::GetInstance()->GetShadowOffset();
  ggClassySettings::GetInstance()->SetShadowOffset(QPointF(vShadowOffset.x(), aValue));
}
