
#include "user-aka.h"
#include "serving-aka.h"
#include "home.h"
#include "ns3/akaprotocol.h"
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
#include "ns3/custom-data-tag.h"
#include "ns3/net-device.h"
#include "ns3/myheader.h"
#include "ns3/sha256.h"
#include "vector"
#include<string>
#include<map>
#include<iostream>
#include "ns3/log.h"
#include "ns3/simulator.h"


namespace ns3 {
  akaprotocol::akaprotocol()
  {
    for(unsigned int i=0;i<5;i++) // create 10 UE
    {
       Ptr<olduser> h = CreateObject<olduser>();
      Users.push_back(h);
    }
   
  }
  akaprotocol::akaprotocol(uint16_t n)
  {
    for(unsigned int i=0;i<10;i++) // create 10 UE
    {
       Ptr<olduser> h = CreateObject<olduser>();
      Users.push_back(h);
    }
    for(unsigned int i=0;i<3;i++) // create 10 UE
    {
       Ptr<oldhome> h = CreateObject<oldhome>();
      Home.push_back(h);
    }
    add(n);
   
  }
void akaprotocol::add(uint32_t n){
 for(unsigned int i=0;i<5;i++) // create 10 UE
    {
       Ptr<oldserving> h = CreateObject<oldserving>();
      Serving.push_back(h);
    }
}
void akaprotocol::map(){
  for(uint16_t i=0;i<Users.size();i++)
  {
    ussnmap.push_back(i/2);
  }
  ushnmap.push_back(0);
  for(uint16_t i=0;i<Users.size();i++)
  {
    ushnmap.push_back(1);
  }

}
uint16_t akaprotocol::getSNAttack()
{
  return seFail;
}
uint16_t akaprotocol::getUEAttack()
{
  return ueFail;
}
void akaprotocol::getFail()
{
  ueFail = rand()%5;
  seFail = rand()%3;
}

void akaprotocol::randomMap(){
  for(uint16_t i=0;i<Users.size();i++)
  {
    ussnmap.push_back(rand()%Serving.size());
  }
  ushnmap.push_back(0);
  for(uint16_t i=0;i<Users.size();i++)
  {
    ushnmap.push_back(rand()%Home.size());
  }

}
uint16_t akaprotocol::getS(uint16_t n)
{
  if(n>=Users.size())
  {
    n=0;
  }
  return ussnmap[n];
}
uint16_t akaprotocol::getH(uint16_t n)
{
  if(n>=Users.size())
  {
    n=0;
  }
  return ushnmap[n];
}
Ptr<olduser> akaprotocol::getN(uint32_t n){
  //return Users[n];
  //olduser h;
  if(n>=Users.size())
  {
    n = 0;
  }
  return Users[n];
}
 Ptr<oldserving> akaprotocol::getServingN(uint32_t n)
 {
   if(n>=Serving.size())
  {
    n = 0;
  }
  return Serving[n];
 }
 Ptr<oldhome> akaprotocol::getHomeN(uint32_t n)
 {
   if(n>=Home.size())
  {
    n = 0;
  }
  return Home[n];
 }
uint32_t akaprotocol::getSize()
{
  return uint32_t(Users.size());
}
uint32_t akaprotocol::getServingSize()
{
  return uint32_t(Serving.size());
}
uint32_t akaprotocol::getHomeSize()
{
  return uint32_t(Home.size());
}
akaprotocol::~akaprotocol()
  {
    //Users.push_back(5);
  }
}