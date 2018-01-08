#include "ggColorDialog.h"
#include "ui_ggColorDialog.h"


ggColorDialog::ggColorDialog(QWidget *aParent) :
  QDialog(aParent),
  ui(new Ui::ggColorDialog)
{
  ui->setupUi(this);
  ui->mColorChannelR->SetChannel(ggColorChannelWidget::cChannel::eRed);
  ui->mColorChannelG->SetChannel(ggColorChannelWidget::cChannel::eGreen);
  ui->mColorChannelB->SetChannel(ggColorChannelWidget::cChannel::eBlue);
  ui->mColorChannelA->SetChannel(ggColorChannelWidget::cChannel::eAlpha);
  ui->mColorChannelV->SetChannel(ggColorChannelWidget::cChannel::eValue);
  connect(ui->mColorWheel, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
  connect(ui->mColorChannelR, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
  connect(ui->mColorChannelG, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
  connect(ui->mColorChannelB, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
  connect(ui->mColorChannelA, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
  connect(ui->mColorChannelV, SIGNAL(ColorChanged(QColor)), this, SLOT(setCurrentColor(QColor)));
}


ggColorDialog::~ggColorDialog()
{
  delete ui;
}


void ggColorDialog::on_mColorRSpinBox_valueChanged(int aValue)
{
  QColor vColor(mColor);
  vColor.setRed(aValue);
  setCurrentColor(vColor);
}


void ggColorDialog::on_mColorGSpinBox_valueChanged(int aValue)
{
  QColor vColor(mColor);
  vColor.setGreen(aValue);
  setCurrentColor(vColor);
}


void ggColorDialog::on_mColorBSpinBox_valueChanged(int aValue)
{
  QColor vColor(mColor);
  vColor.setBlue(aValue);
  setCurrentColor(vColor);
}


void ggColorDialog::on_mColorASpinBox_valueChanged(int aValue)
{
  QColor vColor(mColor);
  vColor.setAlpha(aValue);
  setCurrentColor(vColor);
}


const QColor& ggColorDialog::currentColor() const
{
  return mColor;
}


void ggColorDialog::setCurrentColor(const QColor& aColor)
{
  if (mColor != aColor) {

    // update master color
    mColor = aColor;

    // color wheel and channels ...
    ui->mColorWheel->SetColor(aColor);
    ui->mColorChannelR->SetColor(aColor);
    ui->mColorChannelG->SetColor(aColor);
    ui->mColorChannelB->SetColor(aColor);
    ui->mColorChannelA->SetColor(aColor);
    ui->mColorChannelV->SetColor(aColor);

    // spinboxes re-emit changes, and lead to color value drift
    QSignalBlocker vBlockR(ui->mColorRSpinBox);
    QSignalBlocker vBlockG(ui->mColorGSpinBox);
    QSignalBlocker vBlockB(ui->mColorBSpinBox);
    QSignalBlocker vBlockA(ui->mColorASpinBox);
    ui->mColorRSpinBox->setValue(aColor.red());
    ui->mColorGSpinBox->setValue(aColor.green());
    ui->mColorBSpinBox->setValue(aColor.blue());
    ui->mColorASpinBox->setValue(aColor.alpha());

    // update preview
    ui->mColorPreview->SetColor(aColor);

    // if an external caller doesn't like this signal, he can block it ...
    emit currentColorChanged(aColor);
  }
}
