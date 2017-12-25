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

  const QString& GetName() const {
    switch (mType) {
      case cType::eDataSet: return GetDataSet()->TypeID();
      case cType::eCollection: return GetCollection()->GetName();
      case cType::eClass: return GetClass()->GetName();
      case cType::eMember: return GetMember()->GetName();
      default: {static const QString vName("Goofy"); return vName; }
    }
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
            if (aItemA->GetCollection()->GetName() != aItemB->GetCollection()->GetName())
              return (aItemA->GetCollection()->GetName() < aItemB->GetCollection()->GetName()); break;
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

  typedef ggVectorSetT<ggClassyTreeItem*, cLess> tChildren;

  cType mType;
  const void* mData;
  ggClassyTreeItem* mParent;
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
      vCollectionsMap[vCollection->GetName()] = vCollectionTreeItem;
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

  virtual QModelIndex index(int aRow, int aColumn, const QModelIndex& aIndex = QModelIndex()) const override
  {
    if (!hasIndex(aRow, aColumn, aIndex)) return QModelIndex();

    ggClassyTreeItem* vTreeItem = nullptr;

    if (!aIndex.isValid()) vTreeItem = mRootItem;
    else vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());

    ggClassyTreeItem* vTreeItemChild = vTreeItem->GetChild(aRow);

    if (vTreeItemChild != nullptr) return createIndex(aRow, aColumn, vTreeItemChild);
    else return QModelIndex();
  }

  virtual QModelIndex parent(const QModelIndex& aIndex) const override
  {
    if (!aIndex.isValid()) return QModelIndex();

    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    ggClassyTreeItem* vTreeItemParent = vTreeItem->GetParent();

    if (vTreeItemParent == mRootItem) return QModelIndex();
    else return createIndex(vTreeItemParent->GetIndex(), 0, vTreeItemParent);
  }

  virtual int rowCount(const QModelIndex& aIndex = QModelIndex()) const override
  {
    if (aIndex.column() > 0) return 0;

    ggClassyTreeItem* vTreeItem = nullptr;

    if (!aIndex.isValid()) vTreeItem = mRootItem;
    else vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());

    return vTreeItem->GetNumberOfChildren();
  }

  virtual int columnCount(const QModelIndex& aParent = QModelIndex()) const override
  {
    return 1;
  }

  virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override
  {
    if (!aIndex.isValid()) return QVariant();
    if (aRole != Qt::DisplayRole) return QVariant();

    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    if (vTreeItem != nullptr) return vTreeItem->GetName();

    return QVariant();
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
    if (aRole == Qt::DisplayRole) {
      if (aSection == 0) return "Name";
      if (aSection == 1) return "Type";
    }
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
