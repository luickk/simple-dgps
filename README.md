# Simple Differential GPS

## Introduction to available technology

GPS is an widely used and relatively cheap technology, as with everything the price behaves proportionally to the performance. That said, a 20$ GPS module can achieve an accuracy of roughly 7 meters, in good conditions. A 70$ receiver does have a few advantages as for example in the variety of protocols or reading speed but none in accuracy.

There are a few technologies available which offer accuracy of up to a few mm, but those are not just expensive but also very costly to implement and use. Technologies such as RTK(Real Time Kinematic) that rely on Phaseshift measurements to calculate the required pseudo ranges etc. cost up to a few thousand dollars and are very sensible, which often makes them dependent on corrections by nearby correction stations.

## Simple Differential GPS Approach

The aim of SDGPS is to find a cheap, accurate and easy to implement DGPS C++ lib solution. The accuracy the project is aiming for is 2-1 meters. The used DGPS technique is Code Range DGPS which corrects the pseudo range of each satellite. First the pseudo range correction is calculated by substracting the observed pseudo range from the calculated pseudo range (from ephemeris). If ephemeris data is available and the corrected pseudo range value is calculated, single point positioning is applied, to triangulate the final position.  


## Introduction to DGPS

Differential GPS or DGPS makes use of a nearby base station which knows it's true position and calculates the difference in  position to send that correction via. any means of communication to it's clients, for example a rover which already has a GPS module on board which is accurate up to a few meters, with the corrections applied it can achieve accuracies of even a quarter of a meter, assuming it's in the range of the basestation.

### RTK DGPS

Real Time Kinematic makes use of the phase rotation as an indicator for the pseudo range. The basestation measures the phase rotation and divides it by the satellites carrier frequency which returns a time, which can be used to estimate the the atmospheric error. The achieved accuracy is extremely high.

### Position DGPS

Position DGPS just uses the position difference as an correction value, so the basestation compares its lat/ lon to the known true lat/lon and sends the difference as an correction value to the rover.

The only disadvantage is that, if the set of satellites changes, the correction value does not apply anymore. So both, the receiver and the client need the exact same sets of satellites or the result can get even worse.

### Code Range DGPS

Code Range DGPS takes the difference in pseudo range as correction value, so the basestation compares the difference between the observed pseudo range and actual true pseudo range and takes it as the correction value. As with the RTK DGPS the correction values are calculated for each satellite, which increases the usability dramatically. Another huge advantage is that it does not require any expensive receivers.
The only requirement for the receiver is that it supports raw data output, that can output ephemeris and raw pseudo range data.

## Available Correction algorithms

Most of the GPS receivers already have several correction algorithms applied on there standard output, which mostly because they are relatively easy to calculate and behave linearly.
Corrected biases are, for example the satellite clock bias such as the receiver's clocks bias. Also the ionosphere, troposphere do affect the results. Those values/ corrections are known and can be removed relatively easily, if not removed even state of the art receivers would only have an accuracy of a few meters.

# Technical realisation

### Receiver choice

To compute & apply the correction values we need the RAW satellite data, which includes its own positional (Ephemeris) data such as velocity, apogee etc. and the raw pseudo range data.

Not every GPS receiver is capable of transmitting such an amount of data to the "client device", so we need a more capable receiver such as the Neo M8T. The Neo M8T can output a variety of protocols, we need one that includes Ephemeris and Pseudo Range data.

### Receiver protocol choice

Not many protocols sport eph. and pr data, the standard output for most GPS receivers is NMEA, but NMEA does not include sufficient eph. data. Also, NMEA does only include the Gedoial separation (dist. User to sat) but not the required pseudo range from each available satellite. UBX would be such a protocols, which comes in many variety's such as UBX-RXM-SFRBX, which includes pr and eph. data, which are transmitted in 3 packages every 6 secs, each packet contains a single sub frame, every subframe contains different kinds of data.
Those 6 seconds are a limiting factor but can't be overcome due to the low cost receiver.

The parsing of the protocol has already been done by RTK lib and can be found [here](https://github.com/tomojitakasu/RTKLIB/blob/master/src/rcv/ublox.c).
All the code for decoding and parsing was copied from RTKLIB.

# Setup

## Installation

Clone GitHub repo:

`git clone https://github.com/cy8berpunk/simple-dgps`

Installation:

1. Clean up object files: `make clean`
2. Compile project: `make`
3. Run executable `./basestation`

## Dependencies

- wiringPi

## TODO
- add full support for glonass sats
