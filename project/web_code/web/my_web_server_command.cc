/**
 * @file my_web_server_command.cc
 *
 * @copyright 2019 3081 Staff, All rights reserved.
 */
#include "web_code/web/my_web_server_command.h"
#include <sstream>

GetRoutesCommand::GetRoutesCommand(MyWebServer* ws) : myWS(ws) {}
void GetRoutesCommand::execute(MyWebServerSession* session, picojson::value& command, MyWebServerSessionState* state) {
    // Unsused input
    (void)command;
    (void)state;

    std::vector<RouteData> routes = myWS->routes;

    //std::cout << "Updating routes" << std::endl;

    picojson::object data;
    data["command"] = picojson::value("updateRoutes");

    picojson::array routesArray;

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        picojson::object r;
        r["id"] = picojson::value(routes[i].id);

        picojson::array stopArray;
        for (int j = 0; j < static_cast<int>(routes[i].stops.size()); j++) {
            picojson::object stopStruct;
            stopStruct["id"] = picojson::value(routes[i].stops[j].id);
            stopStruct["numPeople"] = picojson::value(static_cast<double>(routes[i].stops[j].num_people));

            picojson::object pStruct;
            pStruct["x"] = picojson::value(routes[i].stops[j].position.x);
            pStruct["y"] = picojson::value(routes[i].stops[j].position.y);

            stopStruct["position"] = picojson::value(pStruct);

            stopArray.push_back(picojson::value(stopStruct));
        }

        r["stops"] = picojson::value(stopArray);
        routesArray.push_back(picojson::value(r));
    }
    data["routes"] = picojson::value(routesArray);

    picojson::value ret(data);

    session->sendJSON(ret);
}



GetBussesCommand::GetBussesCommand(MyWebServer* ws) : myWS(ws) {}
void GetBussesCommand::execute(MyWebServerSession* session, picojson::value& command, MyWebServerSessionState* state) {
    // Unsused input
    (void)command;
    (void)state;

    std::vector<BusData> busses = myWS->busses;

    //std::cout << "Updating Busses" << std::endl;

    picojson::object data;
    data["command"] = picojson::value("updateBusses");

    picojson::array bussesArray;

    for (int i = 0; i < static_cast<int>(busses.size()); i++) {
        picojson::object s;
        s["id"] = picojson::value(busses[i].id);
        s["numPassengers"] = picojson::value(static_cast<double>(busses[i].num_passengers));
        s["capacity"] = picojson::value(static_cast<double>(busses[i].capacity));

        picojson::object pStruct;
        pStruct["x"] = picojson::value(busses[i].position.x);
        pStruct["y"] = picojson::value(busses[i].position.y);
        s["position"] = picojson::value(pStruct);

        picojson::object cStruct;
        cStruct["red"] = picojson::value(static_cast<double>(busses[i].color.red));
        cStruct["green"] = picojson::value(static_cast<double>(busses[i].color.green));
        cStruct["blue"] = picojson::value(static_cast<double>(busses[i].color.blue));
        cStruct["alpha"] = picojson::value(static_cast<double>(busses[i].color.alpha));
        s["color"] = picojson::value(cStruct);

        bussesArray.push_back(picojson::value(s));
    }

    data["busses"] = picojson::value(bussesArray);

    picojson::value ret(data);

    session->sendJSON(ret);
}

StartCommand::StartCommand(VisualizationSimulator* sim) : mySim(sim), timeBetweenBusses(std::vector<int>()), numTimeSteps(10) {

}

void StartCommand::execute(MyWebServerSession* session, picojson::value& command, MyWebServerSessionState* state) {
    // Unused variables
    (void)session;
    (void)state;

    timeBetweenBusses.clear();

    numTimeSteps = static_cast<float>(command.get<picojson::object>()["numTimeSteps"].get<double>());

    picojson::array arr = command.get<picojson::object>()["timeBetweenBusses"].get<picojson::array>();
    for (picojson::array::iterator it = arr.begin(); it != arr.end(); it++) {
        timeBetweenBusses.push_back(static_cast<int>(it->get<double>()));
    }

    for (int i = 0; i < static_cast<int>(timeBetweenBusses.size()); i++) {
        std::cout << "Time between busses for route  " << i <<  ": " << timeBetweenBusses[i] << std::endl;
    }

    std::cout << "Number of time steps for simulation is: " << numTimeSteps << std::endl;
    std::cout << "Starting simulation" << std::endl;

    mySim->Start(timeBetweenBusses, numTimeSteps);
}

PauseCommand::PauseCommand(VisualizationSimulator* sim) : mySim(sim) {}

void PauseCommand::execute(MyWebServerSession* session,
 picojson::value& command, MyWebServerSessionState* state) {
  (void)session;
  (void)state;
  (void)command;

  mySim->Pause();
}

/*AddListenerCommand::AddListenerCommand(VisualizationSimulator* sim) : mySim(sim) {}

void AddListenerCommand::execute(MyWebServerSession* session,
 picojson::value& command, MyWebServerSessionState* state) {
   (void)session;
   (void)state;
   (void)command;

   picojson::object data;
   data["command"] = picojson::value("updateObserver");
   picojson::value ret(data);

   std::string busID = command.get<picojson::object>()["busID"].get<std::string>();

   mySim->ClearListeners();
   BusWebObserver* BusWebOb = new BusWebObserver(busID);
   mySim->AddListener(busID, BusWebOb);

   session->sendJSON(ret);

 }*/

AddStopListenerCommand::AddStopListenerCommand(VisualizationSimulator*
  sim) : mySim(sim) {}

void AddStopListenerCommand::execute(MyWebServerSession*
  session, picojson::value& command, MyWebServerSessionState* state) {
     mySim->ClearListeners();
     std::cout << "starting AddStopListenerCommand::execute" << std::endl;
     std::string id = command.get<picojson::object>()["id"].get<std::string>();
     std::cout << id << std::endl;
     StopWebObserver* StopWebOb = new StopWebObserver(session);
     mySim->AddStopListener(id, StopWebOb);
}

AddListenerCommand::AddListenerCommand(VisualizationSimulator*
  sim) : mySim(sim) {}

void AddListenerCommand::execute(MyWebServerSession*
  session, picojson::value& command, MyWebServerSessionState* state) {
    mySim->ClearListeners();
    std::cout << "starting AddListenerCommand::execute" << std::endl;
    std::string id = command.get<picojson::object>()["id"].get<std::string>();
    std::cout << id << std::endl;
    BusWebObserver* BusWebOb = new BusWebObserver(session);
    mySim->AddListener(id, BusWebOb);
}

UpdateCommand::UpdateCommand(VisualizationSimulator* sim): mySim(sim) {}

void UpdateCommand::execute(MyWebServerSession* session, picojson::value& command, MyWebServerSessionState* state) {
    (void)session;
    (void)state;
    (void)command;

    mySim->Update();
}



InitRoutesCommand::InitRoutesCommand(ConfigManager* configManager) : cm(configManager) {}

void InitRoutesCommand::execute(MyWebServerSession* session, picojson::value& command, MyWebServerSessionState* state) {
    (void)state;
    (void)command;

    int  numRoutes = static_cast<int>(cm->GetRoutes().size());

    picojson::object data;
    data["command"] = picojson::value("initRoutes");
    data["numRoutes"] = picojson::value(static_cast<double>(numRoutes));

    picojson::value ret(data);
    session->sendJSON(ret);

}
