/**
 * @file bus.h
 *
 * @copyright 2019 3081 Staff, All rights reserved.
 */
#ifndef SRC_SMALL_BUS_H_
#define SRC_SMALL_BUS_H_
/*
#include <string>
#include <iostream>
#include <list>

#include "src/bus.h"
#include "src/route.h"*/
#include <string>
#include "src/data_structs.h"
#include "src/bus.h"
#include "src/passenger.h"
#include "src/passenger_loader.h"
#include "src/passenger_unloader.h"
#include "src/route.h"
#include "src/stop.h"

/*class PassengerUnloader;
class PassengerLoader;
class Route;
class Stop; */

class SmallBus : public Bus {
 public:
  SmallBus(std::string name, Route * out, Route * in,
     int capacity = 30, double speed = 1);
  void Report(std::ostream& out) override;
};
#endif  // SRC_SMALL_BUS_H_
