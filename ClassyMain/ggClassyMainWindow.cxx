// 0) include own header
#include "ggClassyMainWindow.h"
#include "ui_ggClassyMainWindow.h"

// 1) include system or QT
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyMain/ggClassyApplication.h"
#include "ClassyGraphics/ggClassyGraphicsScene.h"


ggClassyMainWindow::ggClassyMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ggClassyMainWindow),
  mDataBrowser(new ggClassyDataBrowserDockWidget(this)),
  mDataProperties(new ggClassyDataPropertiesDockWidget(this))
{
  ui->setupUi(this);

  ui->mainToolBar->hide();
  ui->statusBar->setContentsMargins(0, -6, 0, -6);
  ui->statusBar->addPermanentWidget(ui->mZoomLabel);
  ui->statusBar->addPermanentWidget(ui->mZoomComboBox);
  ui->statusBar->addPermanentWidget(ui->mZoomFitPushButton);
  ui->statusBar->addPermanentWidget(ui->mZoomResetPushButton);
  ui->centralWidget->layout()->setMargin(0);
  ui->centralWidget->layout()->setSpacing(0);
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
  connect(ui->mOpenPushButton, SIGNAL(clicked()), this, SLOT(OpenDataSet()));
  connect(ui->mSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveDataSetAs()));
  connect(ui->mMoveTopPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsTop()));
  connect(ui->mMoveUpPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsUp()));
  connect(ui->mMoveDownPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsDown()));
  connect(ui->mMoveBottomPushButton, SIGNAL(clicked()), this, SLOT(MoveSelectedItemsBottom()));
  connect(ui->mColorWheelWidget, SIGNAL(ColorChanged(QColor)), ui->mColorBrightnessWidget, SLOT(SetColor(QColor)));

  addDockWidget(Qt::LeftDockWidgetArea, mDataBrowser);
  addDockWidget(Qt::LeftDockWidgetArea, mDataProperties);

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


QPointF& ggClassyMainWindow::GetNewPosition()
{
  static QPointF vPosition(-300.0f, -200.0f);
  vPosition += QPointF(20.0f, 20.0f);
  if (vPosition.x() > 150.0f) vPosition.setX(-300.0f);
  if (vPosition.y() > 100.0f) vPosition.setY(-200.0f);
  return vPosition;
}


void ggClassyMainWindow::on_mNewClassPushButton_clicked()
{
  // find a new unused class name
  QString vClassNameBase = "ggClassyClass";
  int vClassNameNumber = 0;
  QString vClassName = vClassNameBase + QString::number(vClassNameNumber++);
  while (ggClassyApplication::GetInstance().GetDataSet()->FindClass(vClassName) != nullptr) {
    vClassName = vClassNameBase + QString::number(vClassNameNumber++);
  }

  // create the new class
  ggClassyClass* vClass = new ggClassyClass(vClassName);
  ggClassyClassBox* vClassBox = new ggClassyClassBox(vClassName);

  // place it at somewhere
  vClassBox->SetPosition(GetNewPosition());

  // add it to the dataset
  ggClassyApplication::GetInstance().GetDataSet()->AddClassBox(vClassBox);
  ggClassyApplication::GetInstance().GetDataSet()->AddClass(vClass);

  // select the new class box
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {
    ggClassyGraphicsScene::tClassBoxes vSelectedClassBoxes;
    vSelectedClassBoxes.insert(vClassBox);
    vScene->SelectClassBoxes(vSelectedClassBoxes);
  }
}


void ggClassyMainWindow::on_mNewClassBoxPushButton_clicked()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {

    // get the selected class boxes
    const ggClassyGraphicsScene::tClassBoxes& vSelectedClassBoxes = vScene->GetSelectedClassBoxes();
    ggClassyGraphicsScene::tClassBoxes vNewlassBoxes;

    // loop over all selected class boxes
    ggWalkerT<ggClassyGraphicsScene::tClassBoxes::const_iterator> vSelectedClassBoxesWalker(vSelectedClassBoxes);
    while (vSelectedClassBoxesWalker) {

      // add new box for each selected box
      const ggClassyClassBox* vSelectedClassBox = *vSelectedClassBoxesWalker;
      ggClassyClassBox* vClassBox = new ggClassyClassBox(*vSelectedClassBox);
      vClassBox->SetPosition(vClassBox->GetPosition() + QPointF(20.0f, 20.0f));
      ggClassyApplication::GetInstance().GetDataSet()->AddClassBox(vClassBox);
      vNewlassBoxes.insert(vClassBox);
    }

    // select the new class box(es)
    vScene->SelectClassBoxes(vNewlassBoxes);
  }
}


void ggClassyMainWindow::on_mDeleteClassPushButton_clicked()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {

    // get the selected class boxes and the dataset
    const ggClassyGraphicsScene::tClassBoxes& vSelectedClassBoxes = vScene->GetSelectedClassBoxes();
    ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

    // loop over all selected class boxes
    ggSubject::cExecutorLazy vLazyClassBoxes(&vDataSet->GetClassBoxes());
    ggWalkerT<ggClassyGraphicsScene::tClassBoxes::const_iterator> vSelectedClassBoxesWalker(vSelectedClassBoxes);
    while (vSelectedClassBoxesWalker) {

      // delete the class
      const ggClassyClassBox* vSelectedClassBox = *vSelectedClassBoxesWalker;
      QString vSelectedClassName = vSelectedClassBox->GetClassName();
      vDataSet->DeleteClass(vSelectedClassName);
    }
  }
}


void ggClassyMainWindow::on_mDeleteClassBoxPushButton_clicked()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {

    // get the selected class boxes and the dataset
    const ggClassyGraphicsScene::tClassBoxes& vSelectedClassBoxes = vScene->GetSelectedClassBoxes();
    ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

    // loop over all selected class boxes
    ggSubject::cExecutorLazy vLazyClassBoxes(&vDataSet->GetClassBoxes());
    ggWalkerT<ggClassyGraphicsScene::tClassBoxes::const_iterator> vSelectedClassBoxesWalker(vSelectedClassBoxes);
    while (vSelectedClassBoxesWalker) {

      // delete the box
      const ggClassyClassBox* vSelectedClassBox = *vSelectedClassBoxesWalker;
      vDataSet->GetClassBoxes().DeleteClassBox(vSelectedClassBox);
    }
  }
}


void ggClassyMainWindow::OpenDataSet()
{
  QString vFileName = QFileDialog::getOpenFileName(this, tr("Open Classy Architect File"),
                                                   QDir::currentPath(),
                                                   tr("Classy Architect Files (*.cax *.xml)"));
  if (vFileName.isEmpty()) return;

  QFile vFile(vFileName);
  if (!vFile.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Classy Architect File"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(vFileName).arg(vFile.errorString()));
    return;
  }

  bool vOpened = ggClassyApplication::GetInstance().OpenDataSet(&vFile);
  if (vOpened) ui->mGraphicsView->SetZoomReset();
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
                          tr("Cannot write file\n\"%1\"\n%2.")
                          .arg(vFileName).arg(vFile.errorString()));
    return;
  }

  ggClassyApplication::GetInstance().SaveDataSet(&vFile);
}


void ggClassyMainWindow::MoveSelectedItemsTop()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedClassBoxesTop();
}


void ggClassyMainWindow::MoveSelectedItemsBottom()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedClassBoxesBottom();
}


void ggClassyMainWindow::MoveSelectedItemsUp()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedClassBoxesUp();
}


void ggClassyMainWindow::MoveSelectedItemsDown()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedClassBoxesDown();
}
