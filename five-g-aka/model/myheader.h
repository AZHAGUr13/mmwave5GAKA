#ifndef MYHEADER_H
#define MYHEADER_H

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
typedef unsigned char u8;
namespace ns3 {
class MyHeader : public Header
{
public:
  // new methods
  void SetData (u8[],uint16_t);
  void GetData (u8[]);
  void SetKey (uint16_t);
   void SetToken (uint16_t);
  uint16_t GetKey ();
   uint16_t GetToken ();
  virtual TypeId GetInstanceTypeId(void) const;
  // new method needed
  static  TypeId GetTypeId (void);
  // overridden from Header
  
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
private:
  uint16_t m_length;
  u8 m_data[50];
   uint16_t m_key[2];
};
}
#endif