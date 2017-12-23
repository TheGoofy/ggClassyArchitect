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
  explicit ggClassyDataBrowserDockWidget(QWidget *parent = 0);
  ~ggClassyDataBrowserDockWidget();

private:
  Ui::ggClassyDataBrowserDockWidget *ui;
};

#endif // GGCLASSYDATABROWSERDOCKWIDGET_H
