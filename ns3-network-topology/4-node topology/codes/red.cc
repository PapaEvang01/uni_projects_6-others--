/*
  Network topology:

A--------------C--------------D
               |
               |
               |
               |
               B

A--C: 1+z Mbps / 10 ms delay  
B--C: 1+z Mbps / 5 ms delay  
C--D: 10-z Mbps / 10 ms delay  
Queue at C--D: size 10+y  

---------------------------------------------------------
Simulation Overview:
---------------------------------------------------------
This NS-3 simulation models TCP congestion behavior in a simple 4-node network
with RED (Random Early Detection) queue management enabled at the C–D bottleneck.

- Nodes A and B send bulk TCP traffic to node D.
- RED is used instead of the default DropTail queue on the C–D link.
- RED helps avoid congestion by probabilistically dropping packets *before*
  the queue overflows, based on average queue size.
- The configuration uses gentle mode, average packet size, and min/max thresholds.

Key RED parameters:
- MinTh = 2 packets, MaxTh = 8 packets
- QW = 0.002 (exponential weighted moving average factor)
- Gentle = true, Wait = true

Outputs:
- sourceA.cwnd / sourceB.cwnd: Congestion window over time
- ASCII trace file: Packet events
- Console summary of simulation parameters and performance

To run with custom settings:
  ./waf --run "red --linkBW=6 --CDlinkBW=5 --queuesize=17 --runtime=20"
*/

#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-helper.h"

using namespace ns3;


std::string fileNameRoot1 = "sourceA";    // base name for trace files, etc
std::string fileNameRoot2 = "sourceB";    // base name for trace files, etc

// Definition of trace sink callback
void CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " " <<  newCwnd << std::endl;
}

static void
TraceCwnd ()    // Trace changes to the congestion window
{
  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (fileNameRoot1 + ".cwnd");
  Ptr<OutputStreamWrapper> stream2 = ascii.CreateFileStream (fileNameRoot2 + ".cwnd");

  // Connect the trace sink (CwndChange) to the appropriate trace source ("/.../CongestionWindow")
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback (&CwndChange,stream));

  Config::ConnectWithoutContext ("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback (&CwndChange,stream2));

}

int main (int argc, char *argv[])
{
  // Set simulation parameters
  int tcpSegmentSize = 1000;
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (tcpSegmentSize));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (0));
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
//  Config::SetDefault ("ns3::RttEstimator::MinRTO", TimeValue (MilliSeconds (500)));
  // my
  //LogComponentEnable ("RedQueueDisc", LOG_LEVEL_INFO);
  unsigned int runtime = 20;   // seconds
  int delayAC = 10;            // ms
  int delayBC = 5;            // ms
  int delayCD = 10;            // ms
  double CDlinkBW = 5;    // Mbps
  double linkBW = 6;          // Mbps
  uint32_t queuesize = 17;
  uint32_t maxBytes = 0;       // 0 means "unlimited"
  uint32_t meanPktSize = 500;

  CommandLine cmd;
  cmd.AddValue("linkBW", "Bandwidth of A-C and B-C links (Mbps)", linkBW);
  cmd.AddValue("CDlinkBW", "Bandwidth of C-D link (Mbps)", CDlinkBW);
  cmd.AddValue("queuesize", "Queue size at C-D (in packets)", queuesize);
  cmd.AddValue("runtime", "Simulation runtime in seconds", runtime);
  cmd.Parse(argc, argv);
  
  std::cout << "queuesize=" << queuesize << ", linkBW=" << linkBW << ", CDlinkBW=" << CDlinkBW << std::endl;

  Ptr<Node> A = CreateObject<Node> ();
  Ptr<Node> B = CreateObject<Node> ();
  Ptr<Node> C = CreateObject<Node> ();
  Ptr<Node> D = CreateObject<Node> ();
 
// RED params
  //NS_LOG_INFO ("Set RED params");
  Config::SetDefault ("ns3::RedQueueDisc::MaxSize", StringValue ("1000p"));
  Config::SetDefault ("ns3::RedQueueDisc::MeanPktSize", UintegerValue (meanPktSize));
  Config::SetDefault ("ns3::RedQueueDisc::Wait", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::Gentle", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue (0.002));
  Config::SetDefault ("ns3::RedQueueDisc::MinTh", DoubleValue (2));
  Config::SetDefault ("ns3::RedQueueDisc::MaxTh", DoubleValue (8));


  // use PointToPointChannel and PointToPointNetDevice
  NetDeviceContainer devAC, devBC, devCD;
  PointToPointHelper AC, BC, CD;

  // create point-to-point link from A to C
  AC.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (linkBW * 1000 * 1000)));
  AC.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayAC)));
  devAC = AC.Install(A, C);

  // create point-to-point link from B to C
  BC.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (linkBW * 1000 * 1000)));
  BC.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayBC)));
  devBC = BC.Install(B, C);

  // create point-to-point link from C to D
  CD.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (CDlinkBW * 1000 * 1000)));
  CD.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayCD)));
  //ean exete egkatestimeni tin ekdosi 3.27 valte se sxolio tis 2 epomenes entoles kai vgalte apo sxolio tin triti
    std::string maxsize = std::to_string(queuesize).append("p");
  
   // CD.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue(maxsize));
  //CD.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue(queuesize));
   devCD = CD.Install(C,D);
   


   // pointers to netdevices
    Ptr<NetDevice> Adev = devAC.Get(0) ;
    Ptr<NetDevice> Bdev = devBC.Get(0) ;

    Ptr<NetDevice> Cdev1 = devAC.Get(1) ;
    Ptr<NetDevice> Cdev2 = devBC.Get(1) ;
    Ptr<NetDevice> Cdev3 = devCD.Get(0) ;

    Ptr<NetDevice> Ddev1 = devCD.Get(1) ;
   
  
    
  InternetStackHelper internet;
  internet.Install (A);
  internet.Install (B);
  internet.Install (C);
  internet.Install (D);
  

  // Assign IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interfaces;
  ipv4Interfaces.Add(ipv4.Assign (devAC));
  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  ipv4Interfaces.Add(ipv4.Assign(devBC));
  ipv4.SetBase ("10.0.2.0", "255.255.255.0");
  ipv4Interfaces.Add(ipv4.Assign(devCD));
  ipv4.SetBase ("10.0.3.0", "255.255.255.0");

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Ptr<Ipv4> sink4 = D->GetObject<Ipv4>();  // gets node D's IPv4 subsystem
  Ipv4Address sinkIP = sink4->GetAddress(1,0).GetLocal();

// Remove traffic controller, so that no queuing discipline is used and cwnd fluctuations can  be clearly observed
   TrafficControlHelper tch, tchRed;
   tch.Uninstall(Adev);
   tch.Uninstall(Bdev);
   tch.Uninstall(Cdev1);
   tch.Uninstall(Cdev2);
   tch.Uninstall(Cdev3);
   tch.Uninstall(Ddev1);
  

//my code//
tchRed.SetRootQueueDisc("ns3::RedQueueDisc", "MaxSize",StringValue(maxsize));
QueueDiscContainer queueDiscs = tchRed.Install (Cdev3);
// end

  // create a sink on D
  uint16_t port = 80;
  Address sinkaddr(InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sink ("ns3::TcpSocketFactory", sinkaddr);
  ApplicationContainer sinkApp = sink.Install (D);
  sinkApp.Start (Seconds (0.01));
  // the following means the receiver will run 1 min longer than the sender app.
  sinkApp.Stop (Seconds (runtime + 60.0));

  Address sinkAddr(InetSocketAddress(sinkIP, port));

//source A
  BulkSendHelper sourceAhelper ("ns3::TcpSocketFactory",  sinkAddr);
  sourceAhelper.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  sourceAhelper.SetAttribute ("SendSize", UintegerValue (tcpSegmentSize));
  ApplicationContainer sourceAppsA = sourceAhelper.Install (A);
  sourceAppsA.Start (Seconds (0.0));
  sourceAppsA.Stop (Seconds (runtime));

//source B
  BulkSendHelper sourceBhelper ("ns3::TcpSocketFactory",  sinkAddr);
  sourceBhelper.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  sourceBhelper.SetAttribute ("SendSize", UintegerValue (tcpSegmentSize));
  ApplicationContainer sourceAppsB = sourceBhelper.Install (B);
  sourceAppsB.Start (Seconds (0.0));
  sourceAppsB.Stop (Seconds (runtime));

  // Set up tracing
  AsciiTraceHelper ascii;
  std::string tfname = "lab-assignment.tr";
  AC.EnableAsciiAll (ascii.CreateFileStream (tfname));
  // Setup tracing for cwnd
  Simulator::Schedule(Seconds(0.01),&TraceCwnd);       // this Time cannot be 0.0


  Simulator::Stop (Seconds (runtime+60));
  Simulator::Run ();

  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApp.Get (0));
  std::cout << "Total Bytes Received from sink: " << sink1->GetTotalRx () << std::endl;
  std::cout << "\n=== Simulation Summary ===" << std::endl;
  std::cout << "Simulation Time: " << runtime << " s" << std::endl;
  std::cout << "TCP Segment Size: " << tcpSegmentSize << " bytes" << std::endl;
  std::cout << "Link A-C/B-C Bandwidth: " << linkBW << " Mbps" << std::endl;
  std::cout << "Link C-D Bandwidth: " << CDlinkBW << " Mbps" << std::endl;
  std::cout << "Queue Size at C-D: " << queuesize << " packets" << std::endl;
  std::cout << "RED Parameters: MinTh=2, MaxTh=8, QW=0.002, Gentle=1, Wait=1" << std::endl;

  return 0;
}
