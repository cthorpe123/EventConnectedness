R__LOAD_LIBRARY($CONNECTEDNESS_BASE/lib/libClusterBuilder.so)

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "ClusterBuilder.h"
#include "EventAssembler.h"
#include "TestHelper.h"

// Example root macro using clustering tool

//  infile = file containing wire trees
//  intracklist = text file with indices of tracks (muon proton pion) to use for seeding clusters

void HyperonSelectionTool(std::string infile,std::string intracklist,double minsize=50,double threshold=1.8,bool draw=false){

   std::string indir = "../data/";

   // Open the input file with wire trees and track list
   //TFile *f = TFile::Open((indir + infile).c_str());
   std::ifstream tracklist(indir + intracklist);

   bool debug = false;
   std::string plotdir = "Displays/";

   //////////////////////////////////////////////////////////////////////////////////////////////////////////////

   // Create a file to hold the list of selected events
   gSystem->Exec("mkdir -p EventLists/");
   std::string name = indir + infile;
   while(name.find('/') != std::string::npos) name = name.substr(name.find('/') + 1);
   std::ofstream out("EventLists/" + name + "_Selected.txt");

   // Setup cluster builder
   Connectedness::ClusterBuilder c(draw,plotdir);  
   c.LoadDeadWireMaps("../DeadWireMaps/");
   c.SetThreshold(threshold);
   //c.SetGrowthArea(3);

   Connectedness::EventAssembler E;
   E.SetFile(indir + infile);

   int selected_events=0;
   int total_events=0;

   Long64_t nentries = E.GetNEvents();

   // Event loop
   for(size_t j=0;j<nentries;j++){

      Connectedness::Event e = E.GetEvent(j);

      if(j % 10 == 0) std::cout << "Processing event " << j << std::endl;

      // Get the indexes of the muon, decay proton/pion candidates in the track list
      int muon_index,proton_index,pion_index;
      tracklist >> muon_index >> proton_index >> pion_index;

      if(debug) std::cout << "Muon=" << muon_index << "  DecayProton=" << proton_index << "  DecayPion=" << pion_index << std::endl;

      std::string rse = "run_"  + std::to_string(e.run) + "_subrun_"  + std::to_string(e.subrun) + "_event_"  + std::to_string(e.event);

      int planes_passed = 0;
     
      // Test Plane0
      int muon_seed_channel = e.TrackStart_Channel_Plane0.at(muon_index), muon_seed_time = e.TrackStart_Time_Plane0.at(muon_index); 
      int proton_seed_channel = e.TrackStart_Channel_Plane0.at(proton_index), proton_seed_time = e.TrackStart_Time_Plane0.at(proton_index); 
      int pion_seed_channel = e.TrackStart_Channel_Plane0.at(pion_index), pion_seed_time = e.TrackStart_Time_Plane0.at(pion_index); 
      c.ReadData(e.Wire_Channel_Plane0,e.Wire_Tick_Plane0,e.Wire_Signal_Plane0,rse);
      if(TestPlane(&c,rse,Connectedness::kPlane0,muon_seed_channel,muon_seed_time,proton_seed_channel,proton_seed_time,pion_seed_channel,pion_seed_time,minsize,draw) == kPass)
         planes_passed++;

      // Test Plane1
      muon_seed_channel = e.TrackStart_Channel_Plane1.at(muon_index), muon_seed_time = e.TrackStart_Time_Plane1.at(muon_index); 
      proton_seed_channel = e.TrackStart_Channel_Plane1.at(proton_index), proton_seed_time = e.TrackStart_Time_Plane1.at(proton_index); 
      pion_seed_channel = e.TrackStart_Channel_Plane1.at(pion_index), pion_seed_time = e.TrackStart_Time_Plane1.at(pion_index); 
      c.ReadData(e.Wire_Channel_Plane1,e.Wire_Tick_Plane1,e.Wire_Signal_Plane1,rse);
      if(TestPlane(&c,rse,Connectedness::kPlane1,muon_seed_channel,muon_seed_time,proton_seed_channel,proton_seed_time,pion_seed_channel,pion_seed_time,minsize,draw) == kPass)
         planes_passed++;

      // Test Plane2
      muon_seed_channel = e.TrackStart_Channel_Plane2.at(muon_index), muon_seed_time = e.TrackStart_Time_Plane2.at(muon_index); 
      proton_seed_channel = e.TrackStart_Channel_Plane2.at(proton_index), proton_seed_time = e.TrackStart_Time_Plane2.at(proton_index); 
      pion_seed_channel = e.TrackStart_Channel_Plane2.at(pion_index), pion_seed_time = e.TrackStart_Time_Plane2.at(pion_index); 
      c.ReadData(e.Wire_Channel_Plane2,e.Wire_Tick_Plane2,e.Wire_Signal_Plane2,rse);
      if(TestPlane(&c,rse,Connectedness::kPlane2,muon_seed_channel,muon_seed_time,proton_seed_channel,proton_seed_time,pion_seed_channel,pion_seed_time,minsize,draw) == kPass)
         planes_passed++;
      
      if(planes_passed > 0){
         selected_events++; 
         out << e.run << "  " << e.subrun << "  " << e.event << std::endl;
      }

      total_events++;
   }//event loop

   std::cout << "Selected " << selected_events << "/" << total_events << " events" << std::endl;
   out.close();
}

