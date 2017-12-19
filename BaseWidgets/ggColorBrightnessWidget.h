#ifndef GGCOLORBRIGHTNESSWIDGET_H
#define GGCOLORBRIGHTNESSWIDGET_H

// 1) include system or QT
#include <QWidget>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggColorBrightnessWidget :
  public QWidget
{
  Q_OBJECT
public:

  explicit ggColorBrightnessWidget(QWidget* aParent = nullptr);

  QColor GetColor() const;

public slots:

  void SetColor(const QColor& aColor);

protected:

  virtual void mousePressEvent(QMouseEvent* aEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) override;
  virtual void mouseMoveEvent(QMouseEvent* aEvent) override;
  virtual void resizeEvent(QResizeEvent* aEvent) override;
  virtual void paintEvent(QPaintEvent* aEvent) override;

private:

  QColor mColorSaturized;
  float mColorBrightness;

};

#endif // GGCOLORBRIGHTNESSWIDGET_H
