#include "Base/ggUtility.h"

#include <math.h>
#include <vector>

#include <Base/ggWalkerT.h>


const std::vector<ggFloat> ggRoundFactors20 =
  {-10.0f, -5.0f, -2.0f, -1.0f,
     1.0f,  2.0f,  5.0f, 10.0f};

const std::vector<ggFloat> ggRoundFactors15 =
  {-10.0f, -7.0f, -5.0f, -3.0f, -2.0f, -1.5f, -1.0f,
     1.0f,  1.5f,  2.0f,  3.0f,  5.0f,  7.0f, 10.0f};

const std::vector<ggFloat> ggRoundFactors12 =
  {-10.0f, -8.0f, -7.0f, -6.0f, -5.0f, -4.0f, -3.0f, -2.5f, -2.0f, -1.7f, -1.5f, -1.2f, -1.0f,
     1.0f,  1.2f,  1.5f,  1.7f,  2.0f,  2.5f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f, 10.0f};


ggFloat ggUtility::RoundTo125(ggFloat aValue, cRoundType aRoundType)
{
  // nothing to do, if value is 0
  if (aValue == 0.0f) return aValue;

  // preliminary result
  ggFloat vResult = aValue;

  // calculate the order of magnitude (negative if value below 1)
  ggInt32 vDecimals = (ggUInt32)log10(fabs(aValue));
  vDecimals = fabs(aValue) < 1.0f ? vDecimals - 1 : vDecimals;

  // select proper factors for rounding
  const std::vector<ggFloat>* vFactors = &ggRoundFactors20;
  switch (aRoundType) {
    case cRoundType::eFactor20: vFactors = &ggRoundFactors20; break;
    case cRoundType::eFactor15: vFactors = &ggRoundFactors15; break;
    case cRoundType::eFactor12: vFactors = &ggRoundFactors12; break;
    default: break;
  }

  // search the value, which is as close as possible to the value
  ggFloat vErrorMin = std::numeric_limits<ggFloat>::max();
  ggWalkerT<std::vector<ggFloat>::const_iterator> vFactorsWalker(*vFactors);
  while (vFactorsWalker) {
    ggFloat vValue = *vFactorsWalker * pow(10.0f, (ggFloat)vDecimals);
    if (vErrorMin > fabs(vValue - aValue)) {
      vErrorMin = fabs(vValue - aValue);
      vResult = vValue;
    }
  }

  // done! :-)
  return vResult;
}


ggFloat ggUtility::RoundToOMG(ggFloat aValue, ggUInt16 aOrderOfMagnitude)
{
  // nothing to do, if value is 0
  if (aValue == 0.0f) return aValue;

  // calculate the order of magnitude / position of decimal point (negative if value below 1)
  ggInt32 vDecimals = (ggInt32)log10(fabs(aValue));
  vDecimals = fabs(aValue) < 1.0f ? vDecimals - 1 : vDecimals;

  // compute a factor, which is a power of 10
  ggFloat vFactor = pow(10.0f, vDecimals - aOrderOfMagnitude + 1);

  // divide, round, and multiply
  ggFloat vRoundOffset = (aValue > 0.0f) ? 0.5f : -0.5f;
  ggFloat vResult = (ggInt32)(aValue / vFactor + vRoundOffset) * vFactor;

  return vResult;
}
