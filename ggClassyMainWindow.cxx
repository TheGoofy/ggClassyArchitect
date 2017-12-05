#include "ggClassyMainWindow.h"
#include "ui_ggClassyMainWindow.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

#include "ggClassyApplication.h"


ggClassyMainWindow::ggClassyMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ggClassyMainWindow)
{
  ui->setupUi(this);

  ui->mainToolBar->hide();
  ui->statusBar->hide();
  ui->centralWidget->layout()->setMargin(2);
  ui->centralWidget->layout()->setSpacing(2);
  ui->mZoomComboBox->setCompleter(0);
  ui->mZoomComboBox->setFocusPolicy(Qt::ClickFocus);

  // this connects automatically: connect(ui->mZoomComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mZoomComboBox_currentIndexChanged(int)));
  connect(ui->mZoomComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(on_mZoomComboBox_editingFinished()));
  connect(ui->mSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveDataSetAs()));

  Attach(&ggClassyApplication::GetInstance().Zoom());

  UpdateUI();
}


ggClassyMainWindow::~ggClassyMainWindow()
{
  delete ui;
}


void ggClassyMainWindow::UpdateUI()
{
  ui->mZoomComboBox->lineEdit()->setText(ZoomToString(ggClassyApplication::GetInstance().Zoom().GetValue()));
}


void ggClassyMainWindow::Update(const ggSubject* aSubject)
{
  if (aSubject == &ggClassyApplication::GetInstance().Zoom()) {
    UpdateUI();
  }
}


QString ggClassyMainWindow::ZoomToString(float aZoomFloat) const
{
  return QString::number((int)(100.0f*aZoomFloat + 0.5f)) + "%";
}


float ggClassyMainWindow::StringToZoom(const QString& aZoomString) const
{
  QStringList vZoomStrings = aZoomString.split("%");
  float vZoomFloat = vZoomStrings.first().toFloat() / 100.0f;
  return vZoomFloat;
}


void ggClassyMainWindow::on_mZoomComboBox_activated(int aIndex)
{
  float vZoomFloat = StringToZoom(ui->mZoomComboBox->itemText(aIndex));
  ggClassyApplication::GetInstance().Zoom().SetValue(vZoomFloat);
  ggClassyApplication::GetInstance().Zoom().Notify();
}


void ggClassyMainWindow::on_mZoomComboBox_editingFinished()
{
  float vZoomFloat = StringToZoom(ui->mZoomComboBox->lineEdit()->text());
  if (vZoomFloat > 0.0f) ggClassyApplication::GetInstance().Zoom().SetValue(vZoomFloat);
  ggClassyApplication::GetInstance().Zoom().Notify();
}


void ggClassyMainWindow::on_mZoomFitPushButton_clicked()
{
  ui->mClassyGraphicsView->SetZoomFit();
}


void ggClassyMainWindow::on_mAddClassPushButton_clicked()
{
  static ggSubject::cExecutorLazy* vExecutorLazy = nullptr;
  if (vExecutorLazy == nullptr) {
    vExecutorLazy = new ggSubject::cExecutorLazy(&ggClassyApplication::GetInstance().Zoom());
  }
  else {
    delete vExecutorLazy;
    vExecutorLazy = nullptr;
  }
  qDebug() << __PRETTY_FUNCTION__ << "- IsLazy() =" << ggClassyApplication::GetInstance().Zoom().IsLazy();
}


void ggClassyMainWindow::SaveDataSetAs()
{
  QString vFileName = QFileDialog::getSaveFileName(this, tr("Save Classy Architect File"),
                                                   QDir::currentPath(),
                                                   tr("Classy Architect Files (*.cax *.xml)"));
  if (vFileName.isEmpty()) return;

  QFile vFile(vFileName);
  if (!vFile.open(QFile::WriteOnly | QFile::Text)) {
     QMessageBox::warning(this, tr("Classy Architect File"),
                          tr("Cannot write file %1:\n%2.")
                          .arg(vFileName)
                          .arg(vFile.errorString()));
    return;
  }

  ggClassyApplication::GetInstance().SaveDataSet(&vFile);
}
