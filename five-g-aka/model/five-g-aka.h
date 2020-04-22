/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef FIVE_G_AKA_H
#define FIVE_G_AKA_H

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
class fiveGAka : public Application
{
public:
  fiveGAka ();
  virtual ~fiveGAka ();
 void ReceivePacket (Ptr<NetDevice>, Ptr<const Packet>,uint16_t, const Address);

  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

private:
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
