#ifndef GGCLASSYCLASSBOX_H
#define GGCLASSYCLASSBOX_H

// 1) include system or QT
#include <QDomElement>
#include <QPointF>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassBox class
 */
class ggClassyClassBox :
  public ggSubject
{
public:

  ggClassyClassBox(const QString& aClassName);

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  // main members
  const QString& GetClassName() const;
  void SetClassName(const QString& aClassName);
  const QPointF& GetPosition() const;
  void SetPosition(const QPointF& aPosition);
  float GetWidth() const;
  void SetWidth(float aWidth);
  bool GetMembersVisible() const;
  void SetMembersVisible(bool aVisible);
  bool GetDescriptionVisible() const;
  void SetDescriptionVisible(bool aVisible);
  int GetIndexZ() const;
  void SetIndexZ(int aIndexZ);

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyClassBox* Create(const QDomElement& aElement);

private:

  QString mClassName;
  QPointF mPosition;
  float mWidth;
  bool mMembersVisible;
  bool mDescriptionVisible;
  int mIndexZ;

};

#endif // GGCLASSYCLASSBOX_H
