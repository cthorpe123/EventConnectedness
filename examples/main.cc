
// Example application using clustering tool

// Compile me with:
// g++ `${ROOTSYS}/bin/root-config --cflags` `${ROOTSYS}/bin/root-config --libs` -L$CONNECTEDNESS_BASE/lib -lClusterBuilder -I$CONNECTEDNESS_BASE/Alg main.cc -o main

#include <iostream>
#include <vector>

#include "ClusterBuilder.h"

#include "TFile.h"
#include "TTree.h"

int main(){

   // Setup the cluster builder object and load dead wire maps
   // initialize with false if you don't want to draw plots (faster)
   ClusterBuilder c(true);
   c.LoadDeadWireMaps("../DeadWireMaps/");
   c.SetThreshold(1.8);

   // Open the input file with wire trees
   TFile *f = TFile::Open("../data/WireTreesFHC_Overlay_NuWro_Hyperon_prod_numi_uboone_overlay_fhc_mcc9_run1_v51_nuwro_hyperon_nuwro_reco2_reco2_filtered.root");

   if ((!f) || f->IsZombie()) { delete f; return 1; } // just a precaution
   TTree *t;
   f->GetObject("wire/WireTree", t);
   if (!t) { delete f; return 1; } // just a precaution
   t->SetMakeClass(1); // tree in MakeClass mode
   t->SetBranchStatus("*", 0); // disable all branches

   UInt_t          EventID;
   Int_t           run;
   Int_t           subrun;
   Int_t           event;

   std::vector<int>     *Wire_Channel_Plane0=0 ;
   std::vector<int>     *Wire_Tick_Plane0=0;
   std::vector<double>  *Wire_Signal_Plane0=0;
   std::vector<int>     *TrackStart_Channel_Plane0=0;
   std::vector<int>     *TrackStart_Time_Plane0=0;

   t->SetBranchStatus("EventID",1);
   t->SetBranchStatus("run",1);
   t->SetBranchStatus("subrun",1);
   t->SetBranchStatus("event",1);
   t->SetBranchStatus("Wire_Channel_Plane0",1);
   t->SetBranchStatus("Wire_Tick_Plane0",1);
   t->SetBranchStatus("Wire_Signal_Plane0",1);
   t->SetBranchStatus("TrackStart_Channel_Plane0",1);
   t->SetBranchStatus("TrackStart_Time_Plane0",1);

   t->SetBranchAddress("EventID", &EventID);
   t->SetBranchAddress("run", &run);
   t->SetBranchAddress("subrun", &subrun);
   t->SetBranchAddress("event", &event);
   t->SetBranchAddress("Wire_Channel_Plane0", &Wire_Channel_Plane0);
   t->SetBranchAddress("Wire_Tick_Plane0", &Wire_Tick_Plane0);
   t->SetBranchAddress("Wire_Signal_Plane0", &Wire_Signal_Plane0);
   t->SetBranchAddress("TrackStart_Channel_Plane0", &TrackStart_Channel_Plane0);
   t->SetBranchAddress("TrackStart_Time_Plane0", &TrackStart_Time_Plane0);


   // Event loop
   Long64_t nentries = t->GetEntriesFast();
   for(size_t j=0;j<nentries && j<10;j++){

      t->GetEntry(j);

      std::string rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event);

      // Plane quality checks

      // Check the seeds are not separated by dead channels
      c.SeedDeadWireCheck(*TrackStart_Channel_Plane0,*TrackStart_Time_Plane0,0);

      // Read in the wire info for this plane
      c.ReadData(*Wire_Channel_Plane0,*Wire_Tick_Plane0,*Wire_Signal_Plane0,rse);

         std::vector<int> ClusterIDs;
         std::vector<int> ClusterSizes;

        
      // Try making some clusters using track starts as seeds
      for(size_t i_tr=0;i_tr<TrackStart_Channel_Plane0->size();i_tr++){

        std::pair<int,int> thisClusterIDSize;

         // Cluster building function. Returns (ID,size)
         // Returns -1,-1 if the seed was in an empty bin
         // If the seed bin already belonged to another cluster, returns the ID of that cluster
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel_Plane0->at(i_tr),TrackStart_Time_Plane0->at(i_tr),i_tr+1);
         if(thisClusterIDSize.first > 0){ ClusterIDs.push_back(thisClusterIDSize.first); ClusterSizes.push_back(thisClusterIDSize.second); }
      }

      // Print list of cluster IDs, interpret this for selection
      if(ClusterIDs.size()){

         std::cout << "List of cluster IDs and sizes" << std::endl;

         for(size_t i_c=0;i_c<ClusterIDs.size();i_c++) std::cout << "ID=" << ClusterIDs.at(i_c) << "  Size=" << ClusterSizes.at(i_c) << "  ";  

         std::cout << std::endl;

      }
        

      c.DrawClustered(rse,0);
 
      c.Reset();

   }//event loop

   return 0;
}


