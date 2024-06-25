R__LOAD_LIBRARY($CONNECTEDNESS_BASE/lib/libClusterBuilder.so);

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

void HyperonSelectionTool(std::string infile,std::string intracklist,double minsize=50,double threshold=1.8,bool draw=true){

  std::string indir = "";
  std::map<std::tuple<int,int,int>,std::vector<int>> track_index_m = Connectedness::MakeSeedMap(indir + intracklist); 

  bool debug = true;
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

    std::vector<int> indexes = Connectedness::GetSeeds(e.run,e.subrun,e.event,track_index_m);
    if(debug) std::cout << "Muon=" << indexes.at(0) << "  DecayProton=" << indexes.at(1) << "  DecayPion=" << indexes.at(2) << std::endl;

    std::string rse = "run_"  + std::to_string(e.run) + "_subrun_"  + std::to_string(e.subrun) + "_event_"  + std::to_string(e.event);

    int planes_passed = 0;

    for(int i_pl=0;i_pl<Connectedness::kMAXPlane;i_pl++){

      int muon_seed_channel=-1,muon_seed_time=-1;
      int proton_seed_channel=-1,proton_seed_time=-1;
      int pion_seed_channel=-1,pion_seed_time=-1;

      for(size_t i_tr=0;i_tr<e.TrackIndex->size();i_tr++){
        if(e.TrackIndex->at(i_tr) == indexes.at(0)){
          muon_seed_channel = e.TrackStart_Channel.at(i_pl)->at(i_tr); 
          muon_seed_time = e.TrackStart_Time.at(i_pl)->at(i_tr); 
        }
        if(e.TrackIndex->at(i_tr) == indexes.at(1)){
          proton_seed_channel = e.TrackStart_Channel.at(i_pl)->at(i_tr); 
          proton_seed_time = e.TrackStart_Time.at(i_pl)->at(i_tr); 
        }
        if(e.TrackIndex->at(i_tr) == indexes.at(2)){
          pion_seed_channel = e.TrackStart_Channel.at(i_pl)->at(i_tr); 
          pion_seed_time = e.TrackStart_Time.at(i_pl)->at(i_tr); 
        }
      }

      if(muon_seed_channel == -1 || proton_seed_channel == -1 || pion_seed_channel == -1)
          throw std::invalid_argument("Track indexes for muon proton or pion not found in wire tree");

      c.ReadData(e.Wire_Channel.at(i_pl),e.Wire_Tick.at(i_pl),e.Wire_Signal.at(i_pl),rse);
      if(TestPlane(&c,rse,i_pl,muon_seed_channel,muon_seed_time,proton_seed_channel,proton_seed_time,pion_seed_channel,pion_seed_time,minsize,draw) == kPass)
        planes_passed++;
      c.Reset();
    }

    if(planes_passed > 0){
      selected_events++; 
      out << e.run << "  " << e.subrun << "  " << e.event << std::endl;
    }

    total_events++;
  }//event loop

  std::cout << "Selected " << selected_events << "/" << total_events << " events" << std::endl;
  out.close();
}

