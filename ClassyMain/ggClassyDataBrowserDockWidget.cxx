// 0) include own header
#include "ggClassyDataBrowserDockWidget.h"
#include "ui_ggClassyDataBrowserDockWidget.h"

// 1) include system or QT
#include <QFileSystemModel>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "ClassyDataSet/ggClassyDataSet.h"



class ggClassyDataModel :
  public QAbstractItemModel
{
public:

  ggClassyDataModel(QObject* aParent = nullptr) :
    QAbstractItemModel(aParent)
  {
  }

  class cItem {
  public:
    cItem() :
      mType(cType::eUnknown), mData(nullptr) {
    }
    bool operator () (const cItem* aItemA, const cItem* aItemB) const {
      return aItemA->mData < aItemB->mData;
    }
    template <typename TClassyType>
    static cItem* GetItem(const TClassyType* aClassyData) {
      cItem vItem(aClassyData);
      tItems::iterator vItemsIterator = mItems.find(&vItem);
      if (vItemsIterator != mItems.end()) {
        //qDebug() << __PRETTY_FUNCTION__ << "Get cached Item";
        return *vItemsIterator;
      }
      qDebug() << __PRETTY_FUNCTION__ << "Create new Item";
      cItem* vItemPtr = new cItem(aClassyData);
      mItems.insert(vItemPtr);
      return vItemPtr;
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
  private:
    cItem(const ggClassyDataSet* aDataSet) :
      mType(cType::eDataSet), mData(aDataSet) {
    }
    cItem(const ggClassyCollection* aCollection) :
      mType(cType::eCollection), mData(aCollection) {
    }
    cItem(const ggClassyClass* aClass) :
      mType(cType::eClass), mData(aClass) {
    }
    cItem(const ggClassyClassMember* aMember) :
      mType(cType::eMember), mData(aMember) {
    }
    enum class cType {
      eUnknown,
      eDataSet,
      eCollection,
      eClass,
      eMember
    };
    cType mType;
    const void* mData;
    typedef std::set<cItem*, cItem> tItems;
    static tItems mItems;
  };

  ggClassyDataSet* mDataSet = ggClassyDataSet::CreateTestDataSet();
  mutable std::set<cItem*> mItems;

  const ggClassyDataSet* GetDataSet(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<cItem*>(aIndex.internalPointer())->GetDataSet();
  }

  const ggClassyCollection* GetCollection(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<cItem*>(aIndex.internalPointer())->GetCollection();
  }

  const ggClassyClass* GetClass(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<cItem*>(aIndex.internalPointer())->GetClass();
  }

  const ggClassyClassMember* GetMember(const QModelIndex& aIndex) const
  {
    if (aIndex.internalPointer() == nullptr) return nullptr;
    return static_cast<cItem*>(aIndex.internalPointer())->GetMember();
  }

  QModelIndex CreateIndex(int aRow, int aColumn, const ggClassyDataSet* aDataSet) const
  {
    //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn;
    cItem* vItem = cItem::GetItem(aDataSet);
    mItems.insert(vItem);
    return createIndex(aRow, aColumn, vItem);
  }

  QModelIndex CreateIndex(int aRow, int aColumn, const ggClassyCollection* aCollection) const
  {
    //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn;
    cItem* vItem = cItem::GetItem(aCollection);
    mItems.insert(vItem);
    return createIndex(aRow, aColumn, vItem);
  }

  QModelIndex CreateIndex(int aRow, int aColumn, const ggClassyClass* aClass) const
  {
    //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn;
    cItem* vItem = cItem::GetItem(aClass);
    mItems.insert(vItem);
    return createIndex(aRow, aColumn, vItem);
  }

  QModelIndex CreateIndex(int aRow, int aColumn, const ggClassyClassMember* aMember) const
  {
    //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn;
    cItem* vItem = cItem::GetItem(aMember);
    mItems.insert(vItem);
    return createIndex(aRow, aColumn, vItem);
  }

  virtual QModelIndex index(int aRow, int aColumn, const QModelIndex& aParent = QModelIndex()) const override
  {
    if (!hasIndex(aRow, aColumn, aParent)) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "no index";
      return QModelIndex();
    }
    if (!aParent.isValid()) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "create root index";
      return CreateIndex(aRow, aColumn, mDataSet);
    }
    const ggClassyDataSet* vDataSet = GetDataSet(aParent);
    if (vDataSet != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "create \"class\" index";
      int vNumberOfCollections = vDataSet->GetCollections().GetSize();
      if (aRow < vNumberOfCollections) {
        int vCollectionIndex = aRow;
        return CreateIndex(aRow, aColumn, vDataSet->GetCollections().GetCollection(vCollectionIndex));
      }
      else {
        int vClassIndex = aRow - vNumberOfCollections;
        return CreateIndex(aRow, aColumn, vDataSet->GetClasses().SearchClass(vClassIndex));
      }
    }
    const ggClassyCollection* vCollection = GetCollection(aParent);
    if (vCollection != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "create no child for collection";
      return QModelIndex();
    }
    const ggClassyClass* vClass = GetClass(aParent);
    if (vClass != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "create \"member\" index";
      return CreateIndex(aRow, aColumn, &(vClass->GetMembers()[aRow]));
    }
    const ggClassyClassMember* vMember = GetMember(aParent);
    if (vMember != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "create no child index for \"member\"";
      return QModelIndex();
    }
    qDebug() << __PRETTY_FUNCTION__ << aRow << aColumn << aParent << "GOOFY!";
    return QModelIndex();
  }

  virtual QModelIndex parent(const QModelIndex& aChild) const override
  {
    //qDebug() << __PRETTY_FUNCTION__ << aChild;
    if (!aChild.isValid()) {
      //qDebug() << __PRETTY_FUNCTION__ << aChild << "invalid index";
      return QModelIndex();
    }
    const ggClassyDataSet* vDataSet = GetDataSet(aChild);
    if (vDataSet != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aChild << "\"dataset\" has no parent";
      return QModelIndex();
    }
    const ggClassyCollection* vCollection = GetCollection(aChild);
    if (vCollection != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aChild << "parent of \"collection\" is dataset";
      return CreateIndex(0, 0, mDataSet);
    }
    const ggClassyClass* vClass = GetClass(aChild);
    if (vClass != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aChild << "parent of \"class\" is dataset";
      return CreateIndex(0, 0, mDataSet);
    }
    const ggClassyClassMember* vMember = GetMember(aChild);
    if (vMember != nullptr) {
      ggWalkerT<ggClassyClassContainer::const_iterator> vClassesWalker(mDataSet->GetClasses());
      while (vClassesWalker) {
        ggUSize vClassIndex = 0;
        const ggClassyClass* vClass = *vClassesWalker;
        ggWalkerT<ggClassyClass::tMembers::const_iterator> vMembersWalker(vClass->GetMembers());
        while (vMembersWalker) {
          const ggClassyClassMember& vMember2 = *vMembersWalker;
          if (&vMember2 == vMember) {
            //qDebug() << __PRETTY_FUNCTION__ << aChild << "parent of \"member\" is class" << vClassIndex;
            return CreateIndex(mDataSet->GetCollections().GetSize() + vClassIndex, 0, vClass);
          }
          vClassIndex++;
        }
      }
      qDebug() << __PRETTY_FUNCTION__ << aChild << "parent of \"member\" is unknown";
      return QModelIndex();
    }
    return QModelIndex();
  }

  virtual int rowCount(const QModelIndex& aParent = QModelIndex()) const override
  {
    const ggClassyDataSet* vDataSet = GetDataSet(aParent);
    if (vDataSet != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aParent << "Dataset ==>" << vDataSet->GetCollections().GetSize() + vDataSet->GetClasses().GetSize() << "children";
      return vDataSet->GetCollections().GetSize() + vDataSet->GetClasses().GetSize();
    }
    const ggClassyCollection* vCollection = GetCollection(aParent);
    if (vCollection != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aParent << "Collection ==>" << 0 << "children";
      return 0;
    }
    const ggClassyClass* vClass = GetClass(aParent);
    if (vClass != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aParent << "Class ==>" << vClass->GetMembers().size() << "children";
      return vClass->GetMembers().size();
    }
    const ggClassyClassMember* vMember = GetMember(aParent);
    if (vMember != nullptr) {
      //qDebug() << __PRETTY_FUNCTION__ << aParent << "Member ==>" << 0 << "children";
      return 0;
    }
    //qDebug() << __PRETTY_FUNCTION__ << aParent << "Root ==>" << 1 << "child (the one and only dataset)";
    return 1;
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


ggClassyDataModel::cItem::tItems ggClassyDataModel::cItem::mItems;


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
