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
   t_in->SetBranchStatus("Wire_Channel_Plane0",1);
   t_in->SetBranchStatus("Wire_Tick_Plane0",1);
   t_in->SetBranchStatus("Wire_Signal_Plane0",1);
   t_in->SetBranchStatus("Wire_Channel_Plane1",1);
   t_in->SetBranchStatus("Wire_Tick_Plane1",1);
   t_in->SetBranchStatus("Wire_Signal_Plane1",1);
   t_in->SetBranchStatus("Wire_Channel_Plane2",1);
   t_in->SetBranchStatus("Wire_Tick_Plane2",1);
   t_in->SetBranchStatus("Wire_Signal_Plane2",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane0",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane0",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane1",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane1",1);
   t_in->SetBranchStatus("TrackStart_Channel_Plane2",1);
   t_in->SetBranchStatus("TrackStart_Time_Plane2",1);
   t_in->SetBranchStatus("TrackStart_X",1);
   t_in->SetBranchStatus("TrackStart_Y",1);
   t_in->SetBranchStatus("TrackStart_Z",1);
   t_in->SetBranchStatus("TrackDir_X",1);
   t_in->SetBranchStatus("TrackDir_Y",1);
   t_in->SetBranchStatus("TrackDir_Z",1);
   t_in->SetBranchStatus("CaloStart_Channel_Plane0",1);
   t_in->SetBranchStatus("CaloStart_Time_Plane0",1);
   t_in->SetBranchStatus("CaloStart_X_Plane0",1);
   t_in->SetBranchStatus("CaloStart_Y_Plane0",1);
   t_in->SetBranchStatus("CaloStart_Z_Plane0",1);
   t_in->SetBranchStatus("CaloStart_Channel_Plane1",1);
   t_in->SetBranchStatus("CaloStart_Time_Plane1",1);
   t_in->SetBranchStatus("CaloStart_X_Plane1",1);
   t_in->SetBranchStatus("CaloStart_Y_Plane1",1);
   t_in->SetBranchStatus("CaloStart_Z_Plane1",1);
   t_in->SetBranchStatus("CaloStart_Channel_Plane2",1);
   t_in->SetBranchStatus("CaloStart_Time_Plane2",1);
   t_in->SetBranchStatus("CaloStart_X_Plane2",1);
   t_in->SetBranchStatus("CaloStart_Y_Plane2",1);
   t_in->SetBranchStatus("CaloStart_Z_Plane2",1);

   t_in->SetBranchAddress("EventID", &EventID);
   t_in->SetBranchAddress("run", &run);
   t_in->SetBranchAddress("subrun", &subrun);
   t_in->SetBranchAddress("event", &event);
   t_in->SetBranchAddress("Wire_Channel_Plane0", &Wire_Channel_Plane0);
   t_in->SetBranchAddress("Wire_Tick_Plane0", &Wire_Tick_Plane0);
   t_in->SetBranchAddress("Wire_Signal_Plane0", &Wire_Signal_Plane0);
   t_in->SetBranchAddress("Wire_Channel_Plane1", &Wire_Channel_Plane1);
   t_in->SetBranchAddress("Wire_Tick_Plane1", &Wire_Tick_Plane1);
   t_in->SetBranchAddress("Wire_Signal_Plane1", &Wire_Signal_Plane1);
   t_in->SetBranchAddress("Wire_Channel_Plane2", &Wire_Channel_Plane2);
   t_in->SetBranchAddress("Wire_Tick_Plane2", &Wire_Tick_Plane2);
   t_in->SetBranchAddress("Wire_Signal_Plane2", &Wire_Signal_Plane2);
   t_in->SetBranchAddress("TrackStart_Channel_Plane0", &TrackStart_Channel_Plane0);
   t_in->SetBranchAddress("TrackStart_Time_Plane0", &TrackStart_Time_Plane0);
   t_in->SetBranchAddress("TrackStart_Channel_Plane1", &TrackStart_Channel_Plane1);
   t_in->SetBranchAddress("TrackStart_Time_Plane1", &TrackStart_Time_Plane1);
   t_in->SetBranchAddress("TrackStart_Channel_Plane2", &TrackStart_Channel_Plane2);
   t_in->SetBranchAddress("TrackStart_Time_Plane2", &TrackStart_Time_Plane2);
   t_in->SetBranchAddress("TrackStart_X", &TrackStart_X);
   t_in->SetBranchAddress("TrackStart_Y", &TrackStart_Y);
   t_in->SetBranchAddress("TrackStart_Z", &TrackStart_Z);
   t_in->SetBranchAddress("TrackDir_X", &TrackDir_X);
   t_in->SetBranchAddress("TrackDir_Y", &TrackDir_Y);
   t_in->SetBranchAddress("TrackDir_Z", &TrackDir_Z);
   t_in->SetBranchAddress("CaloStart_Channel_Plane0", &CaloStart_Channel_Plane0);
   t_in->SetBranchAddress("CaloStart_Time_Plane0", &CaloStart_Time_Plane0);
   t_in->SetBranchAddress("CaloStart_X_Plane0", &CaloStart_X_Plane0);
   t_in->SetBranchAddress("CaloStart_Y_Plane0", &CaloStart_Y_Plane0);
   t_in->SetBranchAddress("CaloStart_Z_Plane0", &CaloStart_Z_Plane0);
   t_in->SetBranchAddress("CaloStart_Channel_Plane1", &CaloStart_Channel_Plane1);
   t_in->SetBranchAddress("CaloStart_Time_Plane1", &CaloStart_Time_Plane1);
   t_in->SetBranchAddress("CaloStart_X_Plane1", &CaloStart_X_Plane1);
   t_in->SetBranchAddress("CaloStart_Y_Plane1", &CaloStart_Y_Plane1);
   t_in->SetBranchAddress("CaloStart_Z_Plane1", &CaloStart_Z_Plane1);
   t_in->SetBranchAddress("CaloStart_Channel_Plane2", &CaloStart_Channel_Plane2);
   t_in->SetBranchAddress("CaloStart_Time_Plane2", &CaloStart_Time_Plane2);
   t_in->SetBranchAddress("CaloStart_X_Plane2", &CaloStart_X_Plane2);
   t_in->SetBranchAddress("CaloStart_Y_Plane2", &CaloStart_Y_Plane2);
   t_in->SetBranchAddress("CaloStart_Z_Plane2", &CaloStart_Z_Plane2);
   
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

   e.Wire_Channel_Plane0 = *Wire_Channel_Plane0;
   e.Wire_Tick_Plane0 = *Wire_Tick_Plane0;
   e.Wire_Signal_Plane0 = *Wire_Signal_Plane0;
   e.TrackStart_Channel_Plane0 = *TrackStart_Channel_Plane0;
   e.TrackStart_Time_Plane0 = *TrackStart_Time_Plane0;
   e.CaloStart_Channel_Plane0 = *CaloStart_Channel_Plane0;
   e.CaloStart_Time_Plane0 = *CaloStart_Time_Plane0;
   e.CaloStart_X_Plane0 = *CaloStart_X_Plane0;
   e.CaloStart_Y_Plane0 = *CaloStart_Y_Plane0;
   e.CaloStart_Z_Plane0 = *CaloStart_Z_Plane0;
   
   e.Wire_Channel_Plane1 = *Wire_Channel_Plane1;
   e.Wire_Tick_Plane1 = *Wire_Tick_Plane1;
   e.Wire_Signal_Plane1 = *Wire_Signal_Plane1;
   e.TrackStart_Channel_Plane1 = *TrackStart_Channel_Plane1;
   e.TrackStart_Time_Plane1 = *TrackStart_Time_Plane1;
   e.CaloStart_Channel_Plane1 = *CaloStart_Channel_Plane1;
   e.CaloStart_Time_Plane1 = *CaloStart_Time_Plane1;
   e.CaloStart_X_Plane1 = *CaloStart_X_Plane1;
   e.CaloStart_Y_Plane1 = *CaloStart_Y_Plane1;
   e.CaloStart_Z_Plane1 = *CaloStart_Z_Plane1;

   e.Wire_Channel_Plane2 = *Wire_Channel_Plane2;
   e.Wire_Tick_Plane2 = *Wire_Tick_Plane2;
   e.Wire_Signal_Plane2 = *Wire_Signal_Plane2;
   e.TrackStart_Channel_Plane2 = *TrackStart_Channel_Plane2;
   e.TrackStart_Time_Plane2 = *TrackStart_Time_Plane2;
   e.CaloStart_Channel_Plane2 = *CaloStart_Channel_Plane2;
   e.CaloStart_Time_Plane2 = *CaloStart_Time_Plane2;
   e.CaloStart_X_Plane2 = *CaloStart_X_Plane2;
   e.CaloStart_Y_Plane2 = *CaloStart_Y_Plane2;
   e.CaloStart_Z_Plane2 = *CaloStart_Z_Plane2;

   e.TrackStart_X = *TrackStart_X;
   e.TrackStart_Y = *TrackStart_Y;
   e.TrackStart_Z = *TrackStart_Z;
   e.TrackDir_X = *TrackDir_X;
   e.TrackDir_Y = *TrackDir_Y;
   e.TrackDir_Z = *TrackDir_Z;

   return e;
}

//////////////////////////////////////////////////////////////////////////////////////////////

#endif
