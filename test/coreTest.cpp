#include <iostream>
#include "simpleDGps.h"

using namespace simpleDGps;
using namespace std;

int main() {
  cout << "Hello World s";

  satRanges testPseudoRanges;
  testPseudoRanges.ranges.insert({1, 23399263.4});
  testPseudoRanges.ranges.insert({2, 22473250.5});
  testPseudoRanges.ranges.insert({16, 21269160.8});
  testPseudoRanges.ranges.insert({20, 21606540.7});
  testPseudoRanges.ranges.insert({25, 21200953.2});
  testPseudoRanges.ranges.insert({14, 23681041.7});
  testPseudoRanges.ranges.insert({6, 24601072.2});

  satLocation testSatPos;
  testSatPos.locations.insert({1, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({2, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({16, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({20, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({25, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({14, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({6, ecefPos{-17104.3, -5228.5, 19811.5}});
  
  ecefPos res = trillatPosFromRange(testSatPos, testPseudoRanges);
  cout << res.x << endl;
  cout << res.y << endl;
  cout << res.z << endl;
  return 0;
}