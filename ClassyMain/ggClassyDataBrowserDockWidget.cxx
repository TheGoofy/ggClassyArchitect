// 0) include own header
#include "ggClassyDataBrowserDockWidget.h"
#include "ui_ggClassyDataBrowserDockWidget.h"

// 1) include system or QT
#include <QFileSystemModel>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "Base/ggVectorSetT.h"
#include "ClassyDataSet/ggClassyDataSet.h"



class ggClassyTreeItem {
public:

  ggClassyTreeItem(const ggClassyDataSet* aDataSet) :
    mType(cType::eDataSet), mData(aDataSet), mParent(nullptr) {
  }

  const ggClassyDataSet* GetDataSet() const {
    return (mType == cType::eDataSet) ? static_cast<const ggClassyDataSet*>(mData) : nullptr;
  }

  const ggClassyCollection* GetCollection() const {
    return (mType == cType::eCollection) ? static_cast<const ggClassyCollection*>(mData) : nullptr;
  }

  const ggClassyClass* GetClass() const {
    return (mType == cType::eClass) ? static_cast<const ggClassyClass*>(mData) : nullptr;
  }

  const ggClassyClassMember* GetMember() const {
    return (mType == cType::eMember) ? static_cast<const ggClassyClassMember*>(mData) : nullptr;
  }

  template <typename TClassyType>
  ggClassyTreeItem* AddChild(const TClassyType* aClassyChildItem) {
    ggClassyTreeItem* vChildItem = new ggClassyTreeItem(aClassyChildItem);
    vChildItem->mParent = this;
    mChildren.insert(vChildItem);
    return vChildItem;
  }

  ggUSize GetNumberOfChildren() const {
    return mChildren.size();
  }

  ggClassyTreeItem* GetChild(ggUSize aIndex) const {
    if (aIndex < mChildren.size()) return mChildren[aIndex];
    else return nullptr;
  }

  ggClassyTreeItem* GetParent() const {
    return mParent;
  }

  ggUSize GetIndex() const {
    if (mParent != nullptr) {
      return mParent->mChildren.indexOf(const_cast<ggClassyTreeItem*>(this));
    }
    return 0;
  }

private:

  ggClassyTreeItem(const ggClassyCollection* aCollection) :
    mType(cType::eCollection), mData(aCollection), mParent(nullptr) {
  }

  ggClassyTreeItem(const ggClassyClass* aClass) :
    mType(cType::eClass), mData(aClass), mParent(nullptr) {
  }

  ggClassyTreeItem(const ggClassyClassMember* aMember) :
    mType(cType::eMember), mData(aMember), mParent(nullptr) {
  }

  enum class cType {
    eUnknown,
    eDataSet,
    eCollection,
    eClass,
    eMember
  };

  struct cLess {
    bool operator () (const ggClassyTreeItem* aItemA, const ggClassyTreeItem* aItemB) const {
      // compare the names, if the items have the same type
      if (aItemA->mType == aItemB->mType) {
        switch (aItemA->mType) {
          case cType::eDataSet:
            return aItemA->mData < aItemB->mData;
          case cType::eCollection:
            if (aItemA->GetCollection()->mName != aItemB->GetCollection()->mName)
              return (aItemA->GetCollection()->mName < aItemB->GetCollection()->mName); break;
          case cType::eClass:
            if (aItemA->GetClass()->GetName() != aItemB->GetClass()->GetName())
              return (aItemA->GetClass()->GetName() < aItemB->GetClass()->GetName()); break;
          case cType::eMember:
            if (aItemA->GetMember()->GetName() != aItemB->GetMember()->GetName())
              return (aItemA->GetMember()->GetName() < aItemB->GetMember()->GetName()); break;
          default:
            break;
        }
      }
      // in case of different types or same name, let's compare the address
      return aItemA->mData < aItemB->mData;
    }
  };

  cType mType;

  const void* mData;

  ggClassyTreeItem* mParent;

  typedef ggVectorSetT<ggClassyTreeItem*, cLess> tChildren;
  tChildren mChildren;

};



class ggClassyDataModel :
  public QAbstractItemModel
{
public:

  ggClassyDataModel(QObject* aParent = nullptr) :
    QAbstractItemModel(aParent)
  {
    mDataSet = ggClassyDataSet::CreateTestDataSet();

    mRootItem = new ggClassyTreeItem(mDataSet);

    // add the collections
    typedef std::map<QString, ggClassyTreeItem*> tCollectionsMap;
    tCollectionsMap vCollectionsMap;
    for (ggUSize vCollectionIndex = 0; vCollectionIndex < mDataSet->GetCollections().GetSize(); vCollectionIndex++) {
      const ggClassyCollection* vCollection = mDataSet->GetCollections().GetCollection(vCollectionIndex);
      ggClassyTreeItem* vCollectionTreeItem = mRootItem->AddChild(vCollection);
      vCollectionsMap[vCollection->mName] = vCollectionTreeItem;
    }

    // add classes
    ggWalkerT<ggClassyClassContainer::iterator> vClassesWalker(mDataSet->GetClasses());
    while (vClassesWalker) {

      // add class to collection (or root, if there is no collection)
      ggClassyClass* vClass = *vClassesWalker;
      ggClassyTreeItem* vClassTreeItem = nullptr;
      tCollectionsMap::iterator vCollectionItemIterator = vCollectionsMap.find(vClass->GetCollectionName());
      if (vCollectionItemIterator != vCollectionsMap.end()) {
        ggClassyTreeItem* vCollectionTreeItem = vCollectionItemIterator->second;
        vClassTreeItem = vCollectionTreeItem->AddChild(vClass);
      }
      else {
        vClassTreeItem = mRootItem->AddChild(vClass);
      }

      // class members
      ggWalkerT<ggClassyClass::tMembers::const_iterator> vMembersWalker(vClass->GetMembers());
      while (vMembersWalker) {
        const ggClassyClassMember& vMember = *vMembersWalker;
        vClassTreeItem->AddChild(&vMember);
      }
    }
  }

  ggClassyDataSet* mDataSet;
  ggClassyTreeItem* mRootItem;

  const ggClassyDataSet* GetDataSet(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<ggClassyTreeItem*>(aIndex.internalPointer())->GetDataSet();
  }

  const ggClassyCollection* GetCollection(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<ggClassyTreeItem*>(aIndex.internalPointer())->GetCollection();
  }

  const ggClassyClass* GetClass(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<ggClassyTreeItem*>(aIndex.internalPointer())->GetClass();
  }

  const ggClassyClassMember* GetMember(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<ggClassyTreeItem*>(aIndex.internalPointer())->GetMember();
  }

  virtual QModelIndex index(int aRow, int aColumn, const QModelIndex& aParent = QModelIndex()) const override
  {
    if (!hasIndex(aRow, aColumn, aParent)) return QModelIndex();

    ggClassyTreeItem* vParentItem = nullptr;

    if (!aParent.isValid()) vParentItem = mRootItem;
    else vParentItem = static_cast<ggClassyTreeItem*>(aParent.internalPointer());

    ggClassyTreeItem* vChildItem = vParentItem->GetChild(aRow);

    if (vChildItem != nullptr) return createIndex(aRow, aColumn, vChildItem);
    else return QModelIndex();
  }

  virtual QModelIndex parent(const QModelIndex& aIndex) const override
  {
    if (!aIndex.isValid()) return QModelIndex();

    ggClassyTreeItem* vChildItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    ggClassyTreeItem* vParentItem = vChildItem->GetParent();

    if (vParentItem == mRootItem) return QModelIndex();

    return createIndex(vParentItem->GetIndex(), 0, vParentItem);

  }

  virtual int rowCount(const QModelIndex& aParent = QModelIndex()) const override
  {
    if (aParent.column() > 0) return 0;

    ggClassyTreeItem* vParentItem = nullptr;

    if (!aParent.isValid()) vParentItem = mRootItem;
    else vParentItem = static_cast<ggClassyTreeItem*>(aParent.internalPointer());

    return vParentItem->GetNumberOfChildren();
  }

  virtual int columnCount(const QModelIndex& aParent = QModelIndex()) const override
  {
    return 1;
  }

  virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override
  {
    //qDebug() << __PRETTY_FUNCTION__ << aIndex << aRole;
    if (!aIndex.isValid()) return QVariant();
    if (aRole != Qt::DisplayRole) return QVariant();
    const ggClassyDataSet* vDataSet = GetDataSet(aIndex);
    if (vDataSet != nullptr) return vDataSet->TypeID();
    const ggClassyCollection* vCollection = GetCollection(aIndex);
    if (vCollection != nullptr) return vCollection->mName;
    const ggClassyClass* vClass = GetClass(aIndex);
    if (vClass != nullptr) return vClass->GetName();
    const ggClassyClassMember* vMember = GetMember(aIndex);
    if (vMember != nullptr) return vMember->GetName();
    qDebug() << __PRETTY_FUNCTION__ << aIndex << aRole << "GOOFY";
    return "goofy";
  }

  Qt::ItemFlags flags(const QModelIndex& aIndex) const override
  {
    //qDebug() << __PRETTY_FUNCTION__ << aIndex;
    if (!aIndex.isValid()) return 0;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(aIndex);
  }

  QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole = Qt::DisplayRole) const override
  {
    //qDebug() << __PRETTY_FUNCTION__ << aSection;
    if (aRole == Qt::DisplayRole) return "goofy headeer";
    return QVariant();
  }

  // virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const override
  // virtual bool hasChildren(const QModelIndex &aParent = QModelIndex()) const override

private:

};


ggClassyDataBrowserDockWidget::ggClassyDataBrowserDockWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::ggClassyDataBrowserDockWidget)
{
  ui->setupUi(this);

  /*
  QFileSystemModel *model = new QFileSystemModel(this);
  model->setRootPath(QDir::currentPath());
  ui->mDataBrowserTreeView->setModel(model);
  */

  ggClassyDataModel* vModel = new ggClassyDataModel(this);
  ui->mDataBrowserTreeView->setModel(vModel);

}


ggClassyDataBrowserDockWidget::~ggClassyDataBrowserDockWidget()
{
  delete ui;
}
