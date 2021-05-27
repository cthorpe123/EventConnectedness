#ifndef _ClusterBuilder_cxx_
#define _ClusterBuilder_cxx_

#include "ClusterBuilder.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::ClusterBuilder(){

   DrawEverything = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::ClusterBuilder(bool draw){

   DrawEverything = draw;

   if(DrawEverything) c = new TCanvas("c","c");

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::~ClusterBuilder(){

   delete h_Raw;
   delete h_Binary;
   delete c;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ClusterBuilder::LoadDeadWireMaps(std::string dir){

   std::cout << "Getting dead channel lists" << std::endl;

   std::ifstream input_DeadChannels_Plane0(dir + "/Plane0.txt");
   std::ifstream input_DeadChannels_Plane1(dir + "/Plane1.txt");
   std::ifstream input_DeadChannels_Plane2(dir + "/Plane2.txt");

   int dead;

   while(!input_DeadChannels_Plane0.eof()){ input_DeadChannels_Plane0 >> dead; DeadChannels_Plane0.push_back(dead); }
   while(!input_DeadChannels_Plane1.eof()){ input_DeadChannels_Plane1 >> dead; DeadChannels_Plane1.push_back(dead); }
   while(!input_DeadChannels_Plane2.eof()){ input_DeadChannels_Plane2 >> dead; DeadChannels_Plane2.push_back(dead); }


   std::cout << "Found " << DeadChannels_Plane0.size() << " dead channels in Plane0" << std::endl;
   std::cout << "Found " << DeadChannels_Plane1.size() << " dead channels in Plane1" << std::endl;
   std::cout << "Found " << DeadChannels_Plane2.size() << " dead channels in Plane2" << std::endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::SetThreshold(double threshold){

   Threshold = threshold;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::ReadData(std::vector<int> channel,std::vector<int> tick,std::vector<double> signal,std::string rse){

   // Channel, tick and signal vectors should all be of the same size
   if(channel.size() != tick.size() || tick.size() != signal.size()){ 
      std::cout << "Input channel/tick/signal vectors of different sizes! Exiting" << std::endl;
      return;
   }

   int max_ch=-10000,min_ch=10000000;
   double max_t=-1e10,min_t=1e10;

   for(size_t i=0;i<channel.size();i++){
      if(channel.at(i) > max_ch) max_ch = channel.at(i);
      if(tick.at(i) > max_t) max_t = tick.at(i);
      if(channel.at(i) < min_ch) min_ch = channel.at(i);
      if(tick.at(i) < min_t) min_t = tick.at(i);
   }

   int nchannels = max_ch - min_ch;
   int nticks = max_t - min_t;

   // Setup the histograms
   h_Raw = new TH2D(("h_Channel_vs_Tick_Raw_"+rse).c_str(),"Raw Activity;Channel;Tick",nchannels,min_ch,max_ch,nticks,min_t,max_t);
   h_Binary = new TH2D(("h_Channel_vs_Tick_Binary_"+rse).c_str(),"Binary Activity;Channel;Tick",nchannels,min_ch,max_ch,nticks,min_t,max_t);

   // Fill the histograms
   for(size_t i=0;i<channel.size();i++){
      h_Raw->Fill(channel.at(i),tick.at(i),signal.at(i));
      if(signal.at(i) > Threshold) h_Binary->Fill(channel.at(i),tick.at(i),1);
   }

   // Draw the raw and binary histograms if desired
   if(DrawEverything){

      DrawRaw(rse);
      DrawBinary(rse);

   }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ClusterBuilder::MakeCluster(int seed_channel,int seed_tick,int ID){

   //std::cout << "Building cluster " << ID << std::endl;

   // Get seed location in bin space
   int seed_channel_b = h_Raw->GetXaxis()->FindBin(seed_channel-x_offset);
   int seed_tick_b = h_Raw->GetYaxis()->FindBin(seed_tick-y_offset);

   // Check seed bin is occupied
   if(h_Binary->GetBinContent(seed_channel_b,seed_tick_b) < 1){
      //std::cout << "Seed of cluster " << ID << " is in empty bin, return -1" << std::endl;
      return -1;
   }

   // Check if this is a bin in the list of clusters already produced, if it is
   // return the ID of that cluster

   for(size_t i_cl=0;i_cl<Clusters.size();i_cl++){

      Cluster thisCluster = Clusters.at(i_cl);

      for(size_t i_b=0;i_b<thisCluster.bins_x.size();i_b++){
         if(seed_channel_b == thisCluster.bins_x.at(i_b) && seed_tick_b == thisCluster.bins_y.at(i_b)){ 
            //std::cout << "Seed already belongs to cluster " << thisCluster.ID << std::endl;
            return thisCluster.ID;
         }
      }

   }


   // CLUSTER GROWING ALG //

   // Create empty cluster struct
   Cluster C;
   C.ID = ID;

   // Add the seed bin
   C.bins_x.push_back(seed_channel_b);
   C.bins_y.push_back(seed_tick_b);

   TH2D h_tmp = *h_Binary; 

   h_tmp.SetBinContent(seed_channel_b,seed_tick_b,3);

   // for debugging only
   //h_tmp.Draw("colz");
   //c->Print("tmp.png");   

   int neighbour_x;
   int neighbour_y;

   std::vector<std::pair<int,int>> BinsAddedLastPass;
   std::vector<std::pair<int,int>> BinsAddedThisPass;

   BinsAddedLastPass.push_back(std::make_pair(seed_channel_b,seed_tick_b));

   int nfills_this_pass = 1;

   while(nfills_this_pass > 0){

      nfills_this_pass = 0;
      BinsAddedThisPass.clear();

      // iterate over the bins added in the last pass, check the bins that neighbour those
      for(size_t i_b=0;i_b<BinsAddedLastPass.size();i_b++){

         int current_bin_x = BinsAddedLastPass.at(i_b).first;
         int current_bin_y = BinsAddedLastPass.at(i_b).second;

         // look at each of the eight bins surrounding the current one

         neighbour_x = current_bin_x + 1;
         neighbour_y = current_bin_y;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x - 1;
         neighbour_y = current_bin_y;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x;
         neighbour_y = current_bin_y + 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x;
         neighbour_y = current_bin_y - 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }


         neighbour_x = current_bin_x + 1;
         neighbour_y = current_bin_y + 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x - 1;
         neighbour_y = current_bin_y + 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x + 1;
         neighbour_y = current_bin_y - 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }

         neighbour_x = current_bin_x - 1;
         neighbour_y = current_bin_y - 1;                                
         // if bin is occupied, and not already part of the cluster, add it
         if(h_tmp.GetBinContent(neighbour_x,neighbour_y) > 0 && h_tmp.GetBinContent(neighbour_x,neighbour_y) < 2){ 
            h_tmp.SetBinContent(neighbour_x,neighbour_y,3);
            BinsAddedThisPass.push_back(std::make_pair(neighbour_x,neighbour_y));               
            C.bins_x.push_back(neighbour_x);   
            C.bins_y.push_back(neighbour_y);
            nfills_this_pass++;   
         }


      }//i_b

      BinsAddedLastPass = BinsAddedThisPass;

   } //while(nfills_this_pass > 0)


   Clusters.push_back(C);

   return ID;

   // for debugging only
   // h_tmp.Draw("colz");
   // c->Print("tmp.png");   

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Cluster> ClusterBuilder::GetClusters(){

return Clusters;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ClusterBuilder::SeedDeadWireCheck(std::vector<int> seeds_channel,std::vector<int> seeds_tick,int plane){

//std::cout << "Checking if seeds are separated by dead channels" << std::endl;

if(seeds_channel.size() != seeds_tick.size()){
      std::cout << "Input channel/tick/signal vectors of different sizes! Exiting" << std::endl;
      return true;
}

// if there is zero or 1 seed, this check is meaningless
if(seeds_channel.size() < 2) return false;

// find lowest/highest channel seeds
int min_ch=seeds_channel.at(0)-x_offset;
int max_ch=seeds_channel.at(0)-x_offset;

for(size_t i_s=1;i_s<seeds_channel.size();i_s++){

if(seeds_channel.at(i_s)-x_offset < min_ch) min_ch = seeds_channel.at(i_s)-x_offset;
if(seeds_channel.at(i_s)-x_offset > max_ch) max_ch = seeds_channel.at(i_s)-x_offset;

}

int ch=min_ch;

while(ch <= max_ch){

   // Search the list of dead channels
   if(plane == 0 && (std::find(DeadChannels_Plane0.begin(),DeadChannels_Plane0.end(),ch) != DeadChannels_Plane0.end())){ return true; }
   if(plane == 1 && (std::find(DeadChannels_Plane1.begin(),DeadChannels_Plane1.end(),ch) != DeadChannels_Plane1.end())){ return true; }
   if(plane == 2 && (std::find(DeadChannels_Plane2.begin(),DeadChannels_Plane2.end(),ch) != DeadChannels_Plane2.end())){ return true; }

   ch++;
}
         

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawRaw(std::string rse){

   std::string cmd = "mkdir -p Plots";  
   system(cmd.c_str()); 
   cmd = "mkdir -p Plots/" + rse + "/";
   system(cmd.c_str());

   h_Raw->SetContour(100);
   h_Raw->SetStats(0);
   h_Raw->SetTitle(rse.c_str());
   h_Raw->Draw("colz");
   c->Print("Raw.png");

   cmd = "mv Raw.png Plots/" + rse + "/";
   system(cmd.c_str());

   c->Clear();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawBinary(std::string rse){

   std::string cmd = "mkdir -p Plots";  
   system(cmd.c_str()); 
   cmd = "mkdir -p Plots/" + rse + "/";
   system(cmd.c_str());

   Int_t colors[] = {0,4 }; // #colors >= #levels - 1
   gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);

   h_Binary->SetStats(0);
   h_Binary->SetTitle(rse.c_str());
   h_Binary->Draw("colz");
   c->Print("Binary.png");

   c->Clear();

   cmd = "mv Binary.png Plots/" + rse + "/";
   system(cmd.c_str());

   gStyle->SetPalette();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawClustered(std::string rse){

   std::string cmd = "mkdir -p Plots";  
   system(cmd.c_str()); 
   cmd = "mkdir -p Plots/" + rse + "/";
   system(cmd.c_str());

   // set bin heights equal to cluster IDs
   for(size_t i_cl=0;i_cl<Clusters.size();i_cl++){

      for(size_t i_b=0;i_b<Clusters.at(i_cl).bins_x.size();i_b++){

         h_Binary->SetBinContent(Clusters.at(i_cl).bins_x.at(i_b),Clusters.at(i_cl).bins_y.at(i_b),Clusters.at(i_cl).ID+2);

      }//i_b

   }//i_cl
   
   h_Binary->SetStats(0);
   h_Binary->SetTitle(rse.c_str());
   h_Binary->Draw("colz");
   c->Print("Clustered.png");

   cmd = "mv Clustered.png Plots/" + rse + "/";
   system(cmd.c_str());

   c->Clear();


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
