#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDebug>
#include <set>

#include "ggTypes.h"
#include "ggSubject.h"
#include "ggWalkerT.h"


typedef std::vector<ggString> ggStringVec;
typedef std::set<ggString> ggStringSet;


class ggClassyCollection : public ggSubject
{
public:
  QString mName;
  // box frame and connections
  QPen mBoxBorder;
  QPen mConnectionLines;
  // class name
  QFont mNameFont;
  QColor mNameColor;
  QBrush mNameBackground;
  // member functions
  QFont mMembersFont;
  QColor mMembersColor;
  QBrush mMembersBackground;
  // description
  QFont mDescriptionFont;
  QColor mDescriptionColor;
  QBrush mDescriptionBackground;
};


class ggClassyCollectionContainer :
  public std::set<ggClassyCollection*>
{
};


class ggClassyClassMember
{
public:
  ggClassyClassMember(const QString& aName,
                      const QString& aClassName) :
    mName(aName),
    mClassName(aClassName) {
  }
  void SetName(const QString& aName) {
    mName = aName;
  }
  const QString& GetName() const {
    return mName;
  }
  void SetClassName(const QString& aClassName) {
    mClassName = aClassName;
  }
  const QString& GetClassName() const {
    return mClassName;
  }
  bool operator < (const ggClassyClassMember& aOther) const {
    if (mName != aOther.mName)
      return (mName < aOther.mName);
    else
      return (mClassName < aOther.mClassName);
  }
private:
  QString mName;
  QString mClassName;
};


class ggClassyClass : public ggSubject
{
public:

  ggClassyClass() {
  }

  ggClassyClass(const QString& aClassName) :
    mClassName(aClassName) {
  }

  bool operator() (const ggClassyClass* aClassA, const ggClassyClass* aClassB) const {
    if (aClassA == nullptr || aClassB == nullptr) return aClassA < aClassB;
    return aClassA->mClassName < aClassB->mClassName;
  }

  typedef std::vector<ggClassyClassMember> tMembers;

  QString GetMembersText() const {
    QString vText;
    ggWalkerT<tMembers::const_iterator> vMemberIterator(mMembers);
    while (vMemberIterator) {
      const ggClassyClassMember& vMember = *vMemberIterator;
      const QString& vMemberName = vMember.GetName();
      const QString& vMemberClassName = vMember.GetClassName();
      vText += vMemberName;
      if (vMemberClassName != "") vText += "\t" + vMemberClassName;
      if (!vMemberIterator.IsLast()) vText += "\n";
    }
    return vText;
  }

  void SetMembersText(const QString& aText) {
    mMembers.clear();
    QStringList vMembersText = aText.split("\n");
    ggWalkerT<QStringList::iterator> vMembersTextWalker(vMembersText);
    while (vMembersTextWalker) {
      const QString& vMemberText = *vMembersTextWalker;
      QStringList vMemberNameAndClass = vMemberText.split("\t", QString::SkipEmptyParts);
      QString vMemberName("");
      QString vMemberClassName("");
      if (vMemberNameAndClass.size() >= 1) vMemberName = vMemberNameAndClass.at(0).simplified();
      if (vMemberNameAndClass.size() >= 2) vMemberClassName = vMemberNameAndClass.at(1).simplified();
      mMembers.push_back(ggClassyClassMember(vMemberName, vMemberClassName));
    }
  }

  ggString mClassName;
  ggStringSet mBaseClassNames;
  tMembers mMembers;
  QString mDescription;
  QString mCollectionName;
};


class ggClassyClassBox : public ggSubject
{
public:
  ggClassyClassBox(const QString& aClassName) :
    mClassName(aClassName),
    mPosition(0.0f, 0.0f),
    mWidth(150.0f),
    mMembersVisible(true),
    mDescriptionVisible(true) {}
  QString mClassName;
  QPointF mPosition;
  float mWidth;
  bool mMembersVisible;
  bool mDescriptionVisible;
};


class ggClassyFrame : public ggSubject
{
public:
  QString mText;
  Qt::Alignment mTextAlignment;
  QString mCollectionName;
  QRectF mRect;
};


class ggClassyClassContainer :
  public std::set<ggClassyClass*, ggClassyClass>,
  public ggSubject
{
public:

  ggClassyClass* FindClass(const QString& aClassName) const
  {
    ggClassyClass vClass(aClassName);
    const_iterator vClassesIterator = find(&vClass);
    if (vClassesIterator != end()) return *vClassesIterator;
    else return nullptr;
  }

  bool RenameClass(const QString& aOldClassName,
                   const QString& aNewClassName)
  {
    qDebug() << __PRETTY_FUNCTION__ << aOldClassName << "=>" << aNewClassName;

    // if the name is unchanged, renaming is obsolete (success)
    if (aNewClassName == aOldClassName) return true;
    // can't rename, if there is no class with the "old" name (fail)
    if (FindClass(aOldClassName) == nullptr) return false;
    // can't rename, if there is another class with the "new" name (fail)
    if (FindClass(aNewClassName) != nullptr) return false;

    // change base classes and members of all other classes
    ggWalkerT<iterator> vClassesWalker(*this);
    while (vClassesWalker) {
      // get the class pointer
      ggClassyClass* vClass = *vClassesWalker;
      // be lazy: collect multiple notifications and only notify once
      ggClassyClass::cExecutorLazy vExecutorLazy(vClass);
      // replace a base class name (if there is)
      if (vClass->mBaseClassNames.erase(aOldClassName)) {
        vClass->mBaseClassNames.insert(aNewClassName);
        vClass->Notify();
      }
      // replace member class name(s)
      ggWalkerT<ggClassyClass::tMembers::iterator> vMembersWalker(vClass->mMembers);
      while (vMembersWalker) {
        ggClassyClassMember& vMember = *vMembersWalker;
        if (vMember.GetClassName() == aOldClassName) {
          vMember.SetClassName(aNewClassName);
          vClass->Notify();
        }
      }
    }

    // Since "aOldClassName" is passed as a reference, we have to change the actual
    // class name "vClass->mClassName" at the very last. It (may) happens that "aOldClassName"
    // magically changes to the new class name! ... AND the class can't be renamed, while
    // it is stored in a set, because the name is used as the key.
    ggClassyClass* vClass = FindClass(aOldClassName);
    // temporarily remove it from the set
    erase(vClass);
    // rename it
    vClass->mClassName = aNewClassName;
    // insert it again
    insert(vClass);
    // notify the change
    vClass->Notify();

    // content of this container changed
    Notify();

    // done (success)
    return true;
  }

};


class ggClassyDataSet : public ggSubject
{
public:

  static ggClassyDataSet* GenerateTestData();

  ggClassyCollectionContainer mCollections;
  ggClassyClassContainer mClasses;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;
};


#endif // GGCLASSYDATASET_H
