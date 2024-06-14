R__LOAD_LIBRARY($CONNECTEDNESS_BASE/lib/libClusterBuilder.so);

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "ClusterBuilder.h"
#include "EventAssembler.h"
#include "Shared.h"
#include "TestHelper.h"

// Example root macro using clustering tool

//  infile = file containing wire trees
//  intracklist = text file with indices of tracks (muon proton pion) to use for seeding clusters

void GenerateDisplay(std::string infile,std::string intracklist,int runtodraw,int subruntodraw,int eventtodraw,double minsize=50,double threshold=1.8){

  std::string indir = "";

  std::map<std::tuple<int,int,int>,std::vector<int>> track_index_m = Connectedness::MakeSeedMap(indir + intracklist); 

  bool debug = true;
  std::string plotdir = "Displays/";

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Setup cluster builder
  Connectedness::ClusterBuilder c(true,plotdir);  
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

    if(e.run != runtodraw || e.subrun != subruntodraw || e.event != eventtodraw) continue;      

    std::vector<int> indexes = Connectedness::GetSeeds(runtodraw,subruntodraw,eventtodraw,track_index_m);

    if(debug) std::cout << "Muon=" << indexes.at(0) << "  DecayProton=" << indexes.at(1) << "  DecayPion=" << indexes.at(2) << std::endl;

    std::string rse = "run_"  + std::to_string(e.run) + "_subrun_"  + std::to_string(e.subrun) + "_event_"  + std::to_string(e.event);
    
    for(int i_pl=0;i_pl<Connectedness::kMAXPlane;i_pl++){

      int muon_seed_channel = e.TrackStart_Channel.at(i_pl)->at(indexes.at(0)), muon_seed_time = e.TrackStart_Time.at(i_pl)->at(indexes.at(0)); 
      int proton_seed_channel = e.TrackStart_Channel.at(i_pl)->at(indexes.at(1)), proton_seed_time = e.TrackStart_Time.at(i_pl)->at(indexes.at(1)); 
      int pion_seed_channel = e.TrackStart_Channel.at(i_pl)->at(indexes.at(2)), pion_seed_time = e.TrackStart_Time.at(i_pl)->at(indexes.at(2)); 

      c.ReadData(e.Wire_Channel.at(i_pl),e.Wire_Tick.at(i_pl),e.Wire_Signal.at(i_pl),rse);
      TestPlane(&c,rse,i_pl,muon_seed_channel,muon_seed_time,proton_seed_channel,proton_seed_time,pion_seed_channel,pion_seed_time,minsize,true);
      c.Reset();

    }

  }//event loop

}



