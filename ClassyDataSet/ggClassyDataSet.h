#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

// 1) include system or QT
#include <QDomElement>

// 2) include own project-related (sort by component dependency)
#include "Base/ggTypes.h"
#include "ClassyDataSet/ggClassyClassContainer.h"
#include "ClassyDataSet/ggClassyCollectionContainer.h"
#include "ClassyDataSet/ggClassyClassBoxContainer.h"
#include "ClassyDataSet/ggClassyFrame.h"

// 3) forward declarations

/**
 * @brief The ggClassyDataSet class
 */
class ggClassyDataSet
{
public:

  static ggClassyDataSet* CreateTestDataSet();

  // construction / destruction
  ggClassyDataSet();
  ggClassyDataSet(const ggClassyDataSet& aOther);
  virtual ~ggClassyDataSet();

  // identification
  static const QString& TypeID();
  virtual const QString& VTypeID() const;
  QString GetFormatVersion() const;

  // assignment operator
  ggClassyDataSet& operator = (const ggClassyDataSet& aOther);

  // collections (of classes)
  ggClassyCollection* AddCollection(ggClassyCollection* aCollection);
  ggClassyCollection* RemoveCollection(const QString& aName);
  ggClassyCollectionContainer& GetCollections();
  const ggClassyCollectionContainer& GetCollections() const;
  ggClassyCollection* FindCollectionFromClass(const QString& aClassName);
  const ggClassyCollection* FindCollectionFromClass(const QString& aClassName) const;

  // classes
  ggClassyClass* AddClass(ggClassyClass* aClass);
  ggClassyClass* FindClass(const QString& aClassName);
  bool RenameClass(const QString& aOldClassName,
                   const QString& aNewClassName);
  void DeleteClass(const QString& aClassName);
  ggClassyClassContainer& GetClasses();
  const ggClassyClassContainer& GetClasses() const;

  // class boxes
  ggClassyClassBox* AddClassBox(ggClassyClassBox* aClassBox);
  ggClassyClassBoxContainer& GetClassBoxes();
  const ggClassyClassBoxContainer& GetClassBoxes() const;

  // other
  const ggSubject* GetSubjectConnections() const;
  void Clear();

  // persist as dom-element
  QDomElement CreateDomElement(QDomDocument& aDocument) const;
  static ggClassyDataSet* Create(const QDomElement& aElement);

private:

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  ggClassyClassBoxContainer mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;

  ggSubject mSubjectConnections;

  const ggUInt32 mFormatVersionMajor;
  const ggUInt32 mFormatVersionMinor;
  const ggUInt32 mFormatVersionPatch;

};

#endif // GGCLASSYDATASET_H
