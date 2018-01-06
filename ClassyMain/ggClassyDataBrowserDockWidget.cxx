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





#include <QMimeData>

QDataStream& operator << (QDataStream& aStreamOut, const ggClassyTreeItem* aTreeItem)
{
  aStreamOut.writeRawData((char*)&aTreeItem, sizeof(ggClassyTreeItem*));
  return aStreamOut;
}

QDataStream& operator >> (QDataStream& aStreamIn, ggClassyTreeItem*& aTreeItem)
{
  aStreamIn.readRawData((char*)&aTreeItem, sizeof(ggClassyTreeItem*));
  return aStreamIn;
}

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

  virtual int columnCount(const QModelIndex& /*aIndex*/) const override
  {
    return 1;
  }

  QBrush GetBackgroundBrush(ggClassyTreeItem* aTreeItem) const
  {
    if (aTreeItem != nullptr) {
      if (aTreeItem->GetCollection() != nullptr) return QColor(255, 220, 200, 255);
    }
    return QBrush();
  }

  QBrush GetForegroundBrush(ggClassyTreeItem* aTreeItem) const
  {
    if (aTreeItem != nullptr) {
      if (aTreeItem->GetCollection() != nullptr) return QColor(150, 50, 0, 255);
    }
    return QBrush();
  }

  virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override
  {
    if (!aIndex.isValid()) return QVariant();

    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    if (aRole == Qt::DisplayRole) return vTreeItem->GetName();
    if (aRole == Qt::EditRole) return vTreeItem->GetName();
    if (aRole == Qt::ToolTipRole) return vTreeItem->GetDescription();
    if (aRole == Qt::BackgroundRole) return GetBackgroundBrush(vTreeItem);
    if (aRole == Qt::ForegroundRole) return GetForegroundBrush(vTreeItem);
    if (aRole == Qt::CheckStateRole) if (vTreeItem->GetMember() != nullptr) return Qt::Checked;
    return QVariant();
  }

  virtual bool setData(const QModelIndex& aIndex,
                       const QVariant& aValue,
                       int aRole) override
  {
    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    QString vName = vTreeItem != nullptr ? vTreeItem->GetName() : "goofy";
    qDebug() << __PRETTY_FUNCTION__ << vName << aValue << aRole;
    return QAbstractItemModel::setData(aIndex, aValue, aRole);
  }

  virtual Qt::ItemFlags flags(const QModelIndex& aIndex) const override
  {
    Qt::ItemFlags vFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (!aIndex.isValid()) return vFlags;
    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    if (vTreeItem->GetDataSet() != nullptr)    vFlags = vFlags | Qt::ItemIsDropEnabled;
    if (vTreeItem->GetCollection() != nullptr) vFlags = vFlags | Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    if (vTreeItem->GetClass() != nullptr)      vFlags = vFlags | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
    if (vTreeItem->GetMember() != nullptr)     vFlags = vFlags | Qt::ItemIsEditable | Qt::ItemNeverHasChildren | Qt::ItemIsUserCheckable;
    if (vTreeItem->GetFrame() != nullptr)      vFlags = vFlags | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
    return vFlags;
  }

  virtual bool hasChildren(const QModelIndex& aIndex) const override
  {
    if (!aIndex.isValid()) return QAbstractItemModel::hasChildren(aIndex);
    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    return vTreeItem->GetNumberOfChildren() > 0;
  }

  virtual Qt::DropActions supportedDropActions() const override
  {
    return Qt::CopyAction | Qt::MoveAction;
  }

  virtual Qt::DropActions supportedDragActions() const override
  {
    return Qt::CopyAction | Qt::MoveAction;
  }

  static const QString& GetTreeItemListMimeType()
  {
    static const QString vTreeItemMimeType("application/x-" + ggClassyTreeItem::TypeID() + "List");
    return vTreeItemMimeType;
  }

  virtual QStringList mimeTypes() const override
  {
    QStringList vMimeTypes = QAbstractItemModel::mimeTypes();
    vMimeTypes << GetTreeItemListMimeType();
    return vMimeTypes;
  }

  virtual QMimeData* mimeData(const QModelIndexList& aIndices) const override
  {
    QByteArray vMimeDataBytes;
    QDataStream vMimeDataStream(&vMimeDataBytes, QIODevice::WriteOnly);
    foreach (QModelIndex vIndex, aIndices) {
      if (vIndex.isValid()) {
        ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(vIndex.internalPointer());
        if (vTreeItem != nullptr) vMimeDataStream << vTreeItem;
      }
    }

    QMimeData* vMimeData = QAbstractItemModel::mimeData(aIndices);
    vMimeData->setData(GetTreeItemListMimeType(), vMimeDataBytes);
    return vMimeData;
  }

  virtual bool dropMimeData(const QMimeData* aData,
                            Qt::DropAction aAction,
                            int aRow, int aColumn,
                            const QModelIndex& aIndex) override
  {
    if (aData->hasFormat(GetTreeItemListMimeType())) {
      QByteArray vMimeDataBytes = aData->data(GetTreeItemListMimeType());
      QDataStream vMimeDataStream(&vMimeDataBytes, QIODevice::ReadOnly);
      while (!vMimeDataStream.atEnd()) {
        ggClassyTreeItem* vTreeItem = nullptr;
        vMimeDataStream >> vTreeItem;
        if (vTreeItem != nullptr) qDebug() << __PRETTY_FUNCTION__ << "dropped item with name:" << vTreeItem->GetName();
      }
    }

    ggClassyTreeItem* vTreeItem = static_cast<ggClassyTreeItem*>(aIndex.internalPointer());
    QString vName = vTreeItem != nullptr ? vTreeItem->GetName() : "goofy";
    qDebug() << __PRETTY_FUNCTION__ << aAction << aRow << aColumn << vName;
    return QAbstractItemModel::dropMimeData(aData, aAction, aRow, aColumn, aIndex);
  }

private:

  ggClassyDataSet* mDataSet;
  ggClassyTreeItem* mRootItem;

};





ggClassyDataBrowserDockWidget::ggClassyDataBrowserDockWidget(QWidget* aParent) :
  QDockWidget(aParent),
  ui(new Ui::ggClassyDataBrowserDockWidget)
{
  ui->setupUi(this);

  // setup model
  ggClassyDataModel* vModel = new ggClassyDataModel(this);
  ui->mDataBrowserTreeView->setModel(vModel);

  // setup selection
  ui->mDataBrowserTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  ui->mDataBrowserTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);

  // setup draw/drop
  ui->mDataBrowserTreeView->setDragEnabled(true);
  ui->mDataBrowserTreeView->setDropIndicatorShown(true);
  ui->mDataBrowserTreeView->setDragDropMode(QAbstractItemView::DragDrop);
  ui->mDataBrowserTreeView->setAcceptDrops(true);
  ui->mDataBrowserTreeView->setAutoExpandDelay(500);
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
