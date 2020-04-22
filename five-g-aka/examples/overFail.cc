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
#include "ns3/serving-network-aka.h"
#include "ns3/home-network-aka.h"
#include "ns3/all-user.h"
#include "ns3/user-equipment-aka.h"
#include "ns3/myheader.h"
#include <iostream>
#include <map>
#include <stdint.h>
using namespace ns3;
using namespace mmwave;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("test");

//UserEqupi ;
//ue[0] = CreateObject<UserEqupi>();
AllUser aue(3);

//aue->add(2);
//std::map<char,int> a;
//a.insert(std::make_pair('6',6));
//aue.add();std::make_pair
//static bool Reckey[]={false,false};
uint16_t length[] = {17, 48, 33};
AsciiTraceHelper asciiTraceHelper;
Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("five-g-test.txt");

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
  Ptr<Packet> packet = Create<Packet> (1000);

  MyHeader h1;
    h1.SetData (value, uint16_t (17));
    

  h1.SetKey (key);
  // Fill out udpHeader fields appropriately
  packet->AddHeader (h1);
  socket->Connect (add);
  socket->Send (packet);
  //  socket->Close();
  std::cout << "#######\t Msg with " << key << " send ###########\n";
}

static void
RxofSE (Ptr<Socket> socket, Address hnadd[][10], Address ueadd[], Ptr<const Packet> packet,
        const Address &from)
{
  std::cout << "Msg Received SE\n";
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);

  u8 R1[17], value[48], newValue[33];
  uint16_t key = header.GetKey ();
  uint16_t token1 = header.GetToken ();
  header.GetData (value);

  if (key == 1)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (48), value);
      // value is SUCI,hres
      aue.getServingN (aue.getS(token1))->setHRES(value,token1);
      aue.getServingN (aue.getS(token1))->getR1(R1,token1);
      for (unsigned int i = 0; i < 16; i++)
        {
          newValue[i] = value[i];
          newValue[i + 16] = R1[i];
        }
      newValue[32] = R1[16];
      MyHeader h1;

      Ptr<Packet> packet = Create<Packet> (1000);
      h1.SetData (newValue, uint16_t (33));
      h1.SetKey (uint16_t (2));
      h1.SetToken(token1);
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (hnadd[aue.getS(token1)][aue.getH(token1)]);
      socket->Send (packet);
      //   socket->Close();
      std::cout << "#######\t Msg with " << 2 << " send ###########"<<aue.getS(token1)<<"\n";
    }
  if (key == 3)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (40), value); //Ned to change
      std::cout << "\n Res verification " ;
      if(aue.getSNAttack() == aue.getS(token1))
      {
        value[10] = !value[10];
      }
      uint32_t checkRes = aue.getServingN (aue.getS(token1))->verifyRes (value);
      std::cout << "\n Res verification " << checkRes << std::endl;
      if (checkRes)
        {
          // se.setEk(value);
          for (unsigned int i = 0; i < 8; i++)
            {
              newValue[i] = value[i];
            }
          MyHeader h1;
          Ptr<Packet> packet = Create<Packet> (1000);
          h1.SetData (newValue, uint16_t (8));
          h1.SetKey (uint16_t (4));
          packet->AddHeader (h1);
          socket->Connect (ueadd[token1]);
          socket->Send (packet);
          std::cout << "#######\t Msg with " << 4 << " send ###########"<<aue.getS(token1)<<"\n";
        }
      else
        {
          // Abort
          std::cout<<"#####\nRes not verified #######  "<<token1<<std::endl;
        }
    }
  if (key == 5)
    {
      CwndChange (kk, from,"SN",aue.getS(token1), key, uint32_t (16), value);
      std::cout << "\n\n DECRYPT KEY\n";
      for (unsigned int i = 0; i < 16; i++)
        {
          std::cout << int(value[i]) << " ";
        }
      std::cout << "\n\n##########ALL process over###";
    }
}

static void
RxofUE (Ptr<Socket> socket, Address add,unsigned int token,Ptr<const Packet> packet, const Address &from)
{
  std::cout << "Msg Received UE:"<<token<<"\n";
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);
  
  u8 R1[17], decryptKey[16];
  uint16_t key = header.GetKey ();
  header.GetData (R1);
  if (key == 0)
    {
      CwndChange (kk, from,"UE",token, key, uint32_t (17), R1);

      Ptr<Packet> packet = Create<Packet> (1000);
      MyHeader h1;
      u8 value1[48];
      aue.getN (uint32_t(token))->setString (R1, value1);
      h1.SetData (value1, uint16_t (48));
      h1.SetKey (uint16_t (1));
      h1.SetToken (uint16_t (token));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Connect (add);
      socket->Send (packet);
      // socket->Close();
      std::cout << "#######\t Msg with " << 1 << " send ###########\n";
    }
  if (key == 4)
    {
      CwndChange (kk, from,"UE",token, key, uint32_t (8), R1);
      if(aue.getUEAttack() == token)
      {
        R1[4] = !R1[4];
      }
      bool macVerification = aue.getN (uint32_t(token))->calculation (R1, decryptKey);
      if (macVerification)
        {
          Ptr<Packet> packet = Create<Packet> (1000);
          std::cout << "\n MAcverifid \n";
          MyHeader h1;
          h1.SetData (decryptKey, uint16_t (16));
          h1.SetKey (uint16_t (5));
           h1.SetToken (uint16_t (token));
          // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add);
          socket->Send (packet);
        }
      else
        {
          // Abor
          std::cout<<"######Mac Not verified #### "<<token<<std::endl;
        }
    }
}

static void
RxofHN (Ptr<Socket> socket, Address add[][10],unsigned int token, Ptr<const Packet> packet, const Address &from)
{
  std::cout << "Msg Received HN \n";
  MyHeader header;
  uint32_t kk = packet->PeekHeader (header);
  u8 value[33], newValue[48];
  uint16_t key = header.GetKey ();
  uint16_t token1 = header.GetToken ();
  header.GetData (value);
  CwndChange (kk, from,"HN",token, key, uint32_t (30), value);
  if (key == 2)
    {

      bool isTrue = aue.getHomeN(aue.getH(token1))->setString (value, newValue);
      if (isTrue)
        {
          Ptr<Packet> packet = Create<Packet> (1000);
          MyHeader h1;
          h1.SetData (newValue, uint16_t (40));
          h1.SetKey (uint16_t (3));
          h1.SetToken(token1);
          // Fill out udpHeader fields appropriately
          packet->AddHeader (h1);
          socket->Connect (add[aue.getS(token1)][aue.getH(token1)]);
          socket->Send (packet);
          // socket->Close();
          std::cout << "#######\t Msg with " << 3 << " send ###########\n";
        }
      else
        {
          //ABORT
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
  NodeContainer uese[10];
  NetDeviceContainer  ueseNet[10];
  Ipv4AddressHelper ipv4h;
  Ipv4InterfaceContainer internetIpIfaces[10];
  
   
  uint16_t sinkPort = 8080;
  Address ueAddress[10],snAddress[10]; 
  // UE Packet SINK
  PacketSinkHelper uepacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer uesinkApps[10];
   // interface of n2
    ipv4h.SetBase ("10.2.1.0", "255.255.255.0");
    NodeContainer sehn[10][10];
    NetDeviceContainer sehnNet[10][10];
    Ipv4InterfaceContainer internetIpIfaces1[10][10];
    Address snAddressToHN[10][10];
      // interface of n2
  Address hnAddress[10][10];
    
  std::cout<<"hi";
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
  
  Ptr<Socket> UESocket[10],SESocket[10],HNSocket[10];
  PacketSinkHelper sepacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));

  // HN Paket Sink
  PacketSinkHelper hnpacketSinkHelper ("ns3::UdpSocketFactory",
                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
 
   ApplicationContainer sesinkApps[10],hnsinkApps[10];
  for(unsigned int i=0;i<aue.getSize();i++)
  {
    UESocket[i] = Socket::CreateSocket (aue.getN (i)->getNode ().Get (0),
                                               UdpSocketFactory::GetTypeId ()); //source at n0
      uesinkApps[i].Get (0)->TraceConnectWithoutContext ("Rx",
                                                  MakeBoundCallback (&RxofUE, UESocket[i], snAddress[i],i));
  
  }

  std::cout<<"hi";
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

  std::cout<<"hi";
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

  std::cout<<"hi";
       //n2 as sink
  u8 R1[10][17];
  for(unsigned int i=0;i<aue.getSize();i++)
  {
    aue.getServingN(aue.getS(i))->setR1(i);
    aue.getServingN(aue.getS(i))->getR1(R1[i],uint16_t(i));
  Simulator::ScheduleWithContext (SESocket[aue.getS(i)]->GetNode ()->GetId (), Seconds (1.0), &sendToUE,
                                  SESocket[aue.getS(i)], Seconds (1+i), R1[i], int(0), ueAddress[i]);
  }
  /* Simulator::ScheduleWithContext (recvSink->GetNode ()->GetId (),
                                  Seconds (1.0), &sendSUCI,
                                  recvSink, int(1000), int(1), Seconds(1),SUCI);*/
Ptr<FlowMonitor> flowMonitor;
 FlowMonitorHelper flowHelper;
 flowMonitor = flowHelper.InstallAll();
 
  Simulator::Stop (Seconds ( 20));
  AnimationInterface anim ("fiveg.xml");
  Simulator::Run ();
  
  flowMonitor->SerializeToXmlFile("test-send-msg.xml",true,true);
  Simulator::Destroy ();

  return 0;
}
