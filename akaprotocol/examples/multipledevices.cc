/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/core-module.h"
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
#include "ns3/serving-aka.h"
#include "ns3/home.h"
#include "ns3/aka.h"
#include "ns3/user-aka.h"
#include "ns3/myheader.h"
#include <iostream>
#include <map>
#include <stdint.h>
using namespace ns3;
using namespace mmwave;
using namespace std;
#define max 20
NS_LOG_COMPONENT_DEFINE ("test");

//UserEqupi ;
//ue[0] = CreateObject<UserEqupi>();
oldAll aue(5);

//aue->add(2);
//std::map<char,int> a;
//a.insert(std::make_pair('6',6));
//aue.add();std::make_pair
//static bool Reckey[]={false,false};
AsciiTraceHelper asciiTraceHelper;
Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("old-fixed-device.txt");

static void
CwndChange (uint32_t lenght, Address iaddr,std::string mode,unsigned int token, uint16_t key, uint32_t length, u8 value[])
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << mode<<"\t"<<token<<"\t"<<lenght << "\t" << iaddr
                        << "\t" << key << "\t" << length << "\t";
  for (unsigned int i = 0; i < length; i++)
    {
      *stream->GetStream () << int(value[i]) << " ";
    }
  *stream->GetStream () << std::endl;
}

static void
sendToUE (Ptr<Socket> socket, Time pktInterval, u8 value[], uint16_t key, Address add)
{
  Ptr<Packet> packet = Create<Packet> (1);

  MyHeader h1;
    h1.SetData (value, uint16_t (1));
    

  h1.SetKey (key);
  // Fill out udpHeader fields appropriately
  packet->AddHeader (h1);
  socket->Connect (add);
  socket->Send (packet);
  //  socket->Close();
  std::cout << "#######\t Msg with " << key << " send ###########\n";
}

static void
RxofSE (Ptr<Socket> socket, Address hnadd[][max], Address ueadd[], Ptr<const Packet> packet,
        const Address &from)
{
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);

  u8 R1[17], value[48], newValue[48];
  uint16_t key = header.GetKey ();
  uint16_t token1 = header.GetToken ();
  header.GetData (value);
std::cout << "Msg Received SE "<<aue.getS(token1)<<"\n";
  if (key == 1)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (16), value);
      aue.getServingN (aue.getS(token1))->setSUCI(value,token1);
       R1[0] =  aue.getServingN (aue.getS(token1))->getsnID();
      // value is SUCI,hres
     // aue.getServingN (aue.getS(token1))->setHRES(value,token1);
     // aue.getServingN (aue.getS(token1))->getR1(R1,token1);
      if(aue.getS(token1) == aue.getSNAttack())
      {
         R1[0] = 0xd2;
      }
      for (unsigned int i = 0; i < 16; i++)
        {
          newValue[i] = value[i];
         
        }
      newValue[16] = R1[0];
      MyHeader h1;

      Ptr<Packet> packet = Create<Packet> (17);
      h1.SetData (newValue, uint16_t (17));
      h1.SetKey (uint16_t (2));
      h1.SetToken(token1);
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (hnadd[aue.getS(token1)][aue.getH(token1)]);
      socket->Send (packet);
      //   socket->Close();
      std::cout << "#######\t Msg with " << 2 << " send by SN  "<<aue.getS(token1)<<"####\n";
    }
  if (key == 3)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (40), value); //Ned to change
     // std::cout << "\n Res verification " ;
      //uint32_t checkRes = aue.getServingN (aue.getS(token1))->verifyRes (value);
      
      aue.getServingN (aue.getS(token1))->setHRES(value,token1);
          // se.setEk(value);
          for (unsigned int i = 0; i < 8; i++)
            {
              newValue[i] = value[i];
            }
          MyHeader h1;
          Ptr<Packet> packet = Create<Packet> (8);
          h1.SetData (newValue, uint16_t (8));
          h1.SetKey (uint16_t (4));
          h1.SetToken(token1);
          packet->AddHeader (h1);
          socket->Connect (ueadd[token1]);
          socket->Send (packet);
          std::cout << "#######\t Msg with " << 4 << " send SN  "<<aue.getS(token1)<<"####\n";
       
    }
  if (key == 5)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (32), value);
     
      uint32_t checkRes = aue.getServingN (aue.getS(token1))->verifyRes (value,newValue);
      std::cout<<"Res Verfiication \t"<<checkRes<<"\n";
      if(checkRes > 0)
      {
         MyHeader h1;
        Ptr<Packet> packet = Create<Packet> (48);
        h1.SetData (newValue,uint16_t(48));
        h1.SetKey (uint16_t(6));
        h1.SetToken(token1);
        packet->AddHeader (h1);
        socket->Connect (hnadd[aue.getS(token1)][aue.getH(token1)]);
        socket->Send (packet);
          std::cout << "#######\t Msg with " << 6 << " send SN  "<<aue.getS(token1)<<"####\n";
     }
      else
      {
        std::cout<<"\nRes verifcartion failed with SE:"<<aue.getS(token1)<<"\tUE :"<<token1<<"\n";
      }
      
     
    }
    if(key == 7)
    {
      CwndChange(kk,from,"SN",aue.getS(token1),key,uint32_t(32),value);
       std::cout<<"Final message received #######\n";
       /*for(unsigned int i=0;i<32;i++)
       {
         std::cout<<value[i]<<" ";
       }*/
       std::cout<<"\n\ndoneeee\n";
    }
}

static void
RxofUE (Ptr<Socket> socket, Address add,unsigned int token,Ptr<const Packet> packet, const Address &from)
{
  std::cout << "Msg Received UE:"<<token<<"\n";
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);
  
  u8 R1[17], decryptKey[32];
  uint16_t key = header.GetKey ();
  header.GetData (R1);
  if (key == 0)
    {
      CwndChange (kk, from,"UE",token, key, uint32_t (1), R1);

      Ptr<Packet> packet = Create<Packet> (16);
      MyHeader h1;
      u8 value1[48];
      aue.getN (uint32_t(token))->setString (R1, value1);
      h1.SetData (value1, uint16_t (16));
      h1.SetKey (uint16_t (1));
      h1.SetToken (uint16_t (token));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
      // socket->Close();
      std::cout << "#######\t Msg with " << 1 << " send by UE  "<<token <<"###\n";
    }
  if (key == 4)
    {
      CwndChange (kk, from,"UE",token, key, uint32_t (8), R1);

      bool macVerification = aue.getN (uint32_t(token))->calculation (R1, decryptKey);
      if(token == aue.getUEAttack())
      {
        macVerification = true;
        std::cout<<"\n Mac changed :"<<token<<std::endl;
      }
      if (macVerification)
        {
          Ptr<Packet> packet = Create<Packet> (32);
         // std::cout << "\n MAcverifid \n";
          MyHeader h1;
          h1.SetData (decryptKey, uint16_t (32));
          h1.SetKey (uint16_t (5));
           h1.SetToken (uint16_t (token));
          // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add);
          socket->Send (packet);
           std::cout << "#######\t Msg with " << 5 << " send by UE  "<<token <<"###\n";
    }
      else
        {
          // Abort
          std::cout << "\n Mac Not verifid "<<token<<"\n";
        }
    }
}

static void
RxofHN (Ptr<Socket> socket, Address add[][max],unsigned int token, Ptr<const Packet> packet, const Address &from)
{
  
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);
  u8 value[48], newValue[48];
  uint16_t key = header.GetKey ();
  uint16_t token1 = header.GetToken ();
  header.GetData (value);
  std::cout << "Msg Received HN :"<<aue.getH(token1)<<"\n";
  
  if (key == 2)
    {
CwndChange (kk, from,"HN",token, key, uint32_t (17), value);
      bool isTrue = aue.getHomeN(aue.getH(token1))->setString (value, newValue);
      if (isTrue)
        {
          Ptr<Packet> packet = Create<Packet> (40);
          MyHeader h1;
          h1.SetData (newValue, uint16_t (40));
          h1.SetKey (uint16_t (3));
          h1.SetToken(token1);
          // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add[aue.getS(token1)][aue.getH(token1)]);
          socket->Send (packet);
          // socket->Close();
          std::cout << "#######\t Msg with " << 3 << " send by HN "<<aue.getH(token1)<<"####\n";
        }
      else
        {
          //ABORT

          Ptr<Packet> packet = Create<Packet> (40);
          MyHeader h1;
          h1.SetData (newValue, uint16_t (40));
          h1.SetKey (uint16_t (3));
          h1.SetToken(token1);
          // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add[aue.getS(token1)][aue.getH(token1)]);
          socket->Send (packet);
          // socket->Close();
          std::cout << "#######\t Msg with " << 3 << " send by HN "<<aue.getH(token1)<<" in fail mode###\n";
        }
    }
     if( key == 6 )
    {
       CwndChange(kk,from,"HN",token,key,uint32_t(48),value);
      bool checkRes = aue.getHomeN(aue.getH(token1))->calculation(value,newValue);
      if(checkRes)
      {
          Ptr<Packet> packet = Create<Packet> (8);
       MyHeader h1;
       h1.SetData (newValue,uint16_t(8));
       h1.SetKey (uint16_t(7));
       h1.SetToken(token1);
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add[aue.getS(token1)][aue.getH(token1)]);
      socket->Send (packet);
     // socket->Close();
      	std::cout<<"#######\t Msg with "<< 7 <<" send by HN  "<<aue.getH(token1)<<"####\n";
      }
      else
      {
        /* code */
      }
      
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
  //double stopTime = 45.9;
  // double simStopTime = 7.00;
  //  Ipv4Address remoteHostAddr;

  // Command line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);
    aue.map();
    aue.getFail();
  //Ptr<MmWaveHelper> mmwaveHelper = CreateObject<MmWaveHelper> ();
  //mmwaveHelper->SetSchedulerType ("ns3::MmWaveFlexTtiMaxWeightMacScheduler");
 // mmwaveHelper->SetSchedulerType ("ns3::MmWaveFlexTtiMacScheduler");
 // Ptr<MmWavePointToPointEpcHelper> epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
 // mmwaveHelper->SetEpcHelper (epcHelper);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

 // Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (2);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (MicroSeconds (1)));

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
      ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  // Install mmwave Devices to the nodes
 // NetDeviceContainer enbDevs[10];
  //NetDeviceContainer ueDevs[10];
  NodeContainer uese[max];
  NetDeviceContainer  ueseNet[max];
  Ipv4AddressHelper ipv4h;
  Ipv4InterfaceContainer internetIpIfaces[max];
  
   
  uint16_t sinkPort = 8080;
  Address ueAddress[max],snAddress[max]; 
  // UE Packet SINK
  PacketSinkHelper uepacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer uesinkApps[max];
   // interface of n2
    ipv4h.SetBase ("10.2.1.0", "255.255.255.0");
    NodeContainer sehn[max][max];
    NetDeviceContainer sehnNet[max][max];
    Ipv4InterfaceContainer internetIpIfaces1[max][max];
    Address snAddressToHN[max][max];
      // interface of n2
  Address hnAddress[max][max];
    
for(unsigned int i=0;i<aue.getServingSize();i++)
{
 // enbDevs[i] = mmwaveHelper->InstallEnbDevice (aue.getServingN(i)->getNode ());
 internet.Install(aue.getServingN(i)->getNode ());
   for(unsigned int j=0;j<2;j++)
   // NetDeviceContainer enbDevs = mmwaveHelper->InstallEnbDevice (.getNode ());
   {
        sehn[i][j] = NodeContainer (aue.getServingN (i)->getNode ().Get (0), remoteHostContainer.Get (j));
        sehnNet[i][j] = p2ph.Install (sehn[i][j]);
         ipv4h.NewNetwork();
        internetIpIfaces1[i][j] = ipv4h.Assign (sehnNet[i][j]);
        snAddressToHN[i][j] =  InetSocketAddress (internetIpIfaces1[i][j].GetAddress (0), sinkPort);
        hnAddress[i][j] =     InetSocketAddress (internetIpIfaces1[i][j].GetAddress (1), sinkPort); // interface of n2

   } 
 
  
}

ipv4h.SetBase ("10.1.1.0", "255.255.255.0","0.0.0.1");
  for(unsigned int i=0; i< aue.getSize();i++)
  {
  //  ueDevs[i] = mmwaveHelper->InstallUeDevice (aue.getN (i)->getNode ());
    internet.Install (aue.getN (i)->getNode ());
    uese[i] = NodeContainer (aue.getN (i)->getNode ().Get (0), aue.getServingN (aue.getS(i))->getNode ().Get (0));
    ueseNet[i] = p2ph.Install (uese[i]);
    ipv4h.NewNetwork();
    internetIpIfaces[i] = ipv4h.Assign (ueseNet[i]);
    ueAddress[i] = InetSocketAddress (internetIpIfaces[i].GetAddress (0), sinkPort);
    snAddress[i] = InetSocketAddress (internetIpIfaces[i].GetAddress (1), sinkPort);
    uesinkApps[i] = uepacketSinkHelper.Install (aue.getN (i)->getNode ().Get (0)); //n2 as sink
    uesinkApps[i].Start (Seconds (0.));
    uesinkApps[i].Stop (Seconds (20.));
     
  }
  
  Ptr<Socket> UESocket[max],SESocket[max],HNSocket[max];
  PacketSinkHelper sepacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));

  // HN Paket Sink
  PacketSinkHelper hnpacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
 
   ApplicationContainer sesinkApps[max],hnsinkApps[max];
  for(unsigned int i=0;i<aue.getSize();i++)
  {
    UESocket[i] = Socket::CreateSocket (aue.getN (i)->getNode ().Get (0),
                                               UdpSocketFactory::GetTypeId ()); //source at n0
      uesinkApps[i].Get (0)->TraceConnectWithoutContext ("Rx",
                                                  MakeBoundCallback (&RxofUE, UESocket[i], snAddress[i],i));
  
  }

  for(unsigned int i=0;i<aue.getServingSize();i++)
  {
      // SE Packet Sink
     SESocket[i] =
      Socket::CreateSocket (aue.getServingN(i)->getNode ().Get (0), UdpSocketFactory::GetTypeId ()); //source at n0
 
    sesinkApps[i] = sepacketSinkHelper.Install (aue.getServingN(i)->getNode ().Get (0)); //n2 as sink
    sesinkApps[i].Start (Seconds (0.));
    sesinkApps[i].Stop (Seconds (20.));
    sesinkApps[i].Get (0)->TraceConnectWithoutContext (
      "Rx", MakeBoundCallback (&RxofSE, SESocket[i], hnAddress, ueAddress));
  
  }

  for(unsigned int i=0;i<2;i++)
  {
     HNSocket[i] = Socket::CreateSocket (remoteHostContainer.Get (i),
                                               UdpSocketFactory::GetTypeId ()); //source at n0
    hnsinkApps[i] = hnpacketSinkHelper.Install (remoteHostContainer.Get (i));
    hnsinkApps[i].Start (Seconds (0.));
    hnsinkApps[i].Stop (Seconds (20.));
    hnsinkApps[i].Get (0)->TraceConnectWithoutContext (
      "Rx", MakeBoundCallback (&RxofHN, HNSocket[i], snAddressToHN,i));

  }

       //n2 as sink
       u8 ID[1];
 
  
  for(unsigned int i=0;i<aue.getSize();i++)
  {
     ID[0] = aue.getServingN(aue.getS(i))->getsnID();
  Simulator::ScheduleWithContext (SESocket[aue.getS(i)]->GetNode ()->GetId (), Seconds (1.0), &sendToUE,
                                  SESocket[aue.getS(i)], Seconds (1+i), ID, int(0), ueAddress[i]);
  }
  /* Simulator::ScheduleWithContext (recvSink->GetNode ()->GetId (),
                                  Seconds (1.0), &sendSUCI,
                                  recvSink, int(1000), int(1), Seconds(1),SUCI);*/
Ptr<FlowMonitor> flowMonitor;
 FlowMonitorHelper flowHelper;
 flowMonitor = flowHelper.InstallAll();
 
  Simulator::Stop (Seconds ( 20));
  AnimationInterface anim ("old-fixed-device.xml");
  Simulator::Run ();
  
  flowMonitor->SerializeToXmlFile("old-fixed-device-flow.xml",true,true);
  Simulator::Destroy ();

  return 0;
}
