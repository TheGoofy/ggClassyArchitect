#ifndef GGCLASSYDATABROWSERDOCKWIDGET_H
#define GGCLASSYDATABROWSERDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
  class ggClassyDataBrowserDockWidget;
}

class ggClassyDataBrowserDockWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit ggClassyDataBrowserDockWidget(QWidget *parent = 0);
  ~ggClassyDataBrowserDockWidget();

private:
  Ui::ggClassyDataBrowserDockWidget *ui;
};

#endif // GGCLASSYDATABROWSERDOCKWIDGET_H
