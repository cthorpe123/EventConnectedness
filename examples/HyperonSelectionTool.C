R__LOAD_LIBRARY($CONNECTEDNESS_BASE/lib/libClusterBuilder.so)

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "ClusterBuilder.h"

   const double Threshold = 1.8;
   const double MinSize = 50;
   const bool Debug = false;
   const bool Draw = true;
   const std::string PlotDir = "Displays/";

   Connectedness::ClusterBuilder c(Draw,PlotDir);  

   bool TestPlane(std::string rse,int plane,std::vector<int> TrackStart_Channel,std::vector<int> TrackStart_Time, int MuonIndex,int ProtonIndex,int PionIndex,std::vector<int> Wire_Channel,std::vector<int> Wire_Tick,std::vector<double> Wire_Signal){

      bool pass = false;

      // Read in the wire info for this plane
      c.ReadData(Wire_Channel,Wire_Tick,Wire_Signal,rse);

      std::vector<int> TrackStart_Channel_MPP = {TrackStart_Channel.at(MuonIndex),TrackStart_Channel.at(ProtonIndex),TrackStart_Channel.at(PionIndex)};
      std::vector<int> TrackStart_Time_MPP = {TrackStart_Time.at(MuonIndex),TrackStart_Time.at(ProtonIndex),TrackStart_Time.at(PionIndex)};

      // Check the seeds are not separated by dead channels
      if(!c.SeedDeadWireCheck(TrackStart_Channel_MPP,TrackStart_Time_MPP,plane)){

         std::pair<int,int> thisClusterIDSize;
         std::vector<int> ClusterIDs;
         std::vector<int> ClusterSizes;

         thisClusterIDSize = c.MakeCluster(TrackStart_Channel.at(MuonIndex),TrackStart_Time.at(MuonIndex),1);
         if(thisClusterIDSize.first > 0){ 
            ClusterIDs.push_back(thisClusterIDSize.first); 
            ClusterSizes.push_back(thisClusterIDSize.second);
         }
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel.at(ProtonIndex),TrackStart_Time.at(ProtonIndex),2);
         if(thisClusterIDSize.first > 0){ 
            ClusterIDs.push_back(thisClusterIDSize.first);
            ClusterSizes.push_back(thisClusterIDSize.second); 
         }
         thisClusterIDSize = c.MakeCluster(TrackStart_Channel.at(PionIndex),TrackStart_Time.at(PionIndex),3);
         if(thisClusterIDSize.first > 0){ 
            ClusterIDs.push_back(thisClusterIDSize.first);
            ClusterSizes.push_back(thisClusterIDSize.second); 
         }

         // Selection criteria: ID vector should read 1 2 2 (muon and proton/pion merged) 
         // Clusters should all be of at least size MinSize
         pass = ClusterIDs.size() == 3 && ClusterIDs.at(0) == 1 && ClusterIDs.at(1) == 2 && ClusterIDs.at(2) == 2 && ClusterSizes.at(0) > MinSize && ClusterSizes.at(1) > MinSize && ClusterSizes.at(2) > MinSize;
      }

      if(Draw) c.DrawClustered(rse,plane,pass); 
      c.Reset();
      return pass;
   }

// Example root macro using clustering tool

//  infile = file containing wire trees
//  intracklist = text file with indices of tracks (muon proton pion) to use for seeding clusters

void HyperonSelectionTool(std::string infile,std::string intracklist){

   std::string indir = "../data/";

   // Open the input file with wire trees
   TFile *f = TFile::Open((indir + infile).c_str());

   // Open the track list file (indices of muon, proton and pion candidates)
   std::ifstream tracklist(indir + intracklist);

   //////////////////////////////////////////////////////////////////////////////////////////////////////////////

   gSystem->Exec("mkdir -p EventLists/");

   // Create a file to hold the list of selected events
   std::string name = indir + infile;
   while(name.find('/') != std::string::npos)
      name = name.substr(name.find('/') + 1);

   std::ofstream out("EventLists/" + name + "_Selected.txt");

   c.LoadDeadWireMaps("../DeadWireMaps/");
   c.SetThreshold(Threshold);

   // Setup the cluster builder object and load dead wire maps
   // initialize with false if you don't want to draw plots
   //Connectedness::ClusterBuilder c(Draw,PlotDir);

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

   Long64_t nentries = t->GetEntriesFast();

   // Event loop
   for(size_t j=0;j<nentries;j++){

      t->GetEntry(j);

      std::string PF_Plane0 = "Fail"; 
      std::string PF_Plane1 = "Fail"; 
      std::string PF_Plane2 = "Fail"; 

      if(Debug) std::cout << "Processing event " << j << std::endl;

      // Get the indexes of the muon, decay proton/pion candidates in the track list
      int muon_index,proton_index,pion_index;
      tracklist >> muon_index >> proton_index >> pion_index;

      if(Debug) std::cout << "Muon=" << muon_index << "  DecayProton=" << proton_index << "  DecayPion=" << pion_index << std::endl;

      std::string rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event);

      // Number of planes passing selection criteria
      int Planes_Passed = 0;

      std::pair<int,int> thisClusterIDSize;

      if(TestPlane(rse+"_Plane0",0,*TrackStart_Channel_Plane0,*TrackStart_Time_Plane0,muon_index,proton_index,pion_index,*Wire_Channel_Plane0,*Wire_Tick_Plane0,*Wire_Signal_Plane0)){
         PF_Plane0 = "Pass";
         Planes_Passed++;
      }

      if(TestPlane(rse+"_Plane1",0,*TrackStart_Channel_Plane1,*TrackStart_Time_Plane1,muon_index,proton_index,pion_index,*Wire_Channel_Plane1,*Wire_Tick_Plane1,*Wire_Signal_Plane1)){
         PF_Plane1 = "Pass";
         Planes_Passed++;
      }
      if(TestPlane(rse+"_Plane2",0,*TrackStart_Channel_Plane2,*TrackStart_Time_Plane2,muon_index,proton_index,pion_index,*Wire_Channel_Plane2,*Wire_Tick_Plane2,*Wire_Signal_Plane2)){
         PF_Plane2 = "Pass";
         Planes_Passed++;
      }

      if(Planes_Passed > 0){
         selected_events++; 
         out << run << "  " << subrun << "  " << event << std::endl;
      }
      else std::cout << "Rejected event " << run << "  " << subrun << "  " << event << std::endl;

      total_events++;

      rse = "run_"  + std::to_string(run) + "_subrun_"  + std::to_string(subrun) + "_event_"  + std::to_string(event);

      gSystem->Exec(("mkdir -p Displays/" + rse).c_str()); 
      gSystem->Exec(("mv Displays/" + PlotDir + "/" + rse + "_* " + "Displays/" + rse).c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane0/Binary.pdf Displays/" + rse + "/" + PF_Plane0 + "_Binary_Plane0.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane1/Binary.pdf Displays/" + rse + "/" + PF_Plane1 + "_Binary_Plane1.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane2/Binary.pdf Displays/" + rse + "/" + PF_Plane2 + "_Binary_Plane2.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane0/Raw.pdf Displays/" + rse + "/" + PF_Plane0 + "_Raw_Plane0.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane1/Raw.pdf Displays/" + rse + "/" + PF_Plane1 + "_Raw_Plane1.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane2/Raw.pdf Displays/" + rse + "/" + PF_Plane2 + "_Raw_Plane2.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane0/Clustered.pdf Displays/" + rse + "/" + PF_Plane0 + "_Clustered_Plane0.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane1/Clustered.pdf Displays/" + rse + "/" + PF_Plane1 + "_Clustered_Plane1.pdf").c_str()); 
      gSystem->Exec(("mv Displays/" + rse + "/" + rse + "_Plane2/Clustered.pdf Displays/" + rse + "/" + PF_Plane2 + "_Clustered_Plane2.pdf").c_str()); 
      gSystem->Exec(("rm -rf Displays/" + rse + "/" + rse + "_Plane2").c_str());
      gSystem->Exec(("rm -rf Displays/" + rse + "/" + rse + "_Plane0").c_str());
      gSystem->Exec(("rm -rf Displays/" + rse + "/" + rse + "_Plane1").c_str());

   }//event loop

   std::cout << "Selected " << selected_events << "/" << total_events << " events" << std::endl;
   out.close();
}

