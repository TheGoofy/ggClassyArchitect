#ifndef GGUTILITIES_H
#define GGUTILITIES_H

#include "ggTypes.h"

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

  ggFloat RoundTo125(const ggFloat aValue);

  template <typename T>
  inline const T& Clamp(const T& aValue, const T& aValueMin, const T& aValueMax) {
    return (aValue < aValueMin) ? aValueMin : ((aValue > aValueMax) ? aValueMax : aValue);
  }

}

#endif // GGUTILITIES_H
