// 0) include own header
#include "ggClassyDataBrowserDockWidget.h"
#include "ui_ggClassyDataBrowserDockWidget.h"

// 1) include system or QT
#include <QFileSystemModel>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"
#include "Base/ggWalkerT.h"
#include "Base/ggVectorSetT.h"
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyMain/ggClassyTreeItem.h"
#include "ClassyMain/ggClassyApplication.h"



class ggClassyDataModel :
  public QAbstractItemModel,
  public ggObserver
{
public:

  ggClassyDataModel(QObject* aParent = nullptr) :
    QAbstractItemModel(aParent),
    mDataSet(nullptr),
    mRootItem(nullptr)
  {
    mDataSet = ggClassyApplication::GetInstance().GetDataSet();
    Attach(&mDataSet->GetCollections());
    Attach(&mDataSet->GetClasses());
    Attach(&mDataSet->GetFrames());
    UpdateTree();
  }

  virtual void Update(const ggSubject* aSubject) override
  {
    if (aSubject == &mDataSet->GetCollections() ||
        aSubject == &mDataSet->GetClasses() ||
        aSubject == &mDataSet->GetFrames()) {
      beginResetModel();
      UpdateTree();
      endResetModel();
    }
  }

  void UpdateTree()
  {
    delete mRootItem;
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

    // add frames
    ggWalkerT<ggClassyFrameContainer::iterator> vFramesWalker(mDataSet->GetFrames());
    while (vFramesWalker) {

      // add frame to collection (or root, if there is no collection)
      ggClassyFrame* vFrame = *vFramesWalker;
      tCollectionsMap::iterator vCollectionItemIterator = vCollectionsMap.find(vFrame->GetCollectionName());
      if (vCollectionItemIterator != vCollectionsMap.end()) {
        ggClassyTreeItem* vCollectionTreeItem = vCollectionItemIterator->second;
        vCollectionTreeItem->AddChild(vFrame);
      }
      else {
        mRootItem->AddChild(vFrame);
      }
    }
  }

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
    else return createIndex(vTreeItemParent->GetSiblingIndex(), 0, vTreeItemParent);
  }

  virtual int rowCount(const QModelIndex& aIndex = QModelIndex()) const override
  {
    if (aIndex.column() > 0) return 0;

    ggClassyTreeItem* vTreeItem = nullptr;

    if (!aIndex.isValid()) vTreeItem = mRootItem;
    else vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());

    return vTreeItem->GetNumberOfChildren();
  }

  virtual int columnCount(const QModelIndex& /*aIndex = QModelIndex()*/) const override
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

  /*Qt::ItemFlags flags(const QModelIndex& aIndex) const override
  {
    //qDebug() << __PRETTY_FUNCTION__ << aIndex;
    if (!aIndex.isValid()) return 0;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(aIndex);
  }*/

  // virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const override
  // virtual bool hasChildren(const QModelIndex &aParent = QModelIndex()) const override

private:

  ggClassyDataSet* mDataSet;
  ggClassyTreeItem* mRootItem;

};


ggClassyDataBrowserDockWidget::ggClassyDataBrowserDockWidget(QWidget* aParent) :
  QDockWidget(aParent),
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


void ggClassyDataBrowserDockWidget::on_mNewPushButton_clicked()
{
  QItemSelectionModel* vSelection = ui->mDataBrowserTreeView->selectionModel();
  QModelIndexList vSelectedIndices = vSelection->selectedIndexes();
  foreach (QModelIndex vModelIndex, vSelectedIndices) {
    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(vModelIndex.internalPointer());
    const ggClassyClass* vClass = vTreeItem->GetClass();
    if (vClass != nullptr) {
      ggClassyClassBox* vClassBox = new ggClassyClassBox(vClass->GetName());
      ggClassyApplication::GetInstance().GetDataSet()->AddClassBox(vClassBox);
    }
  }
}


void ggClassyDataBrowserDockWidget::on_mDelPushButton_clicked()
{
  qDebug() << __PRETTY_FUNCTION__;
}


ggClassyDataBrowserDockWidget::~ggClassyDataBrowserDockWidget()
{
  delete ui;
}
