#ifndef GGCLASSYMAINWINDOW_H
#define GGCLASSYMAINWINDOW_H

#include <QMainWindow>

#include "ggObserver.h"

namespace Ui {
  class ggClassyMainWindow;
}

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
  void on_mAddClassPushButton_clicked();
  void SaveDataSetAs();

protected:
  virtual void Update(const ggSubject* aSubject) override;

private:

  // the main window form
  Ui::ggClassyMainWindow *ui;

  QString ZoomToString(float aZoomFloat) const;
  float StringToZoom(const QString& aZoomString) const;

};

#endif // GGCLASSYMAINWINDOW_H
