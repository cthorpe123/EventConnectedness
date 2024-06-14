#ifndef _EventAssembler_cxx_
#define _EventAssembler_cxx_

#include "EventAssembler.h"

using namespace Connectedness;

//////////////////////////////////////////////////////////////////////////////////////////////

void EventAssembler::SetFile(std::string infilename){

   f_in = TFile::Open(infilename.c_str());
   f_in->GetObject("wire/WireTree",t_in);
   nEvents = t_in->GetEntries();

   t_in->SetBranchStatus("EventID",1);
   t_in->SetBranchStatus("run",1);
   t_in->SetBranchStatus("subrun",1);
   t_in->SetBranchStatus("event",1);
   t_in->SetBranchStatus("TrackIndex",1);
   t_in->SetBranchStatus("TrackStart_X",1);
   t_in->SetBranchStatus("TrackStart_Y",1);
   t_in->SetBranchStatus("TrackStart_Z",1);
   t_in->SetBranchStatus("TrackDir_X",1);
   t_in->SetBranchStatus("TrackDir_Y",1);
   t_in->SetBranchStatus("TrackDir_Z",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane0",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane0",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane1",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane1",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane2",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane2",1);
   t_in->SetBranchStatus("Wire_Channel_Plane0",1);
   t_in->SetBranchStatus("Wire_Tick_Plane0",1);
   t_in->SetBranchStatus("Wire_Signal_Plane0",1);
   t_in->SetBranchStatus("Wire_Channel_Plane1",1);
   t_in->SetBranchStatus("Wire_Tick_Plane1",1);
   t_in->SetBranchStatus("Wire_Signal_Plane1",1);
   t_in->SetBranchStatus("Wire_Channel_Plane2",1);
   t_in->SetBranchStatus("Wire_Tick_Plane2",1);
   t_in->SetBranchStatus("Wire_Signal_Plane2",1);

   t_in->SetBranchAddress("EventID", &EventID);
   t_in->SetBranchAddress("run", &run);
   t_in->SetBranchAddress("subrun", &subrun);
   t_in->SetBranchAddress("event", &event);
   t_in->SetBranchAddress("TrackIndex", &TrackIndex);
   t_in->SetBranchAddress("TrackStart_X", &TrackStart_X);
   t_in->SetBranchAddress("TrackStart_Y", &TrackStart_Y);
   t_in->SetBranchAddress("TrackStart_Z", &TrackStart_Z);
   t_in->SetBranchAddress("TrackDir_X", &TrackDir_X);
   t_in->SetBranchAddress("TrackDir_Y", &TrackDir_Y);
   t_in->SetBranchAddress("TrackDir_Z", &TrackDir_Z);

   Wire_Channel.resize(kMAXPlane,0);
   Wire_Tick.resize(kMAXPlane,0);
   Wire_Signal.resize(kMAXPlane,0);
   TrackStart_Channel.resize(kMAXPlane,0);
   TrackStart_Time.resize(kMAXPlane,0);

   t_in->SetBranchAddress("Wire_Channel_Plane0", &Wire_Channel.at(kPlane0));
   t_in->SetBranchAddress("Wire_Tick_Plane0", &Wire_Tick.at(kPlane0));
   t_in->SetBranchAddress("Wire_Signal_Plane0", &Wire_Signal.at(kPlane0));
   t_in->SetBranchAddress("Wire_Channel_Plane1", &Wire_Channel.at(kPlane1));
   t_in->SetBranchAddress("Wire_Tick_Plane1", &Wire_Tick.at(kPlane1));
   t_in->SetBranchAddress("Wire_Signal_Plane1", &Wire_Signal.at(kPlane1));
   t_in->SetBranchAddress("Wire_Channel_Plane2", &Wire_Channel.at(kPlane2));
   t_in->SetBranchAddress("Wire_Tick_Plane2", &Wire_Tick.at(kPlane2));
   t_in->SetBranchAddress("Wire_Signal_Plane2", &Wire_Signal.at(kPlane2));

   t_in->SetBranchAddress("TrackStart_Channel_Plane0", &TrackStart_Channel.at(kPlane0));
   t_in->SetBranchAddress("TrackStart_Time_Plane0", &TrackStart_Time.at(kPlane0));
   t_in->SetBranchAddress("TrackStart_Channel_Plane1", &TrackStart_Channel.at(kPlane1));
   t_in->SetBranchAddress("TrackStart_Time_Plane1", &TrackStart_Time.at(kPlane1));
   t_in->SetBranchAddress("TrackStart_Channel_Plane2", &TrackStart_Channel.at(kPlane2));
   t_in->SetBranchAddress("TrackStart_Time_Plane2", &TrackStart_Time.at(kPlane2));

}

//////////////////////////////////////////////////////////////////////////////////////////////

void EventAssembler::Close(){

   if(f_in != nullptr) f_in->Close();

}

//////////////////////////////////////////////////////////////////////////////////////////////

Event EventAssembler::GetEvent(int i){

   t_in->GetEntry(i);

   Event e;

   e.EventID = EventID;
   e.run = run;
   e.subrun = subrun;
   e.event = event;

   e.Wire_Channel = Wire_Channel;
   e.Wire_Tick = Wire_Tick;
   e.Wire_Signal = Wire_Signal;

   e.TrackIndex = TrackIndex;
   e.TrackStart_Channel = TrackStart_Channel;
   e.TrackStart_Time = TrackStart_Time;

   e.TrackStart_X = TrackStart_X;
   e.TrackStart_Y = TrackStart_Y;
   e.TrackStart_Z = TrackStart_Z;
   e.TrackDir_X = TrackDir_X;
   e.TrackDir_Y = TrackDir_Y;
   e.TrackDir_Z = TrackDir_Z;

   return e;
}

//////////////////////////////////////////////////////////////////////////////////////////////

#endif
