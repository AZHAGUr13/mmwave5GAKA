
#include "five-g-aka.h"
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
#include "myheader.h"
#include<string>
#include<map>
#include<iostream>
#include "ns3/log.h"
#include "ns3/simulator.h"
namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("MyHeader");

    TypeId
MyHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MyHeader")
    .SetParent<Header> ()
    ;
  return tid;
}
TypeId MyHeader::GetInstanceTypeId (void) const
{
  return MyHeader::GetTypeId ();
}
void 
MyHeader::SetData (u8 r[],uint16_t data)
{
   m_length = data;
   for(uint16_t i=0;i<data;i++)
   {
     m_data[i] = r[i];
  //    std::cout<<"Data set :"<<int(m_data[i])<<std::endl;
   }
}
void
MyHeader::GetData (u8 r[])
{
  for(unsigned int i=0;i<m_length;i++)
   {
      r[i] = m_data[i] ;
   //   std::cout<<int(r[i])<<" ";
   }
 //  std::cout<<"Lenth:"<<m_length<<std::endl;
}
void 
MyHeader::SetKey (uint16_t data)
{
   m_key[0] = data;
}
void 
MyHeader::SetToken (uint16_t data)
{
  
   m_key[1] = data;
  
}
uint16_t
MyHeader::GetKey ()
{
  return m_key[0];
}
uint16_t
MyHeader::GetToken()
{
  return m_key[1];
}
uint32_t 
MyHeader::GetSerializedSize (void) const
{
  // 2 bytes of data to store
  return 122;
}
void 
MyHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU16(m_length);
  //std::cout<<"Serilized :"<<int(m_length)<<std::endl;
  for(unsigned int i=0;i<m_length;i++)
   {
       start.WriteU16(m_data[i]);
    //   std::cout<<"Serilized :"<<int(m_data[i])<<std::endl;     
   }
    start.WriteU16(m_key[0]);
    start.WriteU16(m_key[1]);
    
   // std::cout<<"Serilized :"<<int(m_key[0])<<std::endl;
}
uint32_t 
MyHeader::Deserialize (Buffer::Iterator start)
{
   m_length = start.ReadU16 ();
   // std::cout<<"Serilized :"<<int(m_length)<<std::endl;
  for(unsigned int i=0;i<m_length;i++)
   {
      m_data[i] = start.ReadU16 ();
     // m_data[1] = start.ReadNtohU32 ();
   //    std::cout<<"DE Serilized :"<<int(m_data[i])<<std::endl;
       //std::cout<<"DE sSerilized :"<<m_data[1]<<std::endl;
       
   }
    m_key[0] = start.ReadU16 ();
    m_key[1] = start.ReadU16 ();
   
 //   std::cout<<"Serilized :"<<int(m_key[0])<<std::endl;
   

 
  return 100;
}
void 
MyHeader::Print (std::ostream &os) const
{
  for(unsigned int i=0;i<m_length;i++)
   {
     os << m_data[i];
   }
  
}
}
