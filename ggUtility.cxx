#include "ggUtility.h"

#include <math.h>
#include <vector>


ggFloat ggUtility::RoundTo125(ggFloat aValue)
{
  if (aValue == 0.0f) {
    return aValue;
  }
  ggInt32 vDecimals = (ggUInt32)log10(fabs(aValue));
  vDecimals = fabs(aValue) < 1.0f ? vDecimals - 1 : vDecimals;
  std::vector<ggFloat> vFactor;
  vFactor.push_back(-10.0f);
  vFactor.push_back( -5.0f);
  vFactor.push_back( -2.0f);
  vFactor.push_back( -1.0f);
  vFactor.push_back(  1.0f);
  vFactor.push_back(  2.0f);
  vFactor.push_back(  5.0f);
  vFactor.push_back( 10.0f);
  ggFloat vErrorMin = std::numeric_limits<ggFloat>::max();
  ggFloat vResult = aValue;
  for (ggUSize vFactorIndex = 0; vFactorIndex < vFactor.size(); vFactorIndex++) {
    ggFloat vValue = vFactor[vFactorIndex] * pow(10.0f, (ggFloat)vDecimals);
    if (vErrorMin > fabs(vValue - aValue)) {
      vErrorMin = fabs(vValue - aValue);
      vResult = vValue;
    }
  }
  return vResult;
}

