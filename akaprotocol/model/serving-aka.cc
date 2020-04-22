/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "serving-aka.h"
#include "ns3/point-to-point-module.h"
#include "ns3/sha256.h"
#include "ns3/user-equipment-aka.h"
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
#include "ns3/custom-data-tag.h"
#include "ns3/net-device.h"
#include "ns3/myheader.h"
#include<string>
#include<map>
#include<iostream>
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {

/* ... */
NS_LOG_COMPONENT_DEFINE("oldserving");
  NS_OBJECT_ENSURE_REGISTERED(oldserving);

typedef unsigned char u8;
oldserving::~oldserving ()
{
}


uint32_t oldserving::verifyRes(u8 resToken[],u8 output[])
{
  uint16_t k = 0;
  u8 hres[32];
  hashRes(resToken,hres);
  for(unsigned int j=0;j<max;j++)
  {
    k=0;
    for(unsigned int i=0;i<32;i++)
    {
       // std::cout<<int(resToken[i+8])<<"  "<<int(hresUE[j][i])<<std::endl;
      if (int(hres[i])!= int(hresUE[j][i]))
      {
        k=1;
        break;
      }
   }
   if(k==0)
   {
      for(unsigned int k=0;k<32;k++)
        output[k] = resToken[k];
      for(unsigned int k=0;k<16;k++)
        output[k+32] = suci[j][k];
      return j+1;
   }
    

  }
   //count++;
    return 0;
}



oldserving::oldserving (u8 snid)
{
  nodes.Create(1);
  snid = 64;
  count = 0;
  enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector (0.0, 0.0, 0.0));
  enbmobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbmobility.SetPositionAllocator (enbPositionAlloc);
  enbmobility.Install (nodes);
  for(unsigned int i=0;i<16;i++)
      R1[0][i] = 0x63;
    // send to UE
}
oldserving::oldserving ()
{
  nodes.Create(1);
  count =0 ;
  snid = 64;
  enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector (rand()%50, rand()%50, rand()%50));
  enbmobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbmobility.SetPositionAllocator (enbPositionAlloc);
  enbmobility.Install (nodes);
 
  
  for(unsigned int i=0;i<16;i++)
  {

      R1[0][i] = 0x63;
      
  }
    // send to UE
}

 void oldserving::setHRES(u8 value[])
 {
   for(unsigned int i=0;i<32;i++)
      hresUE[0][i] = value[i+8];

 }
 void oldserving::setSUCI(u8 value[])
 {
   for(unsigned int i=0;i<16;i++)
      suci[0][i] = value[i];

 }
 void oldserving::setSUCI(u8 value[],uint16_t l)
 {
   for(unsigned int i=0;i<16;i++)
      suci[l][i] = value[i];

 }
 void oldserving::setHRES(u8 value[],uint16_t l)
 {
   for(unsigned int i=0;i<32;i++)
      hresUE[l][i] = value[i+8];

 }
u8 oldserving::getsnID()
{
  return 0x64;
}
NodeContainer oldserving::getNode()
{
  return nodes;
}
		
}
