#ifndef GGCLASSYMAINWINDOW_H
#define GGCLASSYMAINWINDOW_H

// 1) include system or QT
#include <QMainWindow>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"
#include "ClassyMain/ggClassyDataBrowserDockWidget.h"
#include "ClassyMain/ggClassyDataPropertiesDockWidget.h"

// 3) forward declarations
namespace Ui { class ggClassyMainWindow; }
class ggColorWheelWidget;
class ggColorChannelWidget;
class QToolButton;

/**
 * @brief The ggClassyMainWindow class
 */
class ggClassyMainWindow :
  public QMainWindow,
  public ggObserver
{
  Q_OBJECT

public:
  explicit ggClassyMainWindow(QWidget* aParent = nullptr);
  virtual ~ggClassyMainWindow();
  void UpdateUI();

protected slots:
  void on_mZoomComboBox_editingFinished();
  void on_mZoomComboBox_activated(int);
  void on_mZoomFitPushButton_clicked();
  void on_mZoomResetPushButton_clicked();
  void on_mNewCollectionPushButton_clicked();
  void on_mNewClassPushButton_clicked();
  void on_mNewClassBoxPushButton_clicked();
  void on_mNewFramePushButton_clicked();
  void on_mDeleteCollectionPushButton_clicked();
  void on_mDeleteClassPushButton_clicked();
  void on_mDeleteClassBoxPushButton_clicked();
  void on_mDeleteConnectionPushButton_clicked();
  void on_mDeleteFramePushButton_clicked();
  void MoveSelectedItemsTop();
  void MoveSelectedItemsUp();
  void MoveSelectedItemsDown();
  void MoveSelectedItemsBottom();
  void OpenDataSet();
  void SaveDataSetAs();
  void ChangeColor(const QColor& aColor);
  void AlignTL();
  void AlignTC();
  void AlignTR();
  void AlignCL();
  void AlignCC();
  void AlignCR();
  void AlignBL();
  void AlignBC();
  void AlignBR();

protected:
  virtual void Update(const ggSubject* aSubject) override;

private: 

  // the main window form
  Ui::ggClassyMainWindow* ui;

  // other user interfaces
  ggClassyDataBrowserDockWidget* mDataBrowser;
  ggClassyDataPropertiesDockWidget* mDataProperties;

  static QPointF& GetNewPosition();
  QString ZoomToString(float aZoomFloat) const;
  float StringToZoom(const QString& aZoomString) const;
  void ConnectColor(ggColorWheelWidget* aColorWheelWidgetA, ggColorWheelWidget* aColorWheelWidgetB) const;
  void ConnectColor(ggColorWheelWidget* aColorWheelWidget, ggColorChannelWidget* aColorChannelWidget) const;
  void ConnectColor(ggColorChannelWidget* aColorChannelWidget, ggColorWheelWidget* aColorWheelWidget) const;
  void ConnectColor(ggColorChannelWidget* aColorChannelWidgetA, ggColorChannelWidget* aColorChannelWidgetB) const;

  void CreateAction(QAction*& aAction, const char* aSlotMethodName, QToolButton* aToolButton);
  void SetAlignment(Qt::Alignment aAlignment);

  QAction* mActionAlignTL;
  QAction* mActionAlignTC;
  QAction* mActionAlignTR;
  QAction* mActionAlignCL;
  QAction* mActionAlignCC;
  QAction* mActionAlignCR;
  QAction* mActionAlignBL;
  QAction* mActionAlignBC;
  QAction* mActionAlignBR;

};

#endif // GGCLASSYMAINWINDOW_H
