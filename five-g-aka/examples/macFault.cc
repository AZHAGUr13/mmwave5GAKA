/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include "ns3/netanim-module.h"
#include "ns3/five-g-aka.h"
#include "ns3/custom-data-tag.h"
#include "ns3/serving-network-aka.h"
#include "ns3/home-network-aka.h"
#include "ns3/user-equipment-aka.h"
#include "ns3/myheader.h"
#include <iostream>

using namespace ns3;
using namespace mmwave;
NS_LOG_COMPONENT_DEFINE ("test");

  UserEqupi ue;
  homeNetwork hn;
  //static bool Reckey[]={false,false};
  uint16_t length[]={17,48,33};
  ServingNetworkAka se(64);
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("new-mac-one-device.txt");

static void
CwndChange (uint32_t lenght,Address iaddr,uint16_t key,uint32_t length,u8 value[])
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << lenght << "\t" << iaddr<<"\t"<< key<<"\t"<<length<<"\t";
  for(unsigned int i=0;i<length;i++)
  {
    *stream->GetStream () <<int(value[i])<<" ";
   }
    *stream->GetStream ()<<std::endl;
}

static void sendToUE (Ptr<Socket> socket, Time pktInterval,u8 value[],uint16_t key,Address add)
{
      Ptr<Packet> packet = Create<Packet> (17);
     
      MyHeader h1;
      
        h1.SetData (value,uint16_t(17)); 
      
      
     
      h1.SetKey (key);
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
    //  socket->Close();
      	std::cout<<"#######\t Msg with "<< key <<" send ###########\n";
  
}

static void RxofSE (Ptr<Socket> socket, Address hnadd,Address ueadd,Ptr<const Packet> packet, const Address &from)
{
    std::cout<<"Msg Received SE\n";
    MyHeader header;
    uint32_t kk=packet->PeekHeader (header);
    
    u8 R1[17],value[48],newValue[33];
    uint16_t key = header.GetKey();
    header.GetData(value);
    
    if(key == 1)
    {
      CwndChange(kk,from,key,uint32_t(48),value);
        // value is SUCI,hres 
        se.setHRES(value);
        se.getR1(R1);
        for(unsigned int i=0;i<16;i++)
        {
          newValue[i] = value[i];
          newValue[i+16] = R1[i];
        }
        newValue[32] = 0x67;
       // std::cout<<"int:"<<int(R1[16]);
        MyHeader h1;
        
         Ptr<Packet> packet = Create<Packet> (33);
        h1.SetData (newValue,uint16_t(33));
        h1.SetKey (uint16_t(2));
      // Fill out udpHeader fields appropriately
        packet->AddHeader (h1);
        socket->Connect (hnadd);
        socket->Send (packet);
     //   socket->Close();
      	std::cout<<"#######\t Msg with "<< 2 <<" send ###########\n";
    } 
    if(key == 3)
    {
      CwndChange(kk,from,key,uint32_t(48),value); //Ned to change 
      bool checkRes = se.verifyRes(value);
      checkRes = true;
      std::cout<<"\n Res verification "<<checkRes<<std::endl;
      if(checkRes)
      {
        // se.setEk(value);
        for(unsigned int i=0;i<8;i++)
        {
          newValue[i] = value[i];
        }
        MyHeader h1;
        Ptr<Packet> packet = Create<Packet> (8);
        h1.SetData (newValue,uint16_t(8));
        h1.SetKey (uint16_t(4));
        packet->AddHeader (h1);
        socket->Connect (ueadd);
        socket->Send (packet);
        std::cout<<"#######\t Msg with "<< 4 <<" send ###########\n";
      }
      else
      {
        // Abort
      }


    }
    if(key == 5)
    {
      CwndChange(kk,from,key,uint32_t(16),value);
      std::cout<<"\n\n DECRYPT KEY\n";
      for (unsigned int i=0;i<16;i++)
      {
        std::cout<<int(value[i])<<" ";
      }
      std::cout<<"\n\n##########ALL process over###";
    }
}  


static void RxofUE (Ptr<Socket> socket, Address add,Ptr<const Packet> packet, const Address &from)
{
    std::cout<<"Msg Received UE\n";
    MyHeader header;
    uint32_t kk=packet->PeekHeader (header);

      
    u8 R1[17],decryptKey[16];
    uint16_t key = header.GetKey();
    header.GetData(R1);
    if(key == 0)
    {
       CwndChange(kk,from,key,uint32_t(17),R1);
   
       Ptr<Packet> packet = Create<Packet> (48);
       MyHeader h1;
       u8 value1[48];
       ue.setString(R1,value1);
       h1.SetData (value1,uint16_t(48));
       h1.SetKey (uint16_t(1));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
     // socket->Close();
      	std::cout<<"#######\t Msg with "<< 1 <<" send ###########\n";
    }
    if(key == 4)
    {
       CwndChange(kk,from,key,uint32_t(8),R1);
   
      bool macVerification = ue.calculation(R1,decryptKey);
      if(macVerification)
      {
          Ptr<Packet> packet = Create<Packet> (16);
          std::cout<<"\n Mac Verifid \n";
          MyHeader h1;
          h1.SetData (decryptKey,uint16_t(16));
          h1.SetKey (uint16_t(5));
      // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add);
          socket->Send (packet);
           std::cout<<"#######\t Msg with "<< 5 <<" send ###########\n";
      }
      else
      {       
        // Abort
         std::cout<<"\n MAC Failed \n";
      }
      
    }
       
     
}

static void RxofHN (Ptr<Socket> socket, Address add,Ptr<const Packet> packet, const Address &from)
{
    std::cout<<"Msg Received HN \n";
    MyHeader header;
    uint32_t kk=packet->PeekHeader (header);
    u8 value[33],newValue[48];
    uint16_t key = header.GetKey();
    header.GetData(value);
    CwndChange(kk,from,key,uint32_t(33),value);
    if(key == 2 )
    {
     
      bool isTrue = hn.setString(value,newValue);
     if(isTrue)
     {
      Ptr<Packet> packet = Create<Packet> (48);
       MyHeader h1;
       h1.SetData (newValue,uint16_t(48));
       h1.SetKey (uint16_t(3));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
     // socket->Close();
      	std::cout<<"#######\t Msg with "<< 3 <<" send ###########\n";
     }
     else
     {
       //ABORT 
        Ptr<Packet> packet = Create<Packet> (48);
       MyHeader h1;
       h1.SetData (newValue,uint16_t(48));
       h1.SetKey (uint16_t(3));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
       std::cout<<"#######\t Msg with "<< 3 <<" send ###########\n";
     }
       //ue.setString(R1,value);
     
       /*Simulator::ScheduleWithContext (socket->GetNode ()->GetId (),
                                  Seconds (1.0), &sendToUE,
                                  socket,Seconds(1),value,int(1),add);*/
    } 
}






/*static void
CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldCwnd << "\t" << newCwnd << std::endl;
}*/
int 
main (int argc, char *argv[])
{
   Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (1024 * 1024));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::ResourceBlockNum", UintegerValue (1));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::ChunkPerRB", UintegerValue (72));
  double stopTime = 10.0;
 // double simStopTime = 7.00;
//  Ipv4Address remoteHostAddr;

  // Command line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Ptr<MmWaveHelper> mmwaveHelper = CreateObject<MmWaveHelper> ();
  //mmwaveHelper->SetSchedulerType ("ns3::MmWaveFlexTtiMaxWeightMacScheduler");
  mmwaveHelper->SetSchedulerType ("ns3::MmWaveFlexTtiMacScheduler");
  Ptr<MmWavePointToPointEpcHelper>  epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
  mmwaveHelper->SetEpcHelper (epcHelper);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph,p2h1;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (MicroSeconds (1)));
  
  
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);


  // Install mmwave Devices to the nodes
  NetDeviceContainer enbDevs = mmwaveHelper->InstallEnbDevice (se.getNode());
  NetDeviceContainer ueDevs = mmwaveHelper->InstallUeDevice (ue.getNode());
  
  // Install the IP stack on the UEs
  // Assign IP address to UEs, and install applications
  internet.Install (ue.getNode());
  //internet.Install (se.getNode());
  NodeContainer uese = NodeContainer (ue.getNode().Get (0), se.getNode().Get (0));
  NodeContainer sehn = NodeContainer ( se.getNode().Get (0),remoteHostContainer.Get(0));
  NetDeviceContainer ueseNet = p2ph.Install (uese);
  NetDeviceContainer sehnNet = p2ph.Install (sehn);
  
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (ueseNet);
  ipv4h.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer internetIpIfaces1 = ipv4h.Assign (sehnNet);
  
  uint16_t sinkPort = 8080;
  Address ueAddress (InetSocketAddress (internetIpIfaces.GetAddress (0), sinkPort)); // interface of n2
  Address snAddress (InetSocketAddress (internetIpIfaces.GetAddress (1), sinkPort)); // interface of n2
  Address snAddressToHN (InetSocketAddress (internetIpIfaces1.GetAddress(0), sinkPort)); // interface of n2
  Address hnAddress (InetSocketAddress (internetIpIfaces1.GetAddress (1), sinkPort)); // interface of n2


  // UE Packet SINK
  PacketSinkHelper uepacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer uesinkApps = uepacketSinkHelper.Install (ue.getNode().Get (0)); //n2 as sink
  uesinkApps.Start (Seconds (0.));
  uesinkApps.Stop (Seconds (stopTime));

  // SE Packet Sink
  PacketSinkHelper sepacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sesinkApps = sepacketSinkHelper.Install (se.getNode().Get (0)); //n2 as sink
  sesinkApps.Start (Seconds (0.));
  sesinkApps.Stop (Seconds (stopTime));

  // HN Paket Sink 
  PacketSinkHelper hnpacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer hnsinkApps = hnpacketSinkHelper.Install (remoteHostContainer.Get(0)); //n2 as sink
  hnsinkApps.Start (Seconds (0.));
  hnsinkApps.Stop (Seconds (stopTime));

   
  Ptr<Socket> UESocket = Socket::CreateSocket (ue.getNode().Get (0), UdpSocketFactory::GetTypeId ()); //source at n0
  Ptr<Socket> SESocket = Socket::CreateSocket (se.getNode().Get (0), UdpSocketFactory::GetTypeId ()); //source at n0
  Ptr<Socket> HNSocket = Socket::CreateSocket (remoteHostContainer.Get (0), UdpSocketFactory::GetTypeId ()); //source at n0

uesinkApps.Get (0)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&RxofUE,UESocket,snAddress));
sesinkApps.Get (0)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&RxofSE,SESocket,hnAddress,ueAddress));
hnsinkApps.Get (0)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&RxofHN,HNSocket,snAddressToHN));

  u8 R1[17];
  se.getR1(R1);
  Simulator::ScheduleWithContext (SESocket->GetNode ()->GetId (),
                                  Seconds (1.0), &sendToUE,
                                  SESocket,Seconds(1),R1,int(0),ueAddress);
                                   /* Simulator::ScheduleWithContext (recvSink->GetNode ()->GetId (),
                                  Seconds (1.0), &sendSUCI,
                                  recvSink, int(1000), int(1), Seconds(1),SUCI);*/
 Ptr<FlowMonitor> flowMonitor;
 FlowMonitorHelper flowHelper;
 flowMonitor = flowHelper.InstallAll();
 

Simulator::Stop (Seconds (stopTime));
  AnimationInterface anim ("new-mac-one-device.xml");
  Simulator::Run ();
  
  flowMonitor->SerializeToXmlFile("new-mac-one-device-flow.xml",true,true);
  Simulator::Destroy ();
  
  
  return 0;
}


