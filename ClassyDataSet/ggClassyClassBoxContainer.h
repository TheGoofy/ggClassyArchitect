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
  public std::vector<ggClassyClassBox*>
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

};

#endif // GGCLASSYCLASSBOXCONTAINER_H
