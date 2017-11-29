#ifndef GGCLASSYDATASET_H
#define GGCLASSYDATASET_H

#include <QBrush>
#include <QPen>
#include <QFont>
#include <set>

#include "ggSubject.h"
#include "ggWalkerT.h"


class ggClassyCollection : public ggSubject
{
public:
  QString mName;
  // box frame and background
  QPen mBoxBorder;
  QBrush mBoxBackground;
  // class name
  QFont mNameFont;
  QColor mNameColor;
  QBrush mNameBackground;
  // member functions
  QFont mMembersFont;
  QColor mMembersColor;
  QBrush mMembersBackground;
  // comment
  QFont mCommentFont;
  QColor mCommentColor;
  QBrush mCommentBackground;
};


class ggClassyClassMember
{
public:
  ggClassyClassMember(const QString& aName,
                      const QString& aClassName = "") :
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


typedef QString ggString;
typedef std::vector<ggString> ggStrings;


class ggClassyClass : public ggSubject
{
public:
  typedef std::vector<ggClassyClassMember> tMembers;
  bool operator < (const ggClassyClass& aOther) const {
    return (mName < aOther.mName);
  }
  QString GetMembersText() const {
    QString vText;
    ggWalkerT<tMembers::const_iterator> vMemberIterator(mMembers);
    while (vMemberIterator) {
      vText += (*vMemberIterator).GetName();
      if (!vMemberIterator.IsLast()) vText += "\n";
    }
    return vText;
  }
  void SetMembersText(const QString& aText) {
    QStringList vNames = aText.split("\n");
    mMembers.clear();
    foreach (QString vName, vNames) {
      mMembers.push_back(ggClassyClassMember(vName, "goofy"));
    }
  }
  ggString mName;
  ggStrings mBaseClassNames;
  ggClassyCollection* mCollection;
  tMembers mMembers;
  QString mComment;
};


class ggClassyClassBox : public ggSubject
{
public:
  ggClassyClassBox() :
    mClass(nullptr),
    mPosition(0.0f, 0.0f),
    mWidth(100.0f),
    mMembersVisible(true),
    mCommentVisible(true) {}
  ggClassyClass* mClass;
  QPointF mPosition;
  float mWidth;
  bool mMembersVisible;
  bool mCommentVisible;
};


class ggClassyFrame : public ggSubject
{
public:
  QString mText;
  Qt::Alignment mTextAlignment;
  ggClassyCollection* mCollection;
  QRectF mRect;
};


class ggClassyDataSet : public ggSubject
{
public:
  static ggClassyDataSet* GenerateTestData();
  std::set<ggClassyClass*> mClasses;
  std::set<ggClassyCollection*> mCollections;
  std::vector<ggClassyClassBox*> mClassBoxes;
  std::vector<ggClassyFrame*> mFrames;
};


#endif // GGCLASSYDATASET_H
