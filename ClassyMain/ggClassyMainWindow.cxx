// 0) include own header
#include "ggClassyMainWindow.h"
#include "ui_ggClassyMainWindow.h"

// 1) include system or QT
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyMain/ggClassyApplication.h"
#include "ClassyGraphics/ggClassyGraphicsScene.h"


ggClassyMainWindow::ggClassyMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ggClassyMainWindow)
{
  ui->setupUi(this);

  ui->mainToolBar->hide();
  ui->statusBar->setContentsMargins(0, -6, 0, -6);
  ui->statusBar->addPermanentWidget(ui->mZoomLabel);
  ui->statusBar->addPermanentWidget(ui->mZoomComboBox);
  ui->statusBar->addPermanentWidget(ui->mZoomFitPushButton);
  ui->statusBar->addPermanentWidget(ui->mZoomResetPushButton);
  ui->centralWidget->layout()->setMargin(2);
  ui->centralWidget->layout()->setSpacing(2);
  ui->mZoomComboBox->setCompleter(0);
  ui->mZoomComboBox->setFocusPolicy(Qt::ClickFocus);

  // make some objects (for development and testing)
  ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

  // create a classy scene and link it with dataset
  ggClassyGraphicsScene* vScene = new ggClassyGraphicsScene(this);
  vScene->SetDataSet(vDataSet);
  ui->mGraphicsView->setScene(vScene);

  // this connects automatically: connect(ui->mZoomComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mZoomComboBox_currentIndexChanged(int)));
  connect(ui->mZoomComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(on_mZoomComboBox_editingFinished()));
  connect(ui->mSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveDataSetAs()));
  connect(ui->mMoveTopPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsToTop()));
  connect(ui->mMoveUpPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsUp()));
  connect(ui->mMoveDownPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsDown()));
  connect(ui->mMoveBottomPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsToBottom()));

  // register subject(s)
  Attach(ui->mGraphicsView->GetSubjectZoom());

  UpdateUI();
}


ggClassyMainWindow::~ggClassyMainWindow()
{
  delete ui;
}


void ggClassyMainWindow::UpdateUI()
{
  ui->mZoomComboBox->lineEdit()->setText(ZoomToString(ui->mGraphicsView->GetSubjectZoom()->GetValue()));
}


void ggClassyMainWindow::Update(const ggSubject* aSubject)
{
  if (aSubject == ui->mGraphicsView->GetSubjectZoom()) {
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
  ui->mGraphicsView->GetSubjectZoom()->SetValue(vZoomFloat);
  ui->mGraphicsView->GetSubjectZoom()->Notify();
}


void ggClassyMainWindow::on_mZoomComboBox_editingFinished()
{
  float vZoomFloat = StringToZoom(ui->mZoomComboBox->lineEdit()->text());
  if (vZoomFloat > 0.0f) ui->mGraphicsView->GetSubjectZoom()->SetValue(vZoomFloat);
  ui->mGraphicsView->GetSubjectZoom()->Notify();
}


void ggClassyMainWindow::on_mZoomFitPushButton_clicked()
{
  ui->mGraphicsView->SetZoomFit();
}


void ggClassyMainWindow::on_mZoomResetPushButton_clicked()
{
  ui->mGraphicsView->SetZoomReset();
}


void ggClassyMainWindow::on_mAddClassPushButton_clicked()
{
  ggClassyClass* vClass = new ggClassyClass("ggClassD");
  vClass->SetMembersText("Goofy()\tggClassA\nIs()\tggClassB\nHere()\tggClassC");
  ggClassyClassBox* vClassBox = new ggClassyClassBox("ggClassD");
  vClassBox->SetPosition(QPointF(0, -400));
  ggClassyApplication::GetInstance().GetDataSet()->AddClassBox(vClassBox);
  ggClassyApplication::GetInstance().GetDataSet()->AddClass(vClass);
}


void ggClassyMainWindow::SaveDataSetAs()
{
  QString vFileName = QFileDialog::getSaveFileName(this, tr("Save Classy Architect File"),
                                                   "",//QDir::currentPath(),
                                                   tr("Classy Architect Files (*.cax *.xml)"));
  if (vFileName.isEmpty()) return;

  QFile vFile(vFileName);
  if (!vFile.open(QFile::WriteOnly | QFile::Text)) {
     QMessageBox::warning(this, tr("Classy Architect File"),
                          tr("Cannot write file\n\"%1\"\n%2.")
                          .arg(vFileName).arg(vFile.errorString()));
    return;
  }

  ggClassyApplication::GetInstance().SaveDataSet(&vFile);
}


void ggClassyMainWindow::MoveSelectedItemsToTop()
{
  if (ui->mGraphicsView->scene() != nullptr) {
    QGraphicsScene* vScene = ui->mGraphicsView->scene();
    QList<QGraphicsItem*> vSelectedItems = vScene->selectedItems();
    foreach (QGraphicsItem* vItem, vSelectedItems) {
      vScene->removeItem(vItem);
      vScene->addItem(vItem);
    }
  }
}


void ggClassyMainWindow::MoveSelectedItemsToBottom()
{
  if (ui->mGraphicsView->scene() != nullptr) {
    QGraphicsScene* vScene = ui->mGraphicsView->scene();
    QList<QGraphicsItem*> vSelectedItems = vScene->selectedItems();
    // todo
  }
}


void ggClassyMainWindow::MoveSelectedItemsUp()
{
  if (ui->mGraphicsView->scene() != nullptr) {
    QGraphicsScene* vScene = ui->mGraphicsView->scene();
    QList<QGraphicsItem*> vSelectedItems = vScene->selectedItems();
    // todo
  }
}


void ggClassyMainWindow::MoveSelectedItemsDown()
{
  if (ui->mGraphicsView->scene() != nullptr) {
    QGraphicsScene* vScene = ui->mGraphicsView->scene();
    QList<QGraphicsItem*> vSelectedItems = vScene->selectedItems();
    // todo
  }
}
