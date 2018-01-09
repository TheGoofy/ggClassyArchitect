#ifndef GGCLASSYSETTINGSDOCKWIDGET_H
#define GGCLASSYSETTINGSDOCKWIDGET_H

// 1) include system or QT
#include <QDockWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations
namespace Ui { class ggClassySettingsDockWidget; }

class ggClassySettingsDockWidget :
  public QDockWidget
{
  Q_OBJECT

public:
  explicit ggClassySettingsDockWidget(QWidget* aParent = nullptr);
  virtual ~ggClassySettingsDockWidget();

private:
  Ui::ggClassySettingsDockWidget *ui;
};

#endif // GGCLASSYSETTINGSDOCKWIDGET_H
