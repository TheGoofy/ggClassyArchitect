#ifndef GGDECORATION_H
#define GGDECORATION_H

class ggDecoration
{
public:

  enum class cType {
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

private:

  cType mType;
  float mLength;
  cFill mFill;

};

#endif // GGDECORATION_H
