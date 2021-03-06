/**
 * @file visualization_simulator.h
 *
 * @copyright 2019 3081 Staff, All rights reserved.
 */
#include "web_code/web/visualization_simulator.h"
#include "src/bus.h"
#include "src/route.h"

VisualizationSimulator::VisualizationSimulator(WebInterface* webI
  , ConfigManager* configM) {
    webInterface_ = webI;
    configManager_ = configM;
}

VisualizationSimulator::~VisualizationSimulator() {}

void VisualizationSimulator::Start(const std::vector<int>& busStartTimings
  , const int& numTimeSteps) {
    busStartTimings_ = busStartTimings;
    numTimeSteps_ = numTimeSteps;

    timeSinceLastBus_.resize(busStartTimings_.size());
    for (int i = 0; i < static_cast<int>(timeSinceLastBus_.size()); i++) {
        timeSinceLastBus_[i] = 0;
    }

    simulationTimeElapsed_ = 0;

    prototypeRoutes_ = configManager_->GetRoutes();
    for (int i = 0; i < static_cast<int>(prototypeRoutes_.size()); i++) {
        prototypeRoutes_[i]->Report(std::cout);

        prototypeRoutes_[i]->UpdateRouteData();
        webInterface_->UpdateRoute(prototypeRoutes_[i]->GetRouteData());
    }
}

void VisualizationSimulator::Pause() {
    std::cout << "simulation paused" << std::endl;
}

void VisualizationSimulator::AddStopListener(std::string id,
  IObserver* observer) {
  for (int i = 0; i < static_cast<int>(prototypeRoutes_.size()); i++) {
    if ( prototypeRoutes_[i]->ifContain(id) ) {
      prototypeRoutes_[i]->RegisterObserver(observer);
      prototypeRoutes_[i]->Update();
    }
  }
}


void VisualizationSimulator::AddListener(std::string id, IObserver* observer) {
  for (int i = 0; i < static_cast<int>(busses_.size()); i++) {
    if ( busses_[i]->GetName() == id ) {
      busses_[i]->RegisterObserver(observer);
      busses_[i]->Update();
    }
  }
}

void VisualizationSimulator::ClearListeners() {
  for (int i = 0; i < static_cast<int>(busses_.size()); i++) {
      busses_[i]->ClearObserver();
    }
}

void VisualizationSimulator::Update() {
    simulationTimeElapsed_++;

    std::cout << "~~~~~~~~~~ The time is now " << simulationTimeElapsed_;
    std::cout << "~~~~~~~~~~" << std::endl;

    std::cout << "~~~~~~~~~~ Generating new busses if needed ";
    std::cout << "~~~~~~~~~~" << std::endl;

    // Check if we need to generate new busses
    for (int i = 0; i < static_cast<int>(timeSinceLastBus_.size()); i++) {
        // Check if we need to make a new bus
        if (0 >= timeSinceLastBus_[i]) {
            Route * outbound = prototypeRoutes_[2 * i];
            Route * inbound = prototypeRoutes_[2 * i + 1];
            busses_.push_back(fac->GenerateBus(std::to_string(busId),
             outbound->Clone(), inbound->Clone(), 1));
            busId++;

            timeSinceLastBus_[i] = busStartTimings_[i];
        } else {
            timeSinceLastBus_[i]--;
        }
    }

    std::cout << "~~~~~~~~~ Updating busses ";
    std::cout << "~~~~~~~~~" << std::endl;

    // Update busses
    for (int i = static_cast<int>(busses_.size()) - 1; i >= 0; i--) {
        busses_[i]->Update();

        if (busses_[i]->IsTripComplete()) {
            webInterface_->UpdateBus(busses_[i]->GetBusData(), true);
            busses_.erase(busses_.begin() + i);
            continue;
        }

        webInterface_->UpdateBus(busses_[i]->GetBusData());

        busses_[i]->Report(std::cout);
    }

    std::cout << "~~~~~~~~~ Updating routes ";
    std::cout << "~~~~~~~~~" << std::endl;
    // Update routes
    for (int i = 0; i < static_cast<int>(prototypeRoutes_.size()); i++) {
        prototypeRoutes_[i]->Update();

        webInterface_->UpdateRoute(prototypeRoutes_[i]->GetRouteData());

        prototypeRoutes_[i]->Report(std::cout);
    }
}
