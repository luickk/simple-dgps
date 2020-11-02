#include <iostream>
#include "simpleDGps.h"
#include "include/minicsv.h"

using namespace std;

int main()
{
  // measured ranges (uncorrected)
  satRanges testPseudoRanges;
  testPseudoRanges.ranges.insert({12, 19834597.8712713});
  testPseudoRanges.ranges.insert({20, 22473250.5});
  testPseudoRanges.ranges.insert({2, 19784715.1994221});
  testPseudoRanges.ranges.insert({19, 22692935.9330983});
  testPseudoRanges.ranges.insert({32, 22635781.9852949});
  testPseudoRanges.ranges.insert({1, 22234047.0904046});
  testPseudoRanges.ranges.insert({10, 22680784.059995});

  // satellite position
  satLocation testSatPos;
  testSatPos.locations.insert({12, ecefPos{7319.328892, 22537.009754, 9633.784785}});
  testSatPos.locations.insert({20, ecefPos{-19375.669381, -7908.139490, 14546.807094}});
  testSatPos.locations.insert({2, ecefPos{7309.118798, 41520.298278, -638.913719}});
  testSatPos.locations.insert({19, ecefPos{-21884.279366, 17592.995467, -9332.928775}});
  testSatPos.locations.insert({32, ecefPos{-16384.269913, 15924.975555, 13538.689663}});
  testSatPos.locations.insert({1, ecefPos{-32309.946649, 27098.563365, 698.442379}});
  testSatPos.locations.insert({10, ecefPos{5875.696632, 37643.533243, 18439.735399}});

  // basestation correction, required for correction
  latLonAltPos baseStationPos;
  baseStationPos.lat = 85.934710;
  baseStationPos.lon = 17.747955;
  baseStationPos.alt = 5;

  // calculating uncorreted range set
  ecefPos res = trillatPosFromRange(testSatPos, testPseudoRanges);
  std::cout << "Test uncorrected trillat:" << std::endl;
  cout << res.x << endl;
  cout << res.y << endl;
  cout << res.z << endl;

  // calculating range correction set
  satRanges correctionRanges = calcSatRangeCorrection(testSatPos, baseStationPos, testPseudoRanges);
  // applying correction to measured uncorrected pseudo ranges
  satRanges correctedPseudoRanges = applyCorrectionOnPseudoRange(correctionRanges, testPseudoRanges);

  // calculating position from corrected pseudo ranges
  ecefPos resCorrected = trillatPosFromRange(testSatPos, correctedPseudoRanges);
  std::cout << "Test corrected trillat:" << std::endl;
  cout << resCorrected.x << endl;
  cout << resCorrected.y << endl;
  cout << resCorrected.z << endl;

  return 0;
}
