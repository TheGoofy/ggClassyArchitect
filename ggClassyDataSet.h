#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include <QBrush>
#include <QPen>
#include <QFont>
#include <set>

#include "ggTypes.h"
#include "ggSubject.h"
#include "ggWalkerT.h"


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


typedef std::vector<ggString> ggStringVec;
typedef std::set<ggString> ggStringSet;


class ggClassyClass : public ggSubject
{
public:

  ggClassyClass() {
  }

  ggClassyClass(const QString& aClassName) :
    mClassName(aClassName) {
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



struct ggClassyClassPtrLess {
  bool operator() (const ggClassyClass* aClassA, const ggClassyClass* aClassB) const {
    if (aClassA == nullptr || aClassB == nullptr) return aClassA < aClassB;
    return aClassA->mClassName < aClassB->mClassName;
  }
};


class ggClassyClassContainer :
  public std::set<ggClassyClass*, ggClassyClassPtrLess>
{
public:
  ggClassyClass* Find(const QString& aClassName) const
  {
    ggClassyClass vClass(aClassName);
    ggClassyClassContainer::const_iterator vClassContainerIterator = find(&vClass);
    if (vClassContainerIterator != end()) return *vClassContainerIterator;
    else return nullptr;
  }
};


class ggClassyDataSet : public ggSubject
{
public:
  static ggClassyDataSet* GenerateTestData();
  std::set<ggClassyCollection*> mCollections;
  ggClassyClassContainer mClasses;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;
};


#endif // GGCLASSYDATASET_H
