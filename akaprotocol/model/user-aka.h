/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef USER_AKA_H
#define USER_AKA_H

#include <ns3/header.h>

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

/* ... */

typedef unsigned char u8;

class olduser : public Object
{
public:
  olduser ();
  olduser (u8);
   ~olduser ();
void setString(u8[],u8[]);
bool calculation(u8[],u8[]);
NodeContainer getNode();

  static TypeId GetTypeId (void);

    //void add(UserEqupi);
    //UserEqupi getN(int n);
private:
  
 // std::vector<UserEqupi> Users;
  u8 R2[16],resUE[32];
  u8 SQN[16];
  u8 snid;
  u8 key[16];
  u8 amf[2];
  u8 supi[16];
  u8 suci[8];
  NodeContainer nodes;
  MobilityHelper uemobility;
  Ptr<ListPositionAllocator> uePositionAlloc;
  
  
 
};


/*class fiveGAkaHN
{
        protected:

        public:
}*/
} // namespace ns3
#endif /* FIVE_G_AKA_H */