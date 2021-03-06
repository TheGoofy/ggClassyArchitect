#ifndef GGCLASSYMAINWINDOW_H
#define GGCLASSYMAINWINDOW_H

// 1) include system or QT
#include <QMainWindow>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"

// 3) forward declarations
namespace Ui { class ggClassyMainWindow; }
class ggClassySettingsDockWidget;
class ggClassyDataBrowserDockWidget;
class ggClassyDataPropertiesDockWidget;
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

signals:

  void ColorChanged(const QColor& aColor);

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
  void SetColor(const QColor& aColor);
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
  virtual void closeEvent(QCloseEvent* aEvent) override;

private: 

  // the main window form
  Ui::ggClassyMainWindow* ui;

  // other user interfaces
  ggClassySettingsDockWidget* mSettings;
  ggClassyDataBrowserDockWidget* mDataBrowser;
  ggClassyDataPropertiesDockWidget* mDataProperties;

  void WriteSettings();
  void ReadSettings();

  static QPointF& GetNewPosition();
  QString ZoomToString(float aZoomFloat) const;
  float StringToZoom(const QString& aZoomString) const;

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
