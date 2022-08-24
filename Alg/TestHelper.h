#ifndef _TestHelper_h_
#define _TestHelper_h_

#include "ClusterBuilder.h"

// Failure codes:
// kDeadWire = Seeds separated by dead wires
// kSeedMiss = No activity close to one of the seeds
// kMergeProton = Proton merged with the muon
// kMergePion = Pion merged with the muon
// kBothMerge = Proton and pion merged with the muon
// kNoMerge = Proton and Pion didn't merge with each other or the muon
// kTooSmall = One of the clusters is below minsize

enum statuscodes{kPass,kDeadWire,kSeedMiss,kBothMerge,kMergeProton,kMergePion,kNoMerge,kTooSmall};

///////////////////////////////////////////////////////////////////////////////////////////////

int TestPlane(Connectedness::ClusterBuilder* c,std::string rse,int plane,int muon_seed_channel,int muon_seed_time,int proton_seed_channel,int proton_seed_time,int pion_seed_channel,int pion_seed_time,int minsize=50,bool draw=false){

   bool pass = false;

   // Check seeds are not separated by dead wires
   std::vector<int> TrackStarts_Channel = {muon_seed_channel,proton_seed_channel,pion_seed_channel};
   std::vector<int> TrackStarts_Time = {muon_seed_time,proton_seed_time,pion_seed_time};
   bool deadwirefail = c->SeedDeadWireCheck(TrackStarts_Channel,TrackStarts_Time,plane);

   bool seedmiss = false;
   bool mergeproton = false;
   bool mergepion = false;
   bool nomerge = false;  

   std::pair<int,int> thisClusterIDSize;
   std::vector<int> ClusterIDs;
   std::vector<int> ClusterSizes;

   // Check the seeds are not separated by dead channels
   if(!deadwirefail){

      thisClusterIDSize = c->MakeCluster(muon_seed_channel,muon_seed_time,1);
      if(thisClusterIDSize.first == -1 && thisClusterIDSize.second == -1) seedmiss = true;
      if(thisClusterIDSize.first > 0){ 
         ClusterIDs.push_back(thisClusterIDSize.first); 
         ClusterSizes.push_back(thisClusterIDSize.second);            
      }

      thisClusterIDSize = c->MakeCluster(proton_seed_channel,proton_seed_time,2);
      if(thisClusterIDSize.first == -1 && thisClusterIDSize.second == -1) seedmiss = true;
      if(thisClusterIDSize.first > 0){ 
         ClusterIDs.push_back(thisClusterIDSize.first); 
         ClusterSizes.push_back(thisClusterIDSize.second);            
      }

      thisClusterIDSize = c->MakeCluster(pion_seed_channel,pion_seed_time,3);
      if(thisClusterIDSize.first == -1 && thisClusterIDSize.second == -1) seedmiss = true;
      if(thisClusterIDSize.first > 0){ 
         ClusterIDs.push_back(thisClusterIDSize.first); 
         ClusterSizes.push_back(thisClusterIDSize.second);            
      }

      // Selection criteria: ID vector should read 1 2 2 (muon and proton/pion merged) 
      // Clusters should all be of at least size minsize
      pass = ClusterIDs.size() == 3 && ClusterIDs.at(0) == 1 && ClusterIDs.at(1) == 2 && ClusterIDs.at(2) == 2 && ClusterSizes.at(0) > minsize && ClusterSizes.at(1) > minsize && ClusterSizes.at(2) > minsize;
   }

   if(draw) c->DrawClustered(rse,plane,pass); 
   c->Reset();

   // Return one of the status codes
   if(deadwirefail) return kDeadWire;
   if(seedmiss) return kSeedMiss;
   if(ClusterIDs.at(0) == 1 && ClusterIDs.at(1) == 1 && ClusterIDs.at(2) == 1) return kBothMerge;
   if(ClusterIDs.at(0) == 1 && ClusterIDs.at(1) == 1) return kMergeProton;
   if(ClusterIDs.at(0) == 1 && ClusterIDs.at(2) == 1) return kMergePion;
   if(ClusterIDs.at(0) == 1 && ClusterIDs.at(1) == 2 && ClusterIDs.at(2) == 3) return kNoMerge;
   if(ClusterSizes.at(0) < minsize || ClusterSizes.at(1) < minsize || ClusterSizes.at(2) < minsize) return kTooSmall;

   return kPass;
}

///////////////////////////////////////////////////////////////////////////////////////////////

#endif
