#ifndef GGCLASSYMAINWINDOW_H
#define GGCLASSYMAINWINDOW_H

// 1) include system or QT
#include <QMainWindow>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"

// 3) forward declarations
namespace Ui {
  class ggClassyMainWindow;
}

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
  void MoveSelectedItemsTop();
  void MoveSelectedItemsUp();
  void MoveSelectedItemsDown();
  void MoveSelectedItemsBottom();
  void OpenDataSet();
  void SaveDataSetAs();

protected:
  virtual void Update(const ggSubject* aSubject) override;

private: 
  static QPointF& GetNewPosition();

  // the main window form
  Ui::ggClassyMainWindow *ui;

  QString ZoomToString(float aZoomFloat) const;
  float StringToZoom(const QString& aZoomString) const;

};

#endif // GGCLASSYMAINWINDOW_H
