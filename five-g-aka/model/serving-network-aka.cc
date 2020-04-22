/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "serving-network-aka.h"
#include "ns3/point-to-point-module.h"
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
NS_LOG_COMPONENT_DEFINE("SeingNetworkAka");
  NS_OBJECT_ENSURE_REGISTERED(ServingNetworkAka);

typedef unsigned char u8;
ServingNetworkAka::~ServingNetworkAka ()
{
}
uint32_t ServingNetworkAka::verifyResT(u8 resToken[])
{
  uint16_t k = 0;
  for(unsigned int j=0;j<max;j++)
  {
    k=0;
    for(unsigned int i=0;i<32;i++)
    {
      //  std::cout<<int(resToken[i+8])<<"  "<<int(hresUE[j][i])<<std::endl;
      if (int(resToken[i])!= int(hresUE[j][i]))
      {
        k=1;
        break;
      }
   }
   if(k==0)
    return j+1;

  }
   //count++;
    return 0;
}


uint32_t ServingNetworkAka::verifyRes(u8 resToken[])
{
  uint16_t k = 0;
  for(unsigned int j=0;j<max;j++)
  {
    k=0;
    for(unsigned int i=0;i<32;i++)
    {
        //std::cout<<int(resToken[i+8])<<"  "<<int(hresUE[j][i])<<std::endl;
      if (int(resToken[i+8])!= int(hresUE[j][i]))
      {
        k=1;
        break;
      }
   }
   if(k==0)
    return j+1;

  }
   //count++;
    return 0;
}
void
ServingNetworkAka::StartApplication (void)
{
}  
 void
ServingNetworkAka::StopApplication (void)
{
}
  
    




ServingNetworkAka::ServingNetworkAka (u8 snid)
{
  nodes.Create(1);
  snid = 0x64;
 // std::cout<<int(snid)<<"vbshvbsdbjddcn\n";
  
  enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector (0.0, 0.0, 0.0));
  enbmobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbmobility.SetPositionAllocator (enbPositionAlloc);
  enbmobility.Install (nodes);
  for(unsigned int i=0;i<16;i++)
      R1[0][i] = 0x63;
    // send to UE
}

ServingNetworkAka::ServingNetworkAka ()
{
  nodes.Create(1);
  snid = 0x64;
  enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector (rand()%50, rand()%50, 0.0));
  enbmobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbmobility.SetPositionAllocator (enbPositionAlloc);
  enbmobility.Install (nodes);
 
  
  for(unsigned int i=0;i<16;i++)
  {

      R1[0][i] = 0x63;
      
  }
    // send to UE
}
void ServingNetworkAka::getR1(u8 R[])
{
 for(unsigned int i=0;i<16;i++)
      R[i] = R1[0][i];
  R[16] = getsnID()
;
  //std::cout<<int(getsnID())<<" "<<int(R[16]);
}
void ServingNetworkAka::setR1(uint16_t l)
{
 for(unsigned int i=0;i<16;i++)
   {
     
     R1[l][i] = 0x00 + rand()%128;
    }   
}
void ServingNetworkAka::getR1(u8 R[],uint16_t l)
{
 for(unsigned int i=0;i<16;i++)
   {
     
      R[i] = R1[l][i];
     // std::cout<<"Rand is\t:"<<R[i];
   }   
  R[16] = getsnID();
}
 void ServingNetworkAka::setHRES(u8 value[])
 {
   for(unsigned int i=0;i<32;i++)
      hresUE[0][i] = value[i+16];

 }
 void ServingNetworkAka::setHRES(u8 value[],u8 a[])
 {
   for(unsigned int i=0;i<32;i++)
      hresUE[0][i] = value[i+8];

 }
 void ServingNetworkAka::setHRES(u8 value[],uint16_t l)
 {
   for(unsigned int i=0;i<32;i++)
      hresUE[l][i] = value[i+16];

 }
u8 ServingNetworkAka::getsnID()
{
  return 0x64;
}
NodeContainer ServingNetworkAka::getNode()
{
  return nodes;
}
/*def singleKeyGenerate( rand1, rand):
	sumDigit=0
	for i in rand1:
		sumDigit += indexMap[i]
	print("addition:",sumDigit)	
	sumDigit = sumDigit % len(rand)
	print("mod:",sumDigit)
	rotate = rotr(rand, sumDigit )
	print("Rotation :",rotate)
	addition = add(rand1, rotate)
	complement = comple(addition)
	print("complem :",complement)
	return complement,rand1*/
		
}
