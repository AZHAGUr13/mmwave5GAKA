#ifndef ALL_USER_H
#define ALL_USER_H

#include <ns3/header.h>

#include "ns3/user-equipment-aka.h"
#include "ns3/serving-network-aka.h"
#include "ns3/home-network-aka.h"
#include "ns3/object.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include "ns3/netanim-module.h"
#include <ns3/nstime.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "ns3/application.h"
namespace ns3 {

class AllUser
{
  public:
    AllUser ();
     AllUser (uint16_t);
    ~AllUser ();
    void add(uint32_t);
    void map();
     void randomMap();
     void getFail();
     uint16_t getSNAttack();
uint16_t getUEAttack();

    Ptr<UserEqupi> getN(uint32_t);
    Ptr<ServingNetworkAka> getServingN(uint32_t);
    Ptr<homeNetwork> getHomeN(uint32_t);
    uint32_t getSize();
    uint16_t getS(uint16_t);
    uint16_t getH(uint16_t);
    uint32_t getServingSize();
     uint32_t getHomeSize();
  private:
    std::vector<Ptr<UserEqupi>> Users;
    std::vector<Ptr<ServingNetworkAka>> Serving;
    std::vector<Ptr<homeNetwork>> Home;
    std::vector<uint16_t> ussnmap;
    std::vector<uint16_t> ushnmap;
    uint16_t ueFail,seFail;


};
}
#endif