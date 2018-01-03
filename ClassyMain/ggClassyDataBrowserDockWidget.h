#ifndef GGCLASSYDATABROWSERDOCKWIDGET_H
#define GGCLASSYDATABROWSERDOCKWIDGET_H

// 1) include system or QT
#include <QDockWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

namespace Ui {
  class ggClassyDataBrowserDockWidget;
}

class ggClassyDataBrowserDockWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit ggClassyDataBrowserDockWidget(QWidget* aParent = nullptr);
  ~ggClassyDataBrowserDockWidget();

protected slots:

  void on_mNewPushButton_clicked();
  void on_mDelPushButton_clicked();

private:
  Ui::ggClassyDataBrowserDockWidget *ui;
};

#endif // GGCLASSYDATABROWSERDOCKWIDGET_H
