#ifndef GGDECORATION_H
#define GGDECORATION_H

// 1) include system or QT
#include <vector>

// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * @brief The ggDecoration class
 */
class ggDecoration
{
public:

  enum class cType {
    eNothing,
    eLine,
    eArrow,
    eArrowBack,
    eTriangle,
    eTriangleBack,
    eDiamond,
    eCross,
    eCircle
  };

  enum class cFill {
    eEmpty,
    eSolid
  };

  ggDecoration();

  ggDecoration(cType aType,
               float aLength = 12.0f,
               cFill aFill = cFill::eSolid);

  void SetType(cType aType);
  cType GetType() const;

  void SetLength(float aLength);
  float GetLength() const;

  void SetFill(cFill aFill);
  cFill GetFill() const;
  bool GetFillEmpty() const;
  bool GetFillSolid() const;

  void Set(cType aType,
           float aLength = 12.0f,
           cFill aFill = cFill::eSolid);

  typedef std::vector<cType> tTypes;
  static const tTypes& Types();

private:

  cType mType;
  float mLength;
  cFill mFill;

};

#endif // GGDECORATION_H
