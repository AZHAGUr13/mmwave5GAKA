/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef HOME_NETWORK_AKA_H
#define HOME_NETWORK_AKA_H

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

/* ... */
/*class fiveGAka : public ns3::Application
{
  std::string rotr (std::string, int);
  std::string comple (std::string);
  std::string add (std::string, std::string);
  char findKey (int);

public:
  static TypeId GetTypeId (void);
  fiveGAka ();
  ~fiveGAka ();
  void tttt (int);
  virtual TypeId GetInstanceTypeId (void) const;
  void StartApplication ();

  std::map<char, int> indexMap;
  // fiveGAka();
  std::string freshkey (std::string, std::string);
  std::string aes (std::string, std::string, std::string, std::string);
};*/
typedef unsigned char u8;
class homeNetwork : public Object
{
public:
  homeNetwork ();
   ~homeNetwork ();
 bool setString(u8[],u8[]);
 static TypeId GetTypeId (void);
private:
  u8 r1[16];
  u8 r2[16];
  u8 snid;
  u8 key[16];
  u8 supi[16];
  u8 amf[2];
  u8 mac_c[8];
  void calculation(u8[],u8[],u8[8],u8[32],u8[32]);

};

} // namespace ns3
#endif /* FIVE_G_AKA_H */