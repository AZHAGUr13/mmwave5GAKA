/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef AKAPROTOCOL_H
#define AKAPROTOCOL_H

#include <ns3/header.h>

#include "user-aka.h"
#include "serving-aka.h"
#include "home.h"
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

class akaprotocol
{
  public:
    akaprotocol ();
     akaprotocol (uint16_t);
    ~akaprotocol ();
    void add(uint32_t);
    void map();
     void randomMap();
     void getFail();
     uint16_t getSNAttack();
uint16_t getUEAttack();

    Ptr<olduser> getN(uint32_t);
    Ptr<oldserving> getServingN(uint32_t);
    Ptr<oldhome> getHomeN(uint32_t);
    uint32_t getSize();
    uint16_t getS(uint16_t);
    uint16_t getH(uint16_t);
    uint32_t getServingSize();
     uint32_t getHomeSize();
  private:
    std::vector<Ptr<olduser>> Users;
    std::vector<Ptr<oldserving>> Serving;
    std::vector<Ptr<oldhome>> Home;
    std::vector<uint16_t> ussnmap;
    std::vector<uint16_t> ushnmap;
    uint16_t ueFail,seFail;


};
}
#endif
 /* AKAPROTOCOL_H */

