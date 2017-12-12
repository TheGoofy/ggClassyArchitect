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

  ggClassyDataSet();

  static const QString& TypeID();
  virtual const QString& VTypeID() const;
  QString GetFormatVersion() const;

  QDomElement CreateDomElement(QDomDocument& aDocument) const;

  ggClassyClass* AddClass(ggClassyClass* aClass);
  ggClassyClass* FindClass(const QString& aClassName);
  bool RenameClass(const QString& aOldClassName,
                   const QString& aNewClassName);

  ggClassyClassBox* AddClassBox(ggClassyClassBox* aClassBox);

  typedef std::set<const ggClassyClassBox*> tClassBoxes;
  void MoveClassBoxesUp(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesDown(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesTop(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesBottom(const tClassBoxes& aClassBoxes);

  const ggSubject* GetSubjectClasses() const;
  const ggSubject* GetSubjectClassBoxes() const;
  const ggSubject* GetSubjectConnections() const;

  const ggClassyClassContainer& GetClasses() const;
  const ggClassyClassBoxContainer& GetClassBoxes() const;

private:

  bool Find(const tClassBoxes& aClassBoxes,
            const ggClassyClassBox* aClassBox) const;

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  ggClassyClassBoxContainer mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;

  ggSubject mSubjectClasses;
  ggSubject mSubjectClassBoxes;
  ggSubject mSubjectConnections;

  const ggUInt32 mFormatVersionMajor;
  const ggUInt32 mFormatVersionMinor;
  const ggUInt32 mFormatVersionPatch;

};

#endif // GGCLASSYDATASET_H
