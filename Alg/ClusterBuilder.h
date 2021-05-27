#ifndef _ClusterBuilder_h_
#define _ClusterBuilder_h_

#include <iostream>
#include <fstream>

#include "TH2D.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>

#include "Cluster.h"

class ClusterBuilder {

   public: 

      ClusterBuilder();
      ClusterBuilder(bool draw);
      ~ClusterBuilder();

      void LoadDeadWireMaps(std::string dir="../");

      void SetThreshold(double threshold=1.8);

      void ReadData(std::vector<int> channel,std::vector<int> tick,std::vector<double> signal,std::string rse="");

      // Returns the ID of the cluster (check this against list of existing cluster IDs to
      // see if some merging has taken place). Returns -1 if seed landed on empty bin 
      int MakeCluster(int seed_channel,int seed_tick,int ID);

      std::vector<Cluster> GetClusters();

      // Plane quality checks

      // Check seeds are not separated by dead wires
      bool SeedDeadWireCheck(std::vector<int> seeds_channel,std::vector<int> seeds_tick, int plane);

   private:

      bool DrawEverything=false;
      int x_offset = 0;
      int y_offset = 20;


      std::vector<int> DeadChannels_Plane0;
      std::vector<int> DeadChannels_Plane1;
      std::vector<int> DeadChannels_Plane2;

      double Threshold=1.8;

      TH2D *h_Raw;
      TH2D *h_Binary;

      TCanvas *c;

      std::vector<Cluster> Clusters;

   public:

      void DrawRaw(std::string rse="");
      void DrawBinary(std::string rse="");
      void DrawClustered(std::string rse="");

};


#endif


