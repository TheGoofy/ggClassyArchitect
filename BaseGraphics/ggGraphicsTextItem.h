#ifndef GGGRAPHICSITEMTEXT_H
#define GGGRAPHICSITEMTEXT_H

// 1) include system or QT
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations
class QMimeData;
class ggSubject;

/**
 * @brief The ggGraphicsTextItem class
 */
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
  const ggSubject* GetSubjectEditingFinished() const;

protected:

  void keyPressEvent(QKeyEvent* aEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;
  void dropEvent(QGraphicsSceneDragDropEvent* aEvent) override;
  void contextMenuEvent(QGraphicsSceneContextMenuEvent* aEvent) override;
  void focusInEvent(QFocusEvent* aEvent) override;
  void focusOutEvent(QFocusEvent* aEvent) override;

  void paint(QPainter* aPainter, const QStyleOptionGraphicsItem*, QWidget*) override;

private slots:

  void on_document_contentChanged();

private:

  QMimeData* FormatMimeData(const QMimeData* aMimeData) const;
  void SetEditable(bool aEditable = true);

  ggSubject* mSubjectText;
  ggSubject* mSubjectEditingFinished;
  bool mSuppressLineBreaks;
  bool mEnterKeyFinishesEdit;
  QString mHtmlBackup;
  QPointF mLastMousePressPos;
  QBrush mBrush;
  QPen mPen;

};

#endif // GGGRAPHICSITEMTEXT_H
