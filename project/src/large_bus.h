/**
 * @file bus.h
 *
 * @copyright 2019 3081 Staff, All rights reserved.
 */
#ifndef SRC_LARGE_BUS_H_
#define SRC_LARGE_BUS_H_

/*#include <string>
#include <iostream>
#include <list>

#include "src/bus.h"
#include "src/route.h"*/
#include <string>
#include "src/bus.h"
#include "src/data_structs.h"
#include "src/passenger.h"
#include "src/passenger_loader.h"
#include "src/passenger_unloader.h"
#include "src/route.h"
#include "src/stop.h"

/*class PassengerUnloader;
class PassengerLoader;
class Route;
class Stop; */

class LargeBus : public Bus {
 public:
  LargeBus(std::string name, Route * out, Route * in,
     int capacity = 90, double speed = 1);
  void Report(std::ostream& out) override;
};
#endif  // SRC_LARGE_BUS_H_
