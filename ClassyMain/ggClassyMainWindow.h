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
  void on_mNewClassPushButton_clicked();
  void on_mNewClassBoxPushButton_clicked();
  void on_mDeleteClassPushButton_clicked();
  void on_mDeleteClassBoxPushButton_clicked();
  void MoveSelectedItemsTop();
  void MoveSelectedItemsUp();
  void MoveSelectedItemsDown();
  void MoveSelectedItemsBottom();
  void OpenDataSet();
  void SaveDataSetAs();
  void ChangeColor(const QColor& aColor);

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

};

#endif // GGCLASSYMAINWINDOW_H
