// 0) include own header
#include "ggColorWheelWidget.h"

// 1) include system or QT
#include <math.h>
#include <QPainter>
#include <QMouseEvent>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"
#include "BaseWidgets//ggUtilityQt.h"


ggColorWheelWidget::ggColorWheelWidget(QWidget* aParent) :
  QWidget(aParent),
  mAdaptBrightness(false),
  mColorBrightness(0.0f),
  mSelectorRadius(3.0f),
  mMouseDragging(false)
{
  setMouseTracking(true);
  SetColor(QColor(200, 150, 50, 255));
  UpdateCornerColors();
}


void ggColorWheelWidget::SetColor(const QColor& aColor)
{
  // calculate saturized color and brightness
  float vColorBrightness = ggUtilityQt::GetColorBrightness(aColor);
  QColor vColorSaturized = ggUtilityQt::GetColorSaturized(aColor);

  // update colors, if changed
  if (vColorBrightness != mColorBrightness ||
      vColorSaturized != mColorSaturized) {
    mColorSaturized = vColorSaturized;
    mColorBrightness = vColorBrightness;
    mColorPosition = GetPosition(mColorSaturized);
    UpdateCornerColors();
    update(); // trigger repaint
  }
}


QColor ggColorWheelWidget::GetColor() const
{
  return ggUtilityQt::GetColorScaled(mColorSaturized, mColorBrightness);
}


void ggColorWheelWidget::SetAdaptBrightness(bool aAdapt)
{
  if (mAdaptBrightness != aAdapt) {
    mAdaptBrightness = aAdapt;
    UpdateCornerColors();
    update(); // trigger repaint
  }
}


bool ggColorWheelWidget::GetAdaptBrightness() const
{
  return mAdaptBrightness;
}


void ggColorWheelWidget::UpdateCornerColors()
{
  float vVal = mAdaptBrightness ? mColorBrightness : 1.0f;
  mColorR = QColor::fromRgbF(vVal, 0.0f, 0.0f); // red
  mColorY = QColor::fromRgbF(vVal, vVal, 0.0f); // yellow
  mColorG = QColor::fromRgbF(0.0f, vVal, 0.0f); // green
  mColorC = QColor::fromRgbF(0.0f, vVal, vVal); // cyan
  mColorB = QColor::fromRgbF(0.0f, 0.0f, vVal); // blue
  mColorM = QColor::fromRgbF(vVal, 0.0f, vVal); // magenta
}


QPointF GetPosition(float aScale, float aColorA, float aColorB,
                    const QPointF& aPosition,
                    const QVector2D& aDirectionA,
                    const QVector2D& aDirectionB)
{
  if (aScale != 0.0f) {
    float vFactorA = 1.0f - aColorA / aScale;
    float vFactorB = 1.0f - aColorB / aScale;
    return aPosition - (vFactorA * aDirectionA).toPointF()
                     - (vFactorB * aDirectionB).toPointF();
  }
  else {
    return aPosition;
  }
}


QPointF ggColorWheelWidget::GetPosition(const QColor& aColor) const
{
  if (ggUtilityQt::GetColorMax(aColor) == aColor.red()) {
    return ::GetPosition(aColor.redF(), aColor.greenF(), aColor.blueF(), mPosCenter, mDirG, mDirB);
  }
  if (ggUtilityQt::GetColorMax(aColor) == aColor.green()) {
    return ::GetPosition(aColor.greenF(), aColor.blueF(), aColor.redF(), mPosCenter, mDirB, mDirR);
  }
  if (ggUtilityQt::GetColorMax(aColor) == aColor.blue()) {
    return ::GetPosition(aColor.blueF(), aColor.redF(), aColor.greenF(), mPosCenter, mDirR, mDirG);
  }
  return QPointF(mPosCenter);
}


void GetFactors(const QVector2D& aDirectionA,
                const QVector2D& aDirectionB,
                const QVector2D& aPosition,
                float& aFactorA,
                float& aFactorB)
{
  float vArea = aDirectionA.x() * aDirectionB.y() - aDirectionA.y() * aDirectionB.x();
  if (vArea != 0.0f) {
    aFactorA = (aDirectionB.y() * aPosition.x() - aDirectionB.x() * aPosition.y()) / vArea;
    aFactorB = (aDirectionA.x() * aPosition.y() - aDirectionA.y() * aPosition.x()) / vArea;
  }
}


float SignFromLine(const QPointF& aLineOrigin,
                   const QVector2D& aLineDirP1,
                   const QVector2D& aLineDirP2,
                   const QPointF& aPosition)
{
  QVector2D vLineDirection(aLineDirP2 - aLineDirP1);
  QVector2D vLineDirectionN(-vLineDirection.y(), vLineDirection.x());
  QVector2D vDirectionP(aPosition - aLineOrigin - aLineDirP1.toPointF());
  return QVector2D::dotProduct(vDirectionP, vLineDirectionN);
}


QPointF ClampPosition(const QPointF& aPosition,
                      const QPointF& aCenter,
                      const QVector2D& aDirA,
                      const QVector2D& aDirB)
{
  if (SignFromLine(aCenter, aDirA, aDirB, aPosition) > 0.0f) {
    if (SignFromLine(aCenter, QVector2D(), aDirA, aPosition) <= 0.0f) {
      if (SignFromLine(aCenter, QVector2D(), aDirB, aPosition) >= 0.0f) {
        QVector2D vDirAB(aDirB - aDirA);
        float vLengthAB = vDirAB.length();
        if (vLengthAB != 0.0f) vDirAB /= vLengthAB;
        QVector2D vDirAP(aPosition - aCenter - aDirA.toPointF());
        float vScaleAB = QVector2D::dotProduct(vDirAB, vDirAP);
        vScaleAB = ggUtility::Clamp(vScaleAB, 0.0f, vLengthAB);
        return aCenter + (aDirA + vScaleAB * vDirAB).toPointF();
      }
    }
  }
  return aPosition;
}


QPointF ggColorWheelWidget::ClampPosition(const QPointF& aPosition) const
{
  QPointF vPosition(aPosition);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirR, mDirY);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirY, mDirG);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirG, mDirC);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirC, mDirB);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirB, mDirM);
  vPosition = ::ClampPosition(vPosition, mPosCenter, mDirM, mDirR);
  return vPosition;
}


QColor ggColorWheelWidget::GetColorSaturized(const QPointF& aPosition) const
{
  QVector2D vDir(ClampPosition(aPosition) - mPosCenter);
  float vScaleR1 = 0.0f; float vScaleR2 = 0.0f;
  float vScaleG1 = 0.0f; float vScaleG2 = 0.0f;
  float vScaleB1 = 0.0f; float vScaleB2 = 0.0f;
  GetFactors(mDirR, mDirG, vDir, vScaleR1, vScaleG2);
  GetFactors(mDirG, mDirB, vDir, vScaleG1, vScaleB2);
  GetFactors(mDirB, mDirR, vDir, vScaleB1, vScaleR2);
  float vColorR = 1.0f + ggUtility::Min(vScaleR1, vScaleR2);
  float vColorG = 1.0f + ggUtility::Min(vScaleG1, vScaleG2);
  float vColorB = 1.0f + ggUtility::Min(vScaleB1, vScaleB2);
  return QColor::fromRgbF(ggUtility::Clamp(vColorR, 0.0f, 1.0f),
                          ggUtility::Clamp(vColorG, 0.0f, 1.0f),
                          ggUtility::Clamp(vColorB, 0.0f, 1.0f),
                          mColorSaturized.alphaF());
}


QColor ggColorWheelWidget::GetColorFromWheel(const QPointF& aPosition) const
{
  QColor vColor = GetColorSaturized(aPosition);
  if (!mAdaptBrightness) return vColor;
  return ggUtilityQt::GetColorScaled(vColor, mColorBrightness);
}


bool ggColorWheelWidget::IsInside(const QPointF& aPosition) const
{
  bool vIsInside = true;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirR, mDirY, aPosition) <= 0.0f;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirY, mDirG, aPosition) <= 0.0f;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirG, mDirC, aPosition) <= 0.0f;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirC, mDirB, aPosition) <= 0.0f;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirB, mDirM, aPosition) <= 0.0f;
  vIsInside = vIsInside && SignFromLine(mPosCenter, mDirM, mDirR, aPosition) <= 0.0f;
  return vIsInside;
}


void ggColorWheelWidget::mousePressEvent(QMouseEvent* aEvent)
{
  // change selected color
  if (aEvent->button() == Qt::LeftButton) {
    mMouseDragging = true;
    setCursor(Qt::BlankCursor);
    mColorSaturized = GetColorSaturized(aEvent->pos());
    mColorPosition = GetPosition(mColorSaturized);
    emit ColorChanged(GetColor());
    update();
  }

  // call base method
  QWidget::mousePressEvent(aEvent);
}


void ggColorWheelWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
  // change selected color
  if (aEvent->button() == Qt::LeftButton) {
    mMouseDragging = false;
    setCursor(IsInside(aEvent->pos()) ? Qt::CrossCursor : Qt::ArrowCursor);
    update();
  }

  // call base method
  QWidget::mouseReleaseEvent(aEvent);
}


void ggColorWheelWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  // adjust mouse pointer, if it is inside of the color-wheel
  if (!mMouseDragging) {
    setCursor(IsInside(aEvent->pos()) ? Qt::CrossCursor : Qt::ArrowCursor);
  }

  // change selected color
  if (aEvent->buttons() & Qt::LeftButton) {
    mColorSaturized = GetColorSaturized(aEvent->pos());
    mColorPosition = GetPosition(mColorSaturized);
    emit ColorChanged(GetColor());
    update();
  }

  // call base function
  QWidget::mouseMoveEvent(aEvent);
}


void ggColorWheelWidget::resizeEvent(QResizeEvent* aEvent)
{
  // calculate half width, half height, and center
  float vSizeXH = aEvent->size().width() / 2.0f;
  float vSizeYH = aEvent->size().height() / 2.0f;
  float vPosCenterX = rect().x() + vSizeXH;
  float vPosCenterY = rect().y() + vSizeYH;

  // some space for the selector
  vSizeXH -= mSelectorRadius;
  vSizeYH -= mSelectorRadius;

  // fix aspect ratio with the height of an equilateral triangle
  float vRatio = sqrt(3.0f) / 2.0f;
  vSizeXH = vSizeYH / vRatio < vSizeXH ? vSizeYH / vRatio : vSizeXH;
  vSizeYH = vSizeXH * vRatio < vSizeYH ? vSizeXH * vRatio : vSizeYH;

  // corner positions
  mPosCenter = QPointF(vPosCenterX, vPosCenterY);
  mDirR = QVector2D(      vSizeXH,     0.0f); // red corner
  mDirY = QVector2D( vSizeXH/2.0f, -vSizeYH); // yellow corner
  mDirG = QVector2D(-vSizeXH/2.0f, -vSizeYH); // green corner
  mDirC = QVector2D(     -vSizeXH,     0.0f); // cyan corner
  mDirB = QVector2D(-vSizeXH/2.0f,  vSizeYH); // blue corner
  mDirM = QVector2D( vSizeXH/2.0f,  vSizeYH); // magenta corner

  // position of the selected color
  mColorPosition = GetPosition(mColorSaturized);

  // call base class
  QWidget::resizeEvent(aEvent);
}


void DrawPolygon(QPainter& aPainter,
                 const QPointF aPosition,
                 const QVector2D& aDirectionA,
                 const QVector2D& aDirectionB,
                 const QVector2D& aDirectionC,
                 const QVector2D& aDirectionD,
                 const QVector2D& aDirectionE,
                 const QVector2D& aDirectionF)
{
  QPolygonF vPolygon;
  vPolygon << aPosition + aDirectionA.toPointF();
  vPolygon << aPosition + aDirectionB.toPointF();
  vPolygon << aPosition + aDirectionC.toPointF();
  vPolygon << aPosition + aDirectionD.toPointF();
  vPolygon << aPosition + aDirectionE.toPointF();
  vPolygon << aPosition + aDirectionF.toPointF();
  aPainter.drawPolygon(vPolygon);
}


void DrawRhomb(QPainter& aPainter,
               const QPointF& aPosition,
               const QVector2D& aDirectionA,
               const QVector2D& aDirectionB)
{
  QPolygonF vPolygon;
  vPolygon << aPosition;
  vPolygon << aPosition + aDirectionA.toPointF();
  vPolygon << aPosition + aDirectionA.toPointF() + aDirectionB.toPointF();
  vPolygon << aPosition + aDirectionB.toPointF();
  aPainter.drawPolygon(vPolygon);
}


QBrush GradientBrush(const QPointF& aPosition,
                     const QVector2D& aDirectionA,
                     const QVector2D& aDirectionB,
                     const QColor& aColorA,
                     const QColor& aColorB)
{
  // perpendicular direction (normal)
  QVector2D vDirectionAN(-aDirectionA.y(), aDirectionA.x());
  vDirectionAN.normalize();

  // project "B" perpendicular to "A"
  float vScaleAB = QVector2D::dotProduct(vDirectionAN, aDirectionB);
  QVector2D vDirectionBN(vScaleAB * vDirectionAN);

  // setup gradient
  QLinearGradient vGradient(aPosition, aPosition + vDirectionBN.toPointF());
  vGradient.setColorAt(0.0f, aColorA);
  vGradient.setColorAt(1.0f, aColorB);

  // return the gratent brush
  return QBrush(vGradient);
}


void DrawGradientRhomb(QPainter& aPainter,
                       const QPointF& aPosition,
                       const QVector2D& aDirectionA,
                       const QVector2D& aDirectionB,
                       const QColor& aColorA,
                       const QColor& aColorB,
                       const QColor& aColorC)
{
  // backup some painter settings
  QBrush vBrush(aPainter.brush());
  QPainter::CompositionMode vCompositionMode(aPainter.compositionMode());

  // paint rhomb  with default mode (paints background)
  aPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  aPainter.setBrush(GradientBrush(aPosition, aDirectionA, aDirectionB, aColorA, aColorC));
  DrawRhomb(aPainter, aPosition, aDirectionA, aDirectionB);

  // paint rhomb and use brighter color component (on top of background)
  aPainter.setCompositionMode(QPainter::CompositionMode_Lighten);
  aPainter.setBrush(GradientBrush(aPosition, aDirectionB, aDirectionA, aColorB, aColorC));
  DrawRhomb(aPainter, aPosition, aDirectionB, aDirectionA);

  // restore painter settings
  aPainter.setCompositionMode(vCompositionMode);
  aPainter.setBrush(vBrush);
}


void DrawLine(QPainter& aPainter,
              const QPointF& aPosition,
              const QVector2D& aDirection,
              float aOffset,
              float aLength)
{
  QVector2D vDirection(aDirection.normalized());
  aPainter.drawLine(aPosition + (aOffset*vDirection).toPointF(),
                    aPosition + ((aOffset+aLength)*vDirection).toPointF());
}


void ggColorWheelWidget::paintEvent(QPaintEvent* aEvent)
{
  // painter
  QPainter vPainter(this);

  // draw corners, turn off antialiasing (there are artifacts)
  vPainter.setRenderHint(QPainter::Antialiasing, false);
  vPainter.setPen(Qt::NoPen);
  DrawGradientRhomb(vPainter, mPosCenter, mDirY, mDirM, mColorY, mColorM, mColorR); // red corner
  DrawGradientRhomb(vPainter, mPosCenter, mDirC, mDirY, mColorC, mColorY, mColorG); // green corner
  DrawGradientRhomb(vPainter, mPosCenter, mDirM, mDirC, mColorM, mColorC, mColorB); // blue corner

  // draw antialiased outline with window color (smooth the stairs)
  vPainter.setRenderHint(QPainter::Antialiasing, true);
  vPainter.setPen(palette().window().color());
  vPainter.setBrush(Qt::NoBrush);
  DrawPolygon(vPainter, mPosCenter, mDirR, mDirY, mDirG, mDirC, mDirB, mDirM);

  // indicator of selected color
  const float vSelectorRadiusLarge = 9.0f;
  const float vRadius = mMouseDragging ? vSelectorRadiusLarge : mSelectorRadius;
  vPainter.setPen(QPen(Qt::white, 1.5f));
  vPainter.drawEllipse(mColorPosition, vRadius + 1.0f, vRadius + 1.0f);
  vPainter.setPen(Qt::black);
  vPainter.setBrush(GetColor());
  vPainter.drawEllipse(mColorPosition, vRadius, vRadius);

  // center crosshair
  vPainter.setPen(ggUtilityQt::GetContrastColor(GetColorFromWheel(mPosCenter)));
  float vOffset = mSelectorRadius;
  float vLength = vSelectorRadiusLarge - mSelectorRadius - 2.5f;
  DrawLine(vPainter, mPosCenter, mDirR, vOffset, vLength);
  DrawLine(vPainter, mPosCenter, mDirG, vOffset, vLength);
  DrawLine(vPainter, mPosCenter, mDirB, vOffset, vLength);

  // maybe the parent wants to draw as well...
  QWidget::paintEvent(aEvent);
}
