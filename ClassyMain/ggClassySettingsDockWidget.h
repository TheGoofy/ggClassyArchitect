#ifndef GGCLASSYSETTINGSDOCKWIDGET_H
#define GGCLASSYSETTINGSDOCKWIDGET_H

// 1) include system or QT
#include <QDockWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations
namespace Ui { class ggClassySettingsDockWidget; }
class ggColorPreviewWidget;

class ggClassySettingsDockWidget :
  public QDockWidget
{
  Q_OBJECT

public:

  explicit ggClassySettingsDockWidget(QWidget* aParent = nullptr);
  virtual ~ggClassySettingsDockWidget();

private slots:

  void SetColor(const QColor& aColor);
  void ColorAccept();
  void ColorReject();

  void on_mSelectionColorPreviewWidget_Clicked();
  void on_mHighlightColorPreviewWidget_Clicked();
  void on_mShadowColorPreviewWidget_Clicked();
  void on_mShadowWidthSpinBox_valueChanged(double aValue);
  void on_mShadowOffsetXSpinBox_valueChanged(double aValue);
  void on_mShadowOffsetYSpinBox_valueChanged(double aValue);

private:

  Ui::ggClassySettingsDockWidget* ui;

  void EditColor(ggColorPreviewWidget* aColorPreview);
  ggColorPreviewWidget* mEditedColorPreview;
  QColor mColorBackup;

};

#endif // GGCLASSYSETTINGSDOCKWIDGET_H
