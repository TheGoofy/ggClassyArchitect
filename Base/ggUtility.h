#ifndef GGUTILITIES_H
#define GGUTILITIES_H

// 1) include system

// 3) forward declarations
#include "Base/ggTypes.h"

// 3) forward declarations

/**
 * collection of general, simple utilities
 */
namespace ggUtility {

  enum class cNumberType {
    eUnknown,
    eChar,  eInt8,  eInt16,  eInt32,  eInt64,  eSize,
    eUChar, eUInt8, eUInt16, eUInt32, eUInt64, eUSize,
    eFloat, eDouble
  };

  template <typename T> inline cNumberType NumberType() { return cNumberType::eUnknown; }

  template <> inline cNumberType NumberType<ggChar>() { return cNumberType::eChar; }
  template <> inline cNumberType NumberType<ggInt8>() { return cNumberType::eInt8; }
  template <> inline cNumberType NumberType<ggInt16>() { return cNumberType::eInt16; }
  template <> inline cNumberType NumberType<ggInt32>() { return cNumberType::eInt32; }
  template <> inline cNumberType NumberType<ggInt64>() { return cNumberType::eInt64; }
  template <> inline cNumberType NumberType<ggSize>() { return cNumberType::eSize; }
  template <> inline cNumberType NumberType<ggUChar>() { return cNumberType::eUChar; }
  //template <> inline cNumberType NumberType<ggUInt8>() { return cNumberType::eUInt8; }
  template <> inline cNumberType NumberType<ggUInt16>() { return cNumberType::eUInt16; }
  template <> inline cNumberType NumberType<ggUInt32>() { return cNumberType::eUInt32; }
  template <> inline cNumberType NumberType<ggUInt64>() { return cNumberType::eUInt64; }
  template <> inline cNumberType NumberType<ggUSize>() { return cNumberType::eUSize; }
  template <> inline cNumberType NumberType<ggFloat>() { return cNumberType::eFloat; }
  template <> inline cNumberType NumberType<ggDouble>() { return cNumberType::eDouble; }

  // different rounding resolutions
  enum class cRoundType {
    eFactor20,
    eFactor15,
    eFactor12
  };

  // rounds to ... 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50 ...
  ggFloat RoundTo125(const ggFloat aValue, cRoundType aRoundType = cRoundType::eFactor20);

  // round to Order of MaGnitude: 5321.215 => 5320.0 ... 0.08716813 => 0.0872 ... 2.575612 => 2.58
  ggFloat RoundToOMG(ggFloat aValue, ggUInt16 aOrderOfMagnitude = 3);

  template <typename T>
  inline const T& Clamp(const T& aValue, const T& aValueMin, const T& aValueMax) {
    return (aValue < aValueMin) ? aValueMin : ((aValue > aValueMax) ? aValueMax : aValue);
  }

}

#endif // GGUTILITIES_H
