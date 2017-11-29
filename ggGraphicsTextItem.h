#ifndef GGGRAPHICSITEMTEXT_H
#define GGGRAPHICSITEMTEXT_H

#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>

class QMimeData;
class ggSubject;

class ggGraphicsTextItem :
  public QGraphicsTextItem
{
  Q_OBJECT

public:

  ggGraphicsTextItem(QGraphicsItem* aParent = nullptr);
  virtual ~ggGraphicsTextItem();

  void SetSuppressLineBreaks(bool aSuppressLineBreaks);
  bool GetSuppressLineBreaks() const;
  void SetEnterKeyFinishesEdit(bool aEnterKeyFinishesEdit);
  bool GetEnterKeyFinishesEdit() const;

  void SetBrush(const QBrush& aBrush);
  const QBrush& Brush() const;
  void SetPen(const QPen& aPen);
  const QPen& Pen() const;

  void SetText(const QString& aText);
  QString GetText() const;
  const ggSubject* GetSubjectText() const;

protected:

  void keyPressEvent(QKeyEvent* aEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;
  void dropEvent(QGraphicsSceneDragDropEvent* aEvent) override;
  void contextMenuEvent(QGraphicsSceneContextMenuEvent* aEvent) override;
  void focusOutEvent(QFocusEvent* aEvent) override;

  void paint(QPainter* aPainter, const QStyleOptionGraphicsItem*, QWidget*) override;

private slots:

  void on_document_contentChanged();

private:

  QMimeData* FormatMimeData(const QMimeData* aMimeData) const;
  void SetEditable(bool aEditable = true);

  ggSubject* mSubjectText;
  bool mSuppressLineBreaks;
  bool mEnterKeyFinishesEdit;
  QPointF mLastMousePressPos;
  QBrush mBrush;
  QPen mPen;

};

#endif // GGGRAPHICSITEMTEXT_H
