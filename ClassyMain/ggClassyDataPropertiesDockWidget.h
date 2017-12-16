#ifndef GGCLASSYDATAPROPERTIESDOCKWIDGET_H
#define GGCLASSYDATAPROPERTIESDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
  class ggClassyDataPropertiesDockWidget;
}

class ggClassyDataPropertiesDockWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit ggClassyDataPropertiesDockWidget(QWidget *parent = 0);
  ~ggClassyDataPropertiesDockWidget();

private:
  Ui::ggClassyDataPropertiesDockWidget *ui;
};

#endif // GGCLASSYDATAPROPERTIESDOCKWIDGET_H
