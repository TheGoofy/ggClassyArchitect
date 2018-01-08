#ifndef GGCOLORDIALOG_H
#define GGCOLORDIALOG_H

#include <QDialog>

namespace Ui {
  class ggColorDialog;
}

class ggColorDialog :
  public QDialog
{
  Q_OBJECT

public:

  explicit ggColorDialog(QWidget* aParent = nullptr);
  virtual ~ggColorDialog();

  const QColor& currentColor() const;

signals:

  void currentColorChanged(const QColor& aColor);

public slots:

  void setCurrentColor(const QColor& aColor);

private slots:

  void on_mColorRSpinBox_valueChanged(int aValue);
  void on_mColorGSpinBox_valueChanged(int aValue);
  void on_mColorBSpinBox_valueChanged(int aValue);
  void on_mColorASpinBox_valueChanged(int aValue);

private:

  Ui::ggColorDialog* ui;
  QColor mColor;

};

#endif // GGCOLORDIALOG_H
