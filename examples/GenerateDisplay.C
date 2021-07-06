
// Example root macro using clustering tool

R__LOAD_LIBRARY($CONNECTEDNESS_BASE/lib/libClusterBuilder.so)

#include <iostream>
#include <vector>

#include "ClusterBuilder.h"

#include "TFile.h"
#include "TTree.h"

   //  infile = file containing wire trees
   //  intracklist = text file with indices of tracks (muon proton pion) to use for seeding clusters

   void GenerateDisplay(std::string infile,std::string intracklist,int runtodraw,int subruntodraw,int eventtodraw){

      // Setup the cluster builder object and load dead wire maps
      // initialize with false if you don't want to draw plots
  
      std::string indir = "../data/";

      // minimum cluster size accepted
      int min_size = 50;

      // Open the input file with wire trees
      TFile *f = TFile::Open((indir + infile).c_str());

      // Open the track list file (indices of muon, proton and pion candidates)
      std::ifstream tracklist(indir + intracklist);

      bool debug=false;

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////


      // Create a file to hold the list of selected events
      std::string name = indir + infile;

      while(name.find('/') != std::string::npos){

         name = name.substr(name.find('/') + 1);

        }

      //std::ofstream out("EventLists/" + name + "_Selected.txt");

      ClusterBuilder c(true,name);
      c.LoadDeadWireMaps("../DeadWireMaps/");
      c.SetThreshold(1.8);

      if ((!f) || f->IsZombie()) { delete f; return; } // just a precaution
      TTree *t;
      f->GetObject("wire/WireTree", t);
      if (!t) { delete f; return; } // just a precaution
      t->SetMakeClass(1); // tree in MakeClass mode
      t->SetBranchStatus("*", 0); // disable all branches

      UInt_t          EventID;
      Int_t           run;
      Int_t           subrun;
      Int_t           event;
      vector<int>     *Wire_Channel_Plane0;
      vector<int>     *Wire_Tick_Plane0;
      vector<double>  *Wire_Signal_Plane0;
      vector<int>     *Wire_Channel_Plane1;
      vector<int>     *Wire_Tick_Plane1;
      vector<double>  *Wire_Signal_Plane1;
      vector<int>     *Wire_Channel_Plane2;
      vector<int>     *Wire_Tick_Plane2;
      vector<double>  *Wire_Signal_Plane2;
      vector<int>     *TrackStart_Channel_Plane0;
      vector<int>     *TrackStart_Time_Plane0;
      vector<int>     *TrackStart_Channel_Plane1;
      vector<int>     *TrackStart_Time_Plane1;
      vector<int>     *TrackStart_Channel_Plane2;
      vector<int>     *TrackStart_Time_Plane2;
      vector<double>  *TrackStart_X;
      vector<double>  *TrackStart_Y;
      vector<double>  *TrackStart_Z;
      vector<double>  *TrackDir_X;
      vector<double>  *TrackDir_Y;
      vector<double>  *TrackDir_Z;
      vector<int>     *CaloStart_Channel_Plane0;
      vector<int>     *CaloStart_Time_Plane0;
      vector<double>  *CaloStart_X_Plane0;
      vector<double>  *CaloStart_Y_Plane0;
      vector<double>  *CaloStart_Z_Plane0;
      vector<int>     *CaloStart_Channel_Plane1;
      vector<int>     *CaloStart_Time_Plane1;
      vector<double>  *CaloStart_X_Plane1;
      vector<double>  *CaloStart_Y_Plane1;
      vector<double>  *CaloStart_Z_Plane1;
      vector<int>     *CaloStart_Channel_Plane2;
      vector<int>     *CaloStart_Time_Plane2;
      vector<double>  *CaloStart_X_Plane2;
      vector<double>  *CaloStart_Y_Plane2;
      vector<double>  *CaloStart_Z_Plane2;

      // Set object pointer
      Wire_Channel_Plane0 = 0;
      Wire_Tick_Plane0 = 0;
      Wire_Signal_Plane0 = 0;
      Wire_Channel_Plane1 = 0;
      Wire_Tick_Plane1 = 0;
      Wire_Signal_Plane1 = 0;
      Wire_Channel_Plane2 = 0;
      Wire_Tick_Plane2 = 0;
      Wire_Signal_Plane2 = 0;
      TrackStart_Channel_Plane0 = 0;
      TrackStart_Time_Plane0 = 0;
      TrackStart_Channel_Plane1 = 0;
      TrackStart_Time_Plane1 = 0;
      TrackStart_Channel_Plane2 = 0;
      TrackStart_Time_Plane2 = 0;
      TrackStart_X = 0;
      TrackStart_Y = 0;
      TrackStart_Z = 0;
      TrackDir_X = 0;
      TrackDir_Y = 0;
      TrackDir_Z = 0;
      CaloStart_Channel_Plane0 = 0;
      CaloStart_Time_Plane0 = 0;
      CaloStart_X_Plane0 = 0;
      CaloStart_Y_Plane0 = 0;
      CaloStart_Z_Plane0 = 0;
      CaloStart_Channel_Plane1 = 0;
      CaloStart_Time_Plane1 = 0;
      CaloStart_X_Plane1 = 0;
      CaloStart_Y_Plane1 = 0;
      CaloStart_Z_Plane1 = 0;
      CaloStart_Channel_Plane2 = 0;
      CaloStart_Time_Plane2 = 0;
      CaloStart_X_Plane2 = 0;
      CaloStart_Y_Plane2 = 0;
      CaloStart_Z_Plane2 = 0;

      t->SetBranchStatus("EventID",1);
      t->SetBranchStatus("run",1);
      t->SetBranchStatus("subrun",1);
      t->SetBranchStatus("event",1);
      t->SetBranchStatus("Wire_Channel_Plane0",1);
      t->SetBranchStatus("Wire_Tick_Plane0",1);
      t->SetBranchStatus("Wire_Signal_Plane0",1);
      t->SetBranchStatus("Wire_Channel_Plane1",1);
      t->SetBranchStatus("Wire_Tick_Plane1",1);
      t->SetBranchStatus("Wire_Signal_Plane1",1);
      t->SetBranchStatus("Wire_Channel_Plane2",1);
      t->SetBranchStatus("Wire_Tick_Plane2",1);
      t->SetBranchStatus("Wire_Signal_Plane2",1);
      t->SetBranchStatus("TrackStart_Channel_Plane0",1);
      t->SetBranchStatus("TrackStart_Time_Plane0",1);
      t->SetBranchStatus("TrackStart_Channel_Plane1",1);
      t->SetBranchStatus("TrackStart_Time_Plane1",1);
      t->SetBranchStatus("TrackStart_Channel_Plane2",1);
      t->SetBranchStatus("TrackStart_Time_Plane2",1);
      t->SetBranchStatus("TrackStart_X",1);
      t->SetBranchStatus("TrackStart_Y",1);
      t->SetBranchStatus("TrackStart_Z",1);
      t->SetBranchStatus("TrackDir_X",1);
      t->SetBranchStatus("TrackDir_Y",1);
      t->SetBranchStatus("TrackDir_Z",1);
      t->SetBranchStatus("CaloStart_Channel_Plane0",1);
      t->SetBranchStatus("CaloStart_Time_Plane0",1);
      t->SetBranchStatus("CaloStart_X_Plane0",1);
      t->SetBranchStatus("CaloStart_Y_Plane0",1);
      t->SetBranchStatus("CaloStart_Z_Plane0",1);
      t->SetBranchStatus("CaloStart_Channel_Plane1",1);
      t->SetBranchStatus("CaloStart_Time_Plane1",1);
      t->SetBranchStatus("CaloStart_X_Plane1",1);
      t->SetBranchStatus("CaloStart_Y_Plane1",1);
      t->SetBranchStatus("CaloStart_Z_Plane1",1);
      t->SetBranchStatus("CaloStart_Channel_Plane2",1);
      t->SetBranchStatus("CaloStart_Time_Plane2",1);
      t->SetBranchStatus("CaloStart_X_Plane2",1);
      t->SetBranchStatus("CaloStart_Y_Plane2",1);
      t->SetBranchStatus("CaloStart_Z_Plane2",1);

      t->SetBranchAddress("EventID", &EventID);
      t->SetBranchAddress("run", &run);
      t->SetBranchAddress("subrun", &subrun);
      t->SetBranchAddress("event", &event);
      t->SetBranchAddress("Wire_Channel_Plane0", &Wire_Channel_Plane0);
      t->SetBranchAddress("Wire_Tick_Plane0", &Wire_Tick_Plane0);
      t->SetBranchAddress("Wire_Signal_Plane0", &Wire_Signal_Plane0);
      t->SetBranchAddress("Wire_Channel_Plane1", &Wire_Channel_Plane1);
      t->SetBranchAddress("Wire_Tick_Plane1", &Wire_Tick_Plane1);
      t->SetBranchAddress("Wire_Signal_Plane1", &Wire_Signal_Plane1);
      t->SetBranchAddress("Wire_Channel_Plane2", &Wire_Channel_Plane2);
      t->SetBranchAddress("Wire_Tick_Plane2", &Wire_Tick_Plane2);
      t->SetBranchAddress("Wire_Signal_Plane2", &Wire_Signal_Plane2);
      t->SetBranchAddress("TrackStart_Channel_Plane0", &TrackStart_Channel_Plane0);
      t->SetBranchAddress("TrackStart_Time_Plane0", &TrackStart_Time_Plane0);
      t->SetBranchAddress("TrackStart_Channel_Plane1", &TrackStart_Channel_Plane1);
      t->SetBranchAddress("TrackStart_Time_Plane1", &TrackStart_Time_Plane1);
      t->SetBranchAddress("TrackStart_Channel_Plane2", &TrackStart_Channel_Plane2);
      t->SetBranchAddress("TrackStart_Time_Plane2", &TrackStart_Time_Plane2);
      t->SetBranchAddress("TrackStart_X", &TrackStart_X);
      t->SetBranchAddress("TrackStart_Y", &TrackStart_Y);
      t->SetBranchAddress("TrackStart_Z", &TrackStart_Z);
      t->SetBranchAddress("TrackDir_X", &TrackDir_X);
      t->SetBranchAddress("TrackDir_Y", &TrackDir_Y);
      t->SetBranchAddress("TrackDir_Z", &TrackDir_Z);
      t->SetBranchAddress("CaloStart_Channel_Plane0", &CaloStart_Channel_Plane0);
      t->SetBranchAddress("CaloStart_Time_Plane0", &CaloStart_Time_Plane0);
      t->SetBranchAddress("CaloStart_X_Plane0", &CaloStart_X_Plane0);
      t->SetBranchAddress("CaloStart_Y_Plane0", &CaloStart_Y_Plane0);
      t->SetBranchAddress("CaloStart_Z_Plane0", &CaloStart_Z_Plane0);
      t->SetBranchAddress("CaloStart_Channel_Plane1", &CaloStart_Channel_Plane1);
      t->SetBranchAddress("CaloStart_Time_Plane1", &CaloStart_Time_Plane1);
      t->SetBranchAddress("CaloStart_X_Plane1", &CaloStart_X_Plane1);
      t->SetBranchAddress("CaloStart_Y_Plane1", &CaloStart_Y_Plane1);
      t->SetBranchAddress("CaloStart_Z_Plane1", &CaloStart_Z_Plane1);
      t->SetBranchAddress("CaloStart_Channel_Plane2", &CaloStart_Channel_Plane2);
      t->SetBranchAddress("CaloStart_Time_Plane2", &CaloStart_Time_Plane2);
      t->SetBranchAddress("CaloStart_X_Plane2", &CaloStart_X_Plane2);
      t->SetBranchAddress("CaloStart_Y_Plane2", &CaloStart_Y_Plane2);
      t->SetBranchAddress("CaloStart_Z_Plane2", &CaloStart_Z_Plane2);


      //////////////////////////////////////////////////////////////////////////////////////////////////////////////

      int selected_events=0;
      int total_events=0;

      // Event loop
      Long64_t nentries = t->GetEntriesFast();
      for(size_t j=0;j<nentries;j++){


         if(debug) std::cout << "Processing event " << j << std::endl;

         t->GetEntry(j);
        
         // Get the indexes of the muon, decay proton/pion candidates in the track list
         int muon_index,proton_index,pion_index;
         tracklist >> muon_index >> proton_index >> pion_index;

         if(run != runtodraw || subrun != subruntodraw || event != eventtodraw) continue;        

         if(debug) std::cout << "Muon=" << muon_index << "  DecayProton=" << proton_index << "  DecayPion=" << pion_index << std::endl;
         

         // Number of planes passing selection criteria
         int Planes_Passed = 0;
         std::pair<int,int> thisClusterIDSize;

         // Check Plane0 //
         std::string rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event) + "_Plane0";

         // Plane quality checks
         bool Plane0_Good=true;

         // Check the seeds are not separated by dead channels
         if(c.SeedDeadWireCheck(*TrackStart_Channel_Plane0,*TrackStart_Time_Plane0,0)) Plane0_Good=false;

         // Read in the wire info for this plane
         c.ReadData(*Wire_Channel_Plane0,*Wire_Tick_Plane0,*Wire_Signal_Plane0,rse);

         std::vector<int> ClusterIDs_Plane0;
         std::vector<int> ClusterSizes_Plane0;

         if(debug) std::cout << "Generating Clusters in Plane0" << std::endl;

         //std::cout << "Tracks in Plane0 = " << TrackStart_Channel_Plane0->size() << std::endl;

         // try generatig a cluster from the muon track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane0->at(muon_index),TrackStart_Time_Plane0->at(muon_index),1);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane0.push_back(thisClusterIDSize.first); ClusterSizes_Plane0.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the proton track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane0->at(proton_index),TrackStart_Time_Plane0->at(proton_index),2);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane0.push_back(thisClusterIDSize.first); ClusterSizes_Plane0.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the pion track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane0->at(pion_index),TrackStart_Time_Plane0->at(pion_index),3);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane0.push_back(thisClusterIDSize.first); ClusterSizes_Plane0.push_back(thisClusterIDSize.second); }


         // Selection criteria: ID vector should read 1 2 2 (muon and proton/pion merged) 
         // Clusters should all be of at least size min_size
         if(ClusterIDs_Plane0.size() == 3 && ClusterIDs_Plane0.at(0) == 1 && ClusterIDs_Plane0.at(1) == 2 && ClusterIDs_Plane0.at(2) == 2 && ClusterSizes_Plane0.at(0) > min_size && ClusterSizes_Plane0.at(1) > min_size && ClusterSizes_Plane0.at(2) > min_size){

 Planes_Passed++; 
 c.DrawClustered(rse,0); 
 }

         c.Reset();

         // Check Plane1 //
         rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event) + "_Plane1";

         // Plane quality checks
         bool Plane1_Good=true;

         // Check the seeds are not separated by dead channels
         if(c.SeedDeadWireCheck(*TrackStart_Channel_Plane1,*TrackStart_Time_Plane1,0)) Plane1_Good=false;

         // Read in the wire info for this plane
         c.ReadData(*Wire_Channel_Plane1,*Wire_Tick_Plane1,*Wire_Signal_Plane1,rse);

         std::vector<int> ClusterIDs_Plane1;
         std::vector<int> ClusterSizes_Plane1;

         if(debug) std::cout << "Generating Clusters in Plane1" << std::endl;

         // try generatig a cluster from the muon track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane1->at(muon_index),TrackStart_Time_Plane1->at(muon_index),1);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane1.push_back(thisClusterIDSize.first); ClusterSizes_Plane1.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the proton track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane1->at(proton_index),TrackStart_Time_Plane1->at(proton_index),2);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane1.push_back(thisClusterIDSize.first); ClusterSizes_Plane1.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the pion track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane1->at(pion_index),TrackStart_Time_Plane1->at(pion_index),3);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane1.push_back(thisClusterIDSize.first); ClusterSizes_Plane1.push_back(thisClusterIDSize.second); }


         // Selection criteria: ID vector should read 1 2 2 (muon and proton/pion merged) 
         // Clusters should all be of at least size min_size
         if(ClusterIDs_Plane1.size() == 3 && ClusterIDs_Plane1.at(0) == 1 && ClusterIDs_Plane1.at(1) == 2 && ClusterIDs_Plane1.at(2) == 2 && ClusterSizes_Plane1.at(0) > min_size && ClusterSizes_Plane1.at(1) > min_size && ClusterSizes_Plane1.at(2) > min_size){

 Planes_Passed++; 
 c.DrawClustered(rse,1); 

}

         c.Reset();

         // Check Plane2 //

         rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event) + "_Plane2";

         // Plane quality checks
         bool Plane2_Good=true;

         // Check the seeds are not separated by dead channels
         if(c.SeedDeadWireCheck(*TrackStart_Channel_Plane2,*TrackStart_Time_Plane2,0)) Plane2_Good=false;

         // Read in the wire info for this plane
         c.ReadData(*Wire_Channel_Plane2,*Wire_Tick_Plane2,*Wire_Signal_Plane2,rse);

         std::vector<int> ClusterIDs_Plane2;
         std::vector<int> ClusterSizes_Plane2;

         if(debug) std::cout << "Generating Clusters in Plane2" << std::endl;

         // try generatig a cluster from the muon track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane2->at(muon_index),TrackStart_Time_Plane2->at(muon_index),1);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane2.push_back(thisClusterIDSize.first); ClusterSizes_Plane2.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the proton track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane2->at(proton_index),TrackStart_Time_Plane2->at(proton_index),2);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane2.push_back(thisClusterIDSize.first); ClusterSizes_Plane2.push_back(thisClusterIDSize.second); }
         // try generatig a cluster from the pion track start
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane2->at(pion_index),TrackStart_Time_Plane2->at(pion_index),3);
         if(thisClusterIDSize.first > 0){ ClusterIDs_Plane2.push_back(thisClusterIDSize.first); ClusterSizes_Plane2.push_back(thisClusterIDSize.second); }


         // Selection criteria: ID vector should read 1 2 2 (muon and proton/pion merged) 
         // Clusters should all be of at least size min_size
         if(ClusterIDs_Plane2.size() == 3 && ClusterIDs_Plane2.at(0) == 1 && ClusterIDs_Plane2.at(1) == 2 && ClusterIDs_Plane2.at(2) == 2 && ClusterSizes_Plane2.at(0) > min_size && ClusterSizes_Plane2.at(1) > min_size && ClusterSizes_Plane2.at(2) > min_size){

 Planes_Passed++; 
c.DrawClustered(rse,2); 

}

         c.Reset();

         if(Planes_Passed > 0){

            selected_events++; 
         }

         total_events++;

      }//event loop

      std::cout << "Selected " << selected_events << "/" << total_events << " events" << std::endl;

   }

