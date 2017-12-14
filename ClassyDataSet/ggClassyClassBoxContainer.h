#ifndef GGCLASSYCLASSBOXCONTAINER_H
#define GGCLASSYCLASSBOXCONTAINER_H

// 1) include system or QT
#include <vector>
#include <QString>

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyClassBox.h"

// 3) forward declarations

/**
 * @brief The ggClassyClassContainer class
 */
class ggClassyClassBoxContainer :
  public ggSubject
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

  ggClassyClassBox* AddClassBox(ggClassyClassBox* aClassBox);

  typedef std::set<const ggClassyClassBox*> tClassBoxes;
  void MoveClassBoxesUp(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesDown(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesTop(const tClassBoxes& aClassBoxes);
  void MoveClassBoxesBottom(const tClassBoxes& aClassBoxes);

  typedef std::vector<ggClassyClassBox*>::iterator iterator;
  iterator begin();
  iterator end();

  typedef std::vector<const ggClassyClassBox*>::const_iterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;

private:

  bool Find(const tClassBoxes& aClassBoxes,
            const ggClassyClassBox* aClassBox) const;

  void UpdateIndicesZ();

  std::vector<ggClassyClassBox*> mClassBoxes;

};

#endif // GGCLASSYCLASSBOXCONTAINER_H
