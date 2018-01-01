// 0) include own header
#include "ggClassyMainWindow.h"
#include "ui_ggClassyMainWindow.h"

// 1) include system or QT
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMetaMethod>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseWidgets/ggUtilityQt.h"
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

  CreateAction(mActionAlignTL, SLOT(AlignTL()), ui->mAlignTL);
  CreateAction(mActionAlignTC, SLOT(AlignTC()), ui->mAlignTC);
  CreateAction(mActionAlignTR, SLOT(AlignTR()), ui->mAlignTR);
  CreateAction(mActionAlignCL, SLOT(AlignCL()), ui->mAlignCL);
  CreateAction(mActionAlignCC, SLOT(AlignCC()), ui->mAlignCC);
  CreateAction(mActionAlignCR, SLOT(AlignCR()), ui->mAlignCR);
  CreateAction(mActionAlignBL, SLOT(AlignBL()), ui->mAlignBL);
  CreateAction(mActionAlignBC, SLOT(AlignBC()), ui->mAlignBC);
  CreateAction(mActionAlignBR, SLOT(AlignBR()), ui->mAlignBR);

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

  ui->mColorChannelWidgetR->SetChannel(ggColorChannelWidget::cChannel::eRed);
  ui->mColorChannelWidgetR->SetColor(ui->mColorWheelWidget->GetColor());
  ui->mColorChannelWidgetG->SetChannel(ggColorChannelWidget::cChannel::eGreen);
  ui->mColorChannelWidgetG->SetColor(ui->mColorWheelWidget->GetColor());
  ui->mColorChannelWidgetB->SetChannel(ggColorChannelWidget::cChannel::eBlue);
  ui->mColorChannelWidgetB->SetColor(ui->mColorWheelWidget->GetColor());
  ui->mColorChannelWidgetA->SetChannel(ggColorChannelWidget::cChannel::eAlpha);
  ui->mColorChannelWidgetA->SetColor(ui->mColorWheelWidget->GetColor());
  ui->mColorChannelWidgetV->SetChannel(ggColorChannelWidget::cChannel::eLightness);
  ui->mColorChannelWidgetV->SetColor(ui->mColorWheelWidget->GetColor());

  ConnectColor(ui->mColorWheelWidget, ui->mColorChannelWidgetR);
  ConnectColor(ui->mColorWheelWidget, ui->mColorChannelWidgetG);
  ConnectColor(ui->mColorWheelWidget, ui->mColorChannelWidgetB);
  ConnectColor(ui->mColorWheelWidget, ui->mColorChannelWidgetA);
  ConnectColor(ui->mColorWheelWidget, ui->mColorChannelWidgetV);

  ConnectColor(ui->mColorChannelWidgetR, ui->mColorWheelWidget);
  ConnectColor(ui->mColorChannelWidgetR, ui->mColorChannelWidgetG);
  ConnectColor(ui->mColorChannelWidgetR, ui->mColorChannelWidgetB);
  ConnectColor(ui->mColorChannelWidgetR, ui->mColorChannelWidgetA);
  ConnectColor(ui->mColorChannelWidgetR, ui->mColorChannelWidgetV);

  ConnectColor(ui->mColorChannelWidgetG, ui->mColorWheelWidget);
  ConnectColor(ui->mColorChannelWidgetG, ui->mColorChannelWidgetR);
  ConnectColor(ui->mColorChannelWidgetG, ui->mColorChannelWidgetB);
  ConnectColor(ui->mColorChannelWidgetG, ui->mColorChannelWidgetA);
  ConnectColor(ui->mColorChannelWidgetG, ui->mColorChannelWidgetV);

  ConnectColor(ui->mColorChannelWidgetB, ui->mColorWheelWidget);
  ConnectColor(ui->mColorChannelWidgetB, ui->mColorChannelWidgetR);
  ConnectColor(ui->mColorChannelWidgetB, ui->mColorChannelWidgetG);
  ConnectColor(ui->mColorChannelWidgetB, ui->mColorChannelWidgetA);
  ConnectColor(ui->mColorChannelWidgetB, ui->mColorChannelWidgetV);

  ConnectColor(ui->mColorChannelWidgetA, ui->mColorWheelWidget);
  ConnectColor(ui->mColorChannelWidgetA, ui->mColorChannelWidgetR);
  ConnectColor(ui->mColorChannelWidgetA, ui->mColorChannelWidgetG);
  ConnectColor(ui->mColorChannelWidgetA, ui->mColorChannelWidgetB);
  ConnectColor(ui->mColorChannelWidgetA, ui->mColorChannelWidgetV);

  ConnectColor(ui->mColorChannelWidgetV, ui->mColorWheelWidget);
  ConnectColor(ui->mColorChannelWidgetV, ui->mColorChannelWidgetR);
  ConnectColor(ui->mColorChannelWidgetV, ui->mColorChannelWidgetG);
  ConnectColor(ui->mColorChannelWidgetV, ui->mColorChannelWidgetB);
  ConnectColor(ui->mColorChannelWidgetV, ui->mColorChannelWidgetA);

  connect(ui->mColorWheelWidget, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));
  connect(ui->mColorChannelWidgetR, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));
  connect(ui->mColorChannelWidgetG, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));
  connect(ui->mColorChannelWidgetB, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));
  connect(ui->mColorChannelWidgetA, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));
  connect(ui->mColorChannelWidgetV, SIGNAL(ColorChanged(QColor)), this, SLOT(ChangeColor(QColor)));

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


void ggClassyMainWindow::ConnectColor(ggColorWheelWidget* aColorWheelWidgetA, ggColorWheelWidget* aColorWheelWidgetB) const
{
  connect(aColorWheelWidgetA, SIGNAL(ColorChanged(QColor)), aColorWheelWidgetB, SLOT(SetColor(QColor)));
  connect(aColorWheelWidgetB, SIGNAL(ColorChanged(QColor)), aColorWheelWidgetA, SLOT(SetColor(QColor)));
}


void ggClassyMainWindow::ConnectColor(ggColorWheelWidget* aColorWheelWidget, ggColorChannelWidget* aColorChannelWidget) const
{
  connect(aColorChannelWidget, SIGNAL(ColorChanged(QColor)), aColorWheelWidget, SLOT(SetColor(QColor)));
  connect(aColorWheelWidget, SIGNAL(ColorChanged(QColor)), aColorChannelWidget, SLOT(SetColor(QColor)));
}


void ggClassyMainWindow::ConnectColor(ggColorChannelWidget* aColorChannelWidget, ggColorWheelWidget* aColorWheelWidget) const
{
  connect(aColorChannelWidget, SIGNAL(ColorChanged(QColor)), aColorWheelWidget, SLOT(SetColor(QColor)));
  connect(aColorWheelWidget, SIGNAL(ColorChanged(QColor)), aColorChannelWidget, SLOT(SetColor(QColor)));
}


void ggClassyMainWindow::ConnectColor(ggColorChannelWidget* aColorChannelWidgetA, ggColorChannelWidget* aColorChannelWidgetB) const
{
  connect(aColorChannelWidgetA, SIGNAL(ColorChanged(QColor)), aColorChannelWidgetB, SLOT(SetColor(QColor)));
  connect(aColorChannelWidgetB, SIGNAL(ColorChanged(QColor)), aColorChannelWidgetA, SLOT(SetColor(QColor)));
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
  if (vScene != nullptr) vScene->MoveSelectedItemsTop();
}


void ggClassyMainWindow::MoveSelectedItemsBottom()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedItemsBottom();
}


void ggClassyMainWindow::MoveSelectedItemsUp()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedItemsUp();
}


void ggClassyMainWindow::MoveSelectedItemsDown()
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) vScene->MoveSelectedItemsDown();
}


void ggClassyMainWindow::ChangeColor(const QColor& aColor)
{
  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {

    // the selected collections (compile them in a set in order to avoid duplicates)
    std::set<ggClassyCollection*> vCollections;
    ggClassyDataSet* vDataSet = ggClassyApplication::GetInstance().GetDataSet();

    // get collections of the selected class boxes
    const ggClassyGraphicsScene::tClassBoxes& vSelectedClassBoxes = vScene->GetSelectedClassBoxes();
    ggWalkerT<ggClassyGraphicsScene::tClassBoxes::const_iterator> vSelectedClassBoxesWalker(vSelectedClassBoxes);
    while (vSelectedClassBoxesWalker) {
      const ggClassyClassBox* vSelectedClassBox = *vSelectedClassBoxesWalker;
      vCollections.insert(vDataSet->FindCollectionFromClass(vSelectedClassBox->GetClassName()));
    }

    // get collections of the selected frames
    const ggClassyGraphicsScene::tFrames& vSelectedFrames = vScene->GetSelectedFrames();
    ggWalkerT<ggClassyGraphicsScene::tFrames::const_iterator> vSelectedFramesWalker(vSelectedFrames);
    while (vSelectedFramesWalker) {
      const ggClassyFrame* vSelectedFrame = *vSelectedFramesWalker;
      vCollections.insert(vDataSet->GetCollections().FindCollection(vSelectedFrame->GetCollectionName()));
    }

    // modify the collection(s)
    ggWalkerT<std::set<ggClassyCollection*>::iterator> vCollectionsWalker(vCollections);
    while (vCollectionsWalker) {
      ggClassyCollection* vCollection = *vCollectionsWalker;
      if (vCollection != nullptr) {
        vCollection->mBoxBorder.setColor(ggUtilityQt::GetColorWithAlpha(aColor, 0.15f));
        vCollection->mConnectionLines.setColor(ggUtilityQt::GetColorWithAlpha(aColor, 1.0f));
        vCollection->mNameColor = ggUtilityQt::GetColorContrast(aColor);
        vCollection->mNameBackground.setColor(aColor);
        vCollection->mMembersBackground.setColor(ggUtilityQt::GetColorWithLightness(aColor, 0.97f));
        vCollection->mDescriptionBackground.setColor(ggUtilityQt::GetColorWithLightness(aColor, 0.90f));
        vCollection->Notify();
      }
    }
  }
}


void ggClassyMainWindow::CreateAction(QAction*& aAction, const char* aMethod, QToolButton* aToolButton)
{
  aAction = new QAction("x", this);
  aAction->setCheckable(true);
  connect(aAction, SIGNAL(triggered()), this, aMethod);
  aToolButton->setDefaultAction(aAction);
  aToolButton->setAutoRaise(true);
}


void ggClassyMainWindow::SetAlignment(Qt::Alignment aAlignment)
{
  mActionAlignTL->setChecked(aAlignment == (Qt::AlignTop | Qt::AlignLeft));
  mActionAlignTC->setChecked(aAlignment == (Qt::AlignTop | Qt::AlignHCenter));
  mActionAlignTR->setChecked(aAlignment == (Qt::AlignTop | Qt::AlignRight));
  mActionAlignCL->setChecked(aAlignment == (Qt::AlignVCenter | Qt::AlignLeft));
  mActionAlignCC->setChecked(aAlignment == (Qt::AlignVCenter | Qt::AlignHCenter));
  mActionAlignCR->setChecked(aAlignment == (Qt::AlignVCenter | Qt::AlignRight));
  mActionAlignBL->setChecked(aAlignment == (Qt::AlignBottom | Qt::AlignLeft));
  mActionAlignBC->setChecked(aAlignment == (Qt::AlignBottom | Qt::AlignHCenter));
  mActionAlignBR->setChecked(aAlignment == (Qt::AlignBottom | Qt::AlignRight));

  ggClassyGraphicsScene* vScene = dynamic_cast<ggClassyGraphicsScene*>(ui->mGraphicsView->scene());
  if (vScene != nullptr) {
    ggClassyGraphicsScene::tFrames vSelectedFrames = vScene->GetSelectedFrames();
    ggWalkerT<ggClassyGraphicsScene::tFrames::iterator> vSelectedFramesWalker(vSelectedFrames);
    while (vSelectedFramesWalker) {
      const ggClassyFrame* vFrame = *vSelectedFramesWalker;
      const_cast<ggClassyFrame*>(vFrame)->SetAlignment(aAlignment);
    }
  }
}


void ggClassyMainWindow::AlignTL()
{
  SetAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void ggClassyMainWindow::AlignTC()
{
  SetAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void ggClassyMainWindow::AlignTR()
{
  SetAlignment(Qt::AlignTop | Qt::AlignRight);
}

void ggClassyMainWindow::AlignCL()
{
  SetAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void ggClassyMainWindow::AlignCC()
{
  SetAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

void ggClassyMainWindow::AlignCR()
{
  SetAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void ggClassyMainWindow::AlignBL()
{
  SetAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

void ggClassyMainWindow::AlignBC()
{
  SetAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void ggClassyMainWindow::AlignBR()
{
  SetAlignment(Qt::AlignBottom | Qt::AlignRight);
}
