// 0) include own header
#include "ggGraphicsTextItem.h"

// 1) include system or QT
#include <QMimeData>
#include <QClipboard>
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <QTextCursor>
#include <QTextDocument>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

// 2) include own project-related (sort by component dependency)

// goofy: eliminate that include (need clipboard in order to re-format mime-data)
#include "ClassyMain/ggClassyApplication.h"


ggGraphicsTextItem::ggGraphicsTextItem(QGraphicsItem* aParent) :
  QGraphicsTextItem(aParent),
  mSubjectText(new ggSubject()),
  mSubjectEditingFinished(new ggSubject()),
  mSuppressRichText(true),
  mSuppressLineBreaks(false),
  mEnterKeyFinishesEdit(false),
  mLastMousePressPos(0.0f, 0.0f),
  mBrush(Qt::NoBrush)
{
  document()->setDocumentMargin(3.0f);
  setToolTip("Click twice in order to edit the text.");
  SetEditable(false);
  connect(document(), SIGNAL(contentsChanged()),
          this, SLOT(on_document_contentChanged()));
}


ggGraphicsTextItem::~ggGraphicsTextItem()
{
  delete mSubjectText;
  delete mSubjectEditingFinished;
}


void ggGraphicsTextItem::dropEvent(QGraphicsSceneDragDropEvent* aEvent)
{
  // accept only plain text: adjust mime-data
  if (mSuppressRichText) {
    aEvent->setMimeData(StripMimeData(aEvent->mimeData()));
  }

  // do inherited event handling
  QGraphicsTextItem::dropEvent(aEvent);
}


void ggGraphicsTextItem::keyPressEvent(QKeyEvent* aEvent)
{
  // accept only plain text: adjust mime-data
  if (aEvent->matches(QKeySequence::Paste) && mSuppressRichText) {
    QClipboard* vClipboard = ggClassyApplication::GetInstance().clipboard();
    vClipboard->setMimeData(StripMimeData(vClipboard->mimeData()));
  }

  // interpret <return> key ...
  if ((aEvent->key() == Qt::Key_Return) ||
      (aEvent->key() == Qt::Key_Enter)) {

    // finish editing unless the shift key is pressed
    if (GetEnterKeyFinishesEdit()) {
      if (!(aEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))) {
        clearFocus();
        aEvent->accept();
        return;
      }
    }

    // don't do anything, if line breaks are suppressed
    if (GetSuppressLineBreaks()) {
      aEvent->accept();
      return;
    }
  }

  // interpret the escape key
  if (aEvent->key() == Qt::Key_Escape) {
    setHtml(mHtmlBackup);
    clearFocus();
    aEvent->accept();
  }

  // do inherited event handling
  QGraphicsTextItem::keyPressEvent(aEvent);
}


void ggGraphicsTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* aEvent)
{
  // accept only plain text: adjust mime-data
  if (mSuppressRichText) {
    QClipboard* vClipboard = ggClassyApplication::GetInstance().clipboard();
    vClipboard->setMimeData(StripMimeData(vClipboard->mimeData()));
  }

  // do inherited event handling
  QGraphicsTextItem::contextMenuEvent(aEvent);

  // the base event steals the focus. force it back
  SetEditable(true);
}


void ggGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* aEvent)
{
  // make it editable, if user clicks twice to the same position
  QPointF vPos = aEvent->pos();
  if (vPos == mLastMousePressPos) SetEditable(true);
  mLastMousePressPos = vPos;

  // do inherited event handling
  QGraphicsTextItem::mousePressEvent(aEvent);
}


void ggGraphicsTextItem::focusInEvent(QFocusEvent* aEvent)
{
  // remember previous text, in case user aborts editing with <esc>
  mHtmlBackup = toHtml();

  // do inherited event handling
  QGraphicsTextItem::focusOutEvent(aEvent);
}


void ggGraphicsTextItem::focusOutEvent(QFocusEvent* aEvent)
{
  // editing is finished now
  SetEditable(false);

  // do inherited event handling
  QGraphicsTextItem::focusOutEvent(aEvent);
}


void ggGraphicsTextItem::paint(QPainter* aPainter,
                               const QStyleOptionGraphicsItem* aStyleOption,
                               QWidget* aWidget)
{
  aPainter->setPen(mPen);
  aPainter->setBrush(mBrush);
  aPainter->drawRect(boundingRect());
  QGraphicsTextItem::paint(aPainter, aStyleOption, aWidget);
}


QMimeData* ggGraphicsTextItem::StripMimeData(const QMimeData* aMimeData) const
{
  QString vText = aMimeData->text();
  if (GetSuppressLineBreaks()) vText = vText.simplified();
  QMimeData* vMimeData = new QMimeData();
  vMimeData->setText(vText);
  return vMimeData;
}


void ggGraphicsTextItem::SetBrush(const QBrush& aBrush)
{
  if (mBrush != aBrush) {
    mBrush = aBrush;
    update();
  }
}


const QBrush& ggGraphicsTextItem::Brush() const
{
  return mBrush;
}


void ggGraphicsTextItem::SetPen(const QPen& aPen)
{
  if (mPen != aPen) {
    mPen = aPen;
    update();
  }
}


const QPen& ggGraphicsTextItem::Pen() const
{
  return mPen;
}


void ggGraphicsTextItem::SetSuppressRichText(bool aSuppressRichText)
{
  if (mSuppressRichText != aSuppressRichText) {
    mSuppressRichText = aSuppressRichText;
    update();
  }
}


bool ggGraphicsTextItem::GetSuppressRichText() const
{
  return mSuppressRichText;
}


void ggGraphicsTextItem::SetSuppressLineBreaks(bool aSuppressLineBreaks)
{
  if (mSuppressLineBreaks != aSuppressLineBreaks) {
    mSuppressLineBreaks = aSuppressLineBreaks;
    update();
  }
}


bool ggGraphicsTextItem::GetSuppressLineBreaks() const
{
  return mSuppressLineBreaks;
}


void ggGraphicsTextItem::SetEnterKeyFinishesEdit(bool aEnterKeyFinishesEdit)
{
  if (mEnterKeyFinishesEdit != aEnterKeyFinishesEdit) {
    mEnterKeyFinishesEdit = aEnterKeyFinishesEdit;
    update();
  }
}


bool ggGraphicsTextItem::GetEnterKeyFinishesEdit() const
{
  return mEnterKeyFinishesEdit;
}


void ggGraphicsTextItem::SetEditable(bool aEditable)
{
  if (aEditable) {
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setCursor(Qt::IBeamCursor);
    setFocus();
  }
  else {
    mSubjectEditingFinished->Notify();
    setTextInteractionFlags(Qt::NoTextInteraction);
    unsetCursor();
  }
}


void ggGraphicsTextItem::SetText(const QString& aText)
{
  QSignalBlocker vSignalBlocker(document());
  setPlainText(aText);
}


QString ggGraphicsTextItem::GetText() const
{
  return toPlainText();
}


const ggSubject* ggGraphicsTextItem::GetSubjectText() const
{
  return mSubjectText;
}


const ggSubject* ggGraphicsTextItem::GetSubjectEditingFinished() const
{
  return mSubjectEditingFinished;
}


void ggGraphicsTextItem::SetAlignment(Qt::Alignment aAlignment)
{
  QTextOption vTextOption = document()->defaultTextOption();
  vTextOption.setAlignment(aAlignment);
  document()->setDefaultTextOption(vTextOption);
}


Qt::Alignment ggGraphicsTextItem::GetAlignment() const
{
  return document()->defaultTextOption().alignment();
}


void ggGraphicsTextItem::on_document_contentChanged()
{
  mSubjectText->Notify();
}
