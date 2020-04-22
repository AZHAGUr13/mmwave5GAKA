/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef SERVING_AKA_H
#define SERVING_AKA_H

#include <ns3/header.h>
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
#include <string>
#include <iostream>
#define max 20
#include "ns3/application.h"
namespace ns3 {



typedef unsigned char u8;
class oldserving : public Object
{
public:
  oldserving (u8);

  oldserving ();
  virtual ~oldserving ();
  NodeContainer getNode();
  void getR1(u8[],uint16_t);
  void setR1(uint16_t);
  u8 getsnID();
  void setHRES(u8[]);
  void setSUCI(u8[]);
  void setSUCI(u8[],uint16_t);
  void setHRES(u8[],uint16_t);
  uint32_t verifyRes(u8[],u8[]);
  uint32_t verifyRes(u8[],uint16_t);
    uint32_t count;
private:
  
  NodeContainer nodes;
  MobilityHelper enbmobility;
  Ptr<ListPositionAllocator> enbPositionAlloc;
 
  u8 R1[max][16];
  u8 snid;
  u8 suci[max][16];
  u8 hresUE[max][32];
  Ptr<NetDevice> dev;
  Ptr<Node> n;
  
};



/*class fiveGAkaHN
{
        protected:

        public:
}*/
} // namespace ns3
#endif /* FIVE_G_AKA_H */