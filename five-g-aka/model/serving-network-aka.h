/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef SERVING_NETWORK_AKA_H
#define SERVING_NETWORK_AKA_H

#include <ns3/header.h>
#include "ns3/point-to-point-module.h"
#include "ns3/user-equipment-aka.h"
#include "home-network-aka.h"
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
class ServingNetworkAka : public Application
{
public:
  ServingNetworkAka (u8);
  ServingNetworkAka ();
  virtual ~ServingNetworkAka ();
  void ReceivePacket (Ptr<NetDevice>, Ptr<const Packet>,uint16_t, const Address);
  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);
  NodeContainer getNode();
  void getR1(u8[]);
  void getR1(u8[],uint16_t);
  void setR1(uint16_t);
  u8 getsnID();
  void setHRES(u8[]);
  void setHRES(u8[],u8[]);
  void setHRES(u8[],uint16_t);
  uint32_t verifyRes(u8[]);
  uint32_t verifyResT(u8[]);
  uint32_t verifyRes(u8[],uint16_t);
private:
  
  NodeContainer nodes;
  MobilityHelper enbmobility;
  Ptr<ListPositionAllocator> enbPositionAlloc;
 
  u8 R1[max][16];
  u8 snid;
  u8 suci[8];
  u8 hresUE[max][32];
  UserEqupi ue;
   homeNetwork hn;
   virtual void StartApplication (void);
  virtual void StopApplication (void);
  void ScheduleTx (void);
  void SendPacket (void);
  Ptr<NetDevice> dev;
  Ptr<Node> n;
  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
};



/*class fiveGAkaHN
{
        protected:

        public:
}*/
} // namespace ns3
#endif /* FIVE_G_AKA_H */