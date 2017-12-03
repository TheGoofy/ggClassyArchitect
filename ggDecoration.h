#ifndef GGDECORATION_H
#define GGDECORATION_H

class ggDecoration
{
public:

  enum class cType {
    eLine,
    eArrow,
    eTriangle,
    eDiamond,
    eCircle
  };

  ggDecoration();

  ggDecoration(cType aType,
               float aLength = 12.0f,
               bool aSolid = true);

  void SetType(cType aType);
  cType GetType() const;

  void SetLength(float aLength);
  float GetLength() const;

  void SetSolid(bool aSolid);
  bool GetSolid() const;

  void Set(cType aType,
           float aLength = 12.0f,
           bool aSolid = true);

private:

  cType mType;
  float mLength;
  bool mSolid;

};

#endif // GGDECORATION_H
