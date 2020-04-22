/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef HOME_H
#define HOME_H

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
#include "ns3/application.h"
namespace ns3 {

typedef unsigned char u8;
class oldhome : public Object
{
public:
  oldhome ();
   ~oldhome ();
 bool setString(u8[],u8[]);
 static TypeId GetTypeId (void);
   bool calculation(u8[],u8[]);
private:
  u8 sqn[16];
  u8 r2[16];
  u8 snid;
  u8 key[16];
  u8 supi[16];
  u8 amf[2];
  u8 mac_c[8];


};

} // namespace ns3
#endif /* FIVE_G_AKA_H */