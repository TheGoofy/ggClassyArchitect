#ifndef GGCLASSYITEM_H
#define GGCLASSYITEM_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)

// 3) forward declarations
class QString;

class ggClassyItem
{
public:

  static const QString& TypeID();
  virtual const QString& VTypeID() const;

};

#endif // GGCLASSYITEM_H
