
#include "ns3/user-aka.h"
#include "ns3/serving-aka.h"
#include "ns3/home.h"
#include "ns3/aka.h"
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
#include <string>
#include <map>
#include <iostream>
#include "ns3/log.h"
#include "ns3/simulator.h"


namespace ns3 {
  oldAll::oldAll()
  {
    for(unsigned int i=0;i<15;i++) // create 10 UE
    {
       Ptr<olduser> h = CreateObject<olduser>();
      Users.push_back(h);
    }
   
  }
  oldAll::oldAll(uint16_t n)
  {
    for(unsigned int i=0;i<15;i++) // create 10 UE
    {
       Ptr<olduser> h = CreateObject<olduser>();
      Users.push_back(h);
    }
    Ptr<olduser> h = CreateObject<olduser>(0x32);
     Users.push_back(h);
    for(unsigned int i=0;i<2;i++) // create 10 HN
    {
       Ptr<oldhome> h = CreateObject<oldhome>();
      Home.push_back(h);
    }
    add(n);
   
  }
void oldAll::add(uint32_t n){
 for(unsigned int i=0;i<n;i++) // create n UE
    {
       Ptr<oldserving> h = CreateObject<oldserving>();
      Serving.push_back(h);
    }
    
}
void oldAll::map(){
  for(uint16_t i=0;i<Users.size();i++)
  {
    ushnmap.push_back(i%2);
  }

  for(uint16_t i=0;i<Users.size();i++)
  {
    ussnmap.push_back(i/getServingSize());
  }

}
uint16_t oldAll::getSNAttack()
{
  return seFail;
}
uint16_t oldAll::getUEAttack()
{
  return ueFail;
}
void oldAll::getFail()
{
  ueFail = 15;
  seFail = 2;
}

void oldAll::randomMap(){
  for(uint16_t i=0;i<Users.size();i++)
  {
    ussnmap.push_back(rand()%Serving.size());
  }
  ushnmap.push_back(0);
  for(uint16_t i=0;i<Users.size();i++)
  {
    ushnmap.push_back(rand()%Home.size());
  }
  ueFail = 15;
  seFail = rand()%Serving.size();
  if(seFail == 2)
  {
    seFail = 3;
  }

}
uint16_t oldAll::getS(uint16_t n)
{
  if(n>=Users.size())
  {
    n=0;
  }
  return ussnmap[n];
}
uint16_t oldAll::getH(uint16_t n)
{
  if(n>=Users.size())
  {
    n=0;
  }
  return ushnmap[n];
}
Ptr<olduser> oldAll::getN(uint32_t n){
  //return Users[n];
  //UserEqupi h;
  if(n>=Users.size())
  {
    n = 0;
  }
  return Users[n];
}
 Ptr<oldserving> oldAll::getServingN(uint32_t n)
 {
   if(n>=Serving.size())
  {
    n = 0;
  }
  return Serving[n];
 }
 Ptr<oldhome> oldAll::getHomeN(uint32_t n)
 {
   if(n>=Home.size())
  {
    n = 0;
  }
  return Home[n];
 }
uint32_t oldAll::getSize()
{
  return uint32_t(Users.size());
}
uint32_t oldAll::getServingSize()
{
  return uint32_t(Serving.size());
}
uint32_t oldAll::getHomeSize()
{
  return uint32_t(Home.size());
}
oldAll::~oldAll()
  {
    //Users.push_back(5);
  }
}