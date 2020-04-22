/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
#include "ns3/five-g-aka.h"
#include "ns3/custom-data-tag.h"
#include "ns3/serving-network-aka.h"
#include "ns3/user-equipment-aka.h"
#include "ns3/myheader.h"
#include <iostream>

using namespace ns3;
using namespace mmwave;
NS_LOG_COMPONENT_DEFINE ("test");

  UserEqupi ue;
  ServingNetworkAka se(u8(0x56));
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("five-g-test.txt");

static void
CwndChange (uint32_t lenght,InetSocketAddress iaddr,uint16_t key,uint32_t length,u8 value[])
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << lenght << "\t" << iaddr.GetIpv4().Get()<<"\t"<< key<<"\t"<<length<<"\t";
  for(unsigned int i=0;i<length;i++)
  {
    *stream->GetStream () <<value[i];
   }
    *stream->GetStream ()<<std::endl;
}
void ReceivePacketSN (Ptr<Socket> socket)
{
  if(Ptr<Packet> packet=socket->Recv()){

		std::cout<<"Msg Received \n";
    MyHeader header;
    uint32_t kk=packet->PeekHeader (header);
    Address addr;
    socket->GetSockName(addr);
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom(addr);
    //std::cout<<iaddr.GetIpv4().Get();

    u8 value[48];
    uint16_t key = header.GetKey();
    header.GetData(value);
    CwndChange(kk,iaddr,key,uint32_t(16),value);
    if(key == 1)
    {
        // value is SUCI
        se.setHRES(value);

    }   
}
}
/*static void sendSUCI (Ptr<Socket> socket, uint32_t pktSize,
                             uint32_t pktCount, Time pktInterval, u8 suci[])
{
  if (pktCount > 0)
    {
      Ptr<Packet> packet = Create<Packet> (pktSize);
      //Ptr<MyHeader> header = CreateObject<MyHeader>();
      MyHeader h1;
      h1.SetData (suci,uint16_t(16));
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Send (packet);

      	std::cout<<"Send SUCI\n";
    }
  else
    {
      socket->Close ();
    }
}*/
static void sendToUE (Ptr<Socket> socket, uint32_t pktSize,
                             uint32_t pktCount, Time pktInterval,u8 value[],uint16_t key)
{
  if (pktCount > 0)
    {
      Ptr<Packet> packet = Create<Packet> (pktSize);
      uint16_t length[]={17,48};
      MyHeader h1;
      h1.SetData (value,uint16_t(length[key]));
      h1.SetKey (key);
      // Fill out udpHeader fields appropriately
      packet->AddHeader (h1);
      socket->Send (packet);

      	std::cout<<"#######\t Msg with "<< key <<" send ###########\n";
    }
  else
    {
      socket->Close ();
    }
}

void ReceivePacket (Ptr<Socket> socket)
{
	if(Ptr<Packet> packet=socket->Recv()){

		std::cout<<"Msg Received \n";
    MyHeader header;
    uint32_t kk=packet->PeekHeader (header);

   
    Address addr;
    socket->GetSockName(addr);
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom(addr);
    //std::cout<<iaddr.GetIpv4().Get();

    
    
    u8 R1[16],value[48];
    uint16_t key = header.GetKey();
    header.GetData(R1);
    CwndChange(kk,iaddr,key,uint32_t(16),R1);
    if(key == 0)
    {
       ue.setString(R1,value);
      /* for(unsigned int i=0;i<48;i++)
      {
        std::cout<<int(value[i])<<" "<<std::endl;
      }*/
    }
  // SEND SUCI To SN
    Simulator::ScheduleWithContext (socket->GetNode ()->GetId (),
                                  Seconds (1.0), &sendToUE,
                                  socket, int(1000), int(1), Seconds(1),value,int(1));
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
  double stopTime = 5.9;
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
  
  p2h1.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2h1.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2h1.SetChannelAttribute ("Delay", TimeValue (MicroSeconds (1)));
  
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);


  // Install mmwave Devices to the nodes
  NetDeviceContainer enbDevs = mmwaveHelper->InstallEnbDevice (se.getNode());
  NetDeviceContainer ueDevs = mmwaveHelper->InstallUeDevice (ue.getNode());
  NodeContainer total ;
  total.Add(ue.getNode());
  total.Add(se.getNode());
  total.Add(remoteHostContainer);
  // Install the IP stack on the UEs
  // Assign IP address to UEs, and install applications
  internet.Install (ue.getNode());
  //internet.Install (se.getNode());
  NetDeviceContainer internetDevices = p2ph.Install (total);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  
  // interface 0 is localhost, 1 is the p2p device
  //Ipv4InterfaceContainer remoteHostAddr = ipv4h.Assign(NetDeviceContainer(p2ph.Install (remoteHostContainer.Get(0))));

  //Ipv4InterfaceContainer ueIpIface;
  //ueIpIface = ipv4h.Assign (NetDeviceContainer (ueDevs));
  Address sinkAddress (InetSocketAddress (internetIpIfaces.GetAddress (0), 80));
  //mmwaveHelper->AttachToClosestEnb (ueDevs, enbDevs);
  //mmwaveHelper->EnableTraces ();


// UE SOCKET
TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
Ptr<Socket> recvSink = Socket::CreateSocket (ue.getNode().Get(0), tid);
InetSocketAddress local = InetSocketAddress (internetIpIfaces.GetAddress (0), 80);
InetSocketAddress Tolocal = InetSocketAddress (internetIpIfaces.GetAddress (1), 80);
 
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
  recvSink->SetAllowBroadcast (false);
  recvSink->Connect (Tolocal);
 

 //SN Socket
 Ptr<Socket> source = Socket::CreateSocket (se.getNode().Get (0), tid);

  InetSocketAddress remote = InetSocketAddress (internetIpIfaces.GetAddress (0), 80);
  InetSocketAddress Toremote = InetSocketAddress (internetIpIfaces.GetAddress (1), 80);
 
  source->SetAllowBroadcast (false);
  source->Connect (remote);
  source->Bind (Toremote);
  source->SetRecvCallback (MakeCallback (&ReceivePacketSN));


/* UE SOCKET
//TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");*/
/*Ptr<Socket> remoteHostSocket = Socket::CreateSocket (remoteHostContainer.Get(0), tid);
InetSocketAddress snA1 = InetSocketAddress (internetIpIfaces.GetAddress (2), 80);
InetSocketAddress snA2 = InetSocketAddress (internetIpIfaces.GetAddress (2), 80);
 
  remoteHostSocket->Bind (snA1);
  remoteHostSocket->SetRecvCallback (MakeCallback (&ReceivePacket));
  remoteHostSocket->SetAllowBroadcast (false);
  remoteHostSocket->Connect (snA2);
 
*/
 //SN Socket
 /*Ptr<Socket> pnSocket2 = Socket::CreateSocket (se.getNode().Get (0), tid);

  //InetSocketAddress snA11 = InetSocketAddress (internetIpIfaces1.GetAddress (0), 80);
  //InetSocketAddress Toremote = InetSocketAddress (internetIpIfaces1.GetAddress (1), 80);
 
  pnSocket2->SetAllowBroadcast (false);
  pnSocket2->Connect (snA1);
  //pnSocket2->Bind (snA2);
  pnSocket2->SetRecvCallback (MakeCallback (&ReceivePacketSN));

*/
 //HN Socket
 /*Ptr<Socket> hnsocket = Socket::CreateSocket (remoteHostContainer.Get (0), tid);

  InetSocketAddress hnFirst  = InetSocketAddress (internetIpIfaces.GetAddress (1), 80);
  InetSocketAddress hnsecond = InetSocketAddress (internetIpIfaces.GetAddress (0), 80);
 
  hnsocket->SetAllowBroadcast (false);
  hnsocket->Connect (hnFirst);
  hnsocket->Bind (hnsecond);
  hnsocket->SetRecvCallback (MakeCallback (&ReceivePacketSN));*/
 //u8 SUCI[]={0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac};
  // Generate First random number
  u8 R1[17];
  se.getR1(R1);
  R1[16] = se.getsnID(); 
  Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                  Seconds (1.0), &sendToUE,
                                  source, int(1000), int(1), Seconds(1),R1,int(0));
                                   /* Simulator::ScheduleWithContext (recvSink->GetNode ()->GetId (),
                                  Seconds (1.0), &sendSUCI,
                                  recvSink, int(1000), int(1), Seconds(1),SUCI);*/

Simulator::Stop (Seconds (stopTime+10));
  AnimationInterface anim ("anim1.xml");
  Simulator::Run ();
  Simulator::Destroy ();
  
  
  return 0;// Set the default gateway for the UE
 /* Ptr<Node> ueNode = ueNodes.Get (0);
  Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
  ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

  // Install and start applications on UEs and remote host
  uint16_t sinkPort = 20000;

  Address sinkAddress (InetSocketAddress (ueIpIface.GetAddress (0), sinkPort));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (ueNodes.Get (0));
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (simStopTime));

  Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (remoteHostContainer.Get (0), TcpSocketFactory::GetTypeId ());
//Add Application

   Ptr<fiveGAka> app = CreateObject<fiveGAka> ();
  app->Setup (ns3TcpSocket, sinkAddress, 1000, 500, DataRate ("30Mb/s"));
  remoteHostContainer.Get (0)->AddApplication (app);
 AsciiTraceHelper asciiTraceHelper;
 // Ptr<OutputStreamWrapper> stream1 = asciiTraceHelper.CreateFileStream ("mmWave-tcp-window.txt");
  //ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, stream1));

  Ptr<OutputStreamWrapper> stream2 = asciiTraceHelper.CreateFileStream ("mmWave-tcp-data.txt");
  sinkApps.Get (0)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&Rx, stream2));
 //sinkApps.Get (1)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&Rx1, stream2));
//sinkApps.Get (2)->TraceConnectWithoutContext ("Rx",MakeBoundCallback (&Rx2, stream2));

  app->SetStartTime (Seconds (0.1));
  app->SetStopTime (Seconds (stopTime));
 p2ph.EnablePcapAll ("mmwave-sgi-capture");

  */
}


