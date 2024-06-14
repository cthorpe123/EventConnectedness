#ifndef _ClusterBuilder_cxx_
#define _ClusterBuilder_cxx_

#include "ClusterBuilder.h"

using namespace Connectedness;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::ClusterBuilder(){

   DrawEverything = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::ClusterBuilder(bool draw,std::string displaydir){

   DrawEverything = draw;

   system("mkdir -p Displays/");

   if(DrawEverything){

      c = new TCanvas("c","c");

      if(displaydir != "") DisplayDir = displaydir;

      system(("mkdir -p Displays/" + DisplayDir).c_str());
      system(("mkdir -p Displays/" + DisplayDir + "/Pass/").c_str());
      system(("mkdir -p Displays/" + DisplayDir + "/Fail/").c_str());

   }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClusterBuilder::~ClusterBuilder(){

   //   delete h_Raw;
   //   delete h_Binary;
   //   delete h_Clustered;

   if(DrawEverything) delete c;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ClusterBuilder::LoadDeadWireMaps(std::string dir){

   std::cout << "Getting dead channel lists" << std::endl;

   std::ifstream input_DeadChannels_Plane0(dir + "Plane0.txt");
   std::ifstream input_DeadChannels_Plane1(dir + "Plane1.txt");
   std::ifstream input_DeadChannels_Plane2(dir + "Plane2.txt");

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

void ClusterBuilder::SetOffsets(int x_offset,int y_offset){

   XOffset = x_offset;
   YOffset = y_offset;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::SetSeachArea(int x_max,int y_max){

   MaxSearchX = x_max;
   MaxSearchY = y_max;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::SetGrowthArea(double area){

   GrowthArea = area;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::ClearClusters(){

   Clusters.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::Reset(){

   Clusters.clear();
   SeedChannels.clear();
   SeedTicks.clear();

   if(h_Raw != nullptr) delete h_Raw;
   if(h_Binary != nullptr) delete h_Binary;
   if(h_Clustered != nullptr) delete h_Clustered;

   h_Raw = nullptr;
   h_Binary = nullptr;
   h_Clustered = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::ReadData(const std::vector<int>* channel,const std::vector<int>* tick,const std::vector<double>* signal,std::string rse){

   // Channel, tick and signal vectors should all be of the same size
   assert(channel->size() == tick->size() && tick->size() == signal->size());

   int max_ch=-10000,min_ch=10000000;
   double max_t=-1e10,min_t=1e10;

   for(size_t i=0;i<channel->size();i++){
      if(channel->at(i) > max_ch) max_ch = channel->at(i);
      if(tick->at(i) > max_t) max_t = tick->at(i);
      if(channel->at(i) < min_ch) min_ch = channel->at(i);
      if(tick->at(i) < min_t) min_t = tick->at(i);
   }

   int nchannels = max_ch - min_ch;
   int nticks = max_t - min_t;

   // Setup the histograms
   h_Raw = new TH2D(("h_Channel_vs_Tick_Raw_"+rse).c_str(),"Raw Activity;Channel;Tick",nchannels,min_ch,max_ch,nticks,min_t,max_t);
   h_Binary = new TH2D(("h_Channel_vs_Tick_Binary_"+rse).c_str(),"Binary Activity;Channel;Tick",nchannels,min_ch,max_ch,nticks,min_t,max_t);

   // Fill the histograms
   for(size_t i=0;i<channel->size();i++){
      h_Raw->Fill(channel->at(i),tick->at(i),signal->at(i));
      if(signal->at(i) > Threshold) h_Binary->Fill(channel->at(i),tick->at(i),1);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::ReadData(TH2D* h_Activity,std::string rse){

   h_Raw = (TH2D*)h_Activity->Clone(("h_Channel_vs_Tick_Raw_"+rse).c_str());
   h_Raw->SetTitle("Raw Activity;Channel;Tick");

   h_Binary = (TH2D*)h_Activity->Clone(("h_Channel_vs_Tick_Binary_"+rse).c_str());
   h_Binary->SetTitle("Binary Activity;Channel;Tick");
   h_Binary->Reset();

   for(int i_x=1;i_x<h_Raw->GetNbinsX()+1;i_x++)
      for(int i_y=1;i_y<h_Raw->GetNbinsY()+1;i_y++)
         if(h_Raw->GetBinContent(i_x,i_y) > Threshold) h_Binary->SetBinContent(i_x,i_y,1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::pair<int,int> ClusterBuilder::MakeCluster(int seed_channel,int seed_tick,int ID){

   // Get seed location in bin space
   int seed_channel_b = h_Raw->GetXaxis()->FindBin(seed_channel-XOffset);
   int seed_tick_b = h_Raw->GetYaxis()->FindBin(seed_tick-YOffset);

   // Check seed bin is occupied
   if(h_Binary->GetBinContent(seed_channel_b,seed_tick_b) < 1){
      std::pair<int,int> bins = FindNearestOccupiedBin(h_Binary,seed_channel_b,seed_tick_b);
      if(bins.first == -1 && bins.second == -1){
         SeedChannels.push_back(seed_channel_b);
         SeedTicks.push_back(seed_tick_b);
         return std::make_pair(-1,-1);
      }
      else {
         seed_channel_b = bins.first;
         seed_tick_b = bins.second;
      }
   }

   SeedChannels.push_back(seed_channel_b);
   SeedTicks.push_back(seed_tick_b);

   // Check if this is a bin in the list of clusters already produced, if it is
   // return the ID of that cluster

   for(size_t i_cl=0;i_cl<Clusters.size();i_cl++){

      Cluster thisCluster = Clusters.at(i_cl);

      for(size_t i_b=0;i_b<thisCluster.bins_x.size();i_b++){
         if(seed_channel_b == thisCluster.bins_x.at(i_b) && seed_tick_b == thisCluster.bins_y.at(i_b)){ 

            return std::make_pair(thisCluster.ID,thisCluster.bins_x.size());
         }
      }
   }

   // CLUSTER GROWING ALG //

   // Create empty cluster struct
   Cluster C;
   C.ID = ID;
   C.seed_bin_x = seed_channel_b;
   C.seed_bin_y = seed_tick_b; 

   // Add the seed bin
   C.bins_x.push_back(seed_channel_b);
   C.bins_y.push_back(seed_tick_b);

   TH2D h_tmp = *h_Binary; 

   h_tmp.SetBinContent(seed_channel_b,seed_tick_b,3);

   // for debugging only
   //h_tmp.Draw("colz");
   //c->Print("tmp.pdf");   

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

         if(Separation(seed_channel_b,seed_tick_b,current_bin_x,current_bin_y) > GrowthArea) continue; 

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

   return std::make_pair(ID,C.bins_x.size());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::pair<int,int> ClusterBuilder::FindNearestOccupiedBin(TH2D *hist,int x, int y){

   //std::cout << "Checking bins around " << x << "  " << y << std::endl;

   std::pair<int,int> NearestBin = { -1, -1};

   // Search neighbouring bins until you find an occupied one, within the ranges set by max_x and max_y

   for(int i_x=1;i_x<MaxSearchX+1;i_x++){
      for(int i_y=1;i_y<MaxSearchY+1;i_y++){

         if(hist->GetBinContent(x+i_x,y) > 0) { NearestBin = {x+i_x,y}; return NearestBin; }
         if(hist->GetBinContent(x-i_x,y) > 0) { NearestBin = {x-i_x,y}; return NearestBin; }
         if(hist->GetBinContent(x,y+i_y) > 0) { NearestBin = {x,y+i_y}; return NearestBin; }
         if(hist->GetBinContent(x,y-i_y) > 0) { NearestBin = {x,y-i_y}; return NearestBin; }
         if(hist->GetBinContent(x+i_x,y+i_y) > 0) { NearestBin = {x+i_x,y+i_y}; return NearestBin; }
         if(hist->GetBinContent(x-i_x,y+i_y) > 0) { NearestBin = {x-i_x,y+i_y}; return NearestBin; }
         if(hist->GetBinContent(x+i_x,y-i_y) > 0) { NearestBin = {x+i_x,y-i_y}; return NearestBin; }
         if(hist->GetBinContent(x-i_x,y-i_y) > 0) { NearestBin = {x-i_x,y-i_y}; return NearestBin; }

      }
   }

   // Return -1,-1 if no nearby bins are occupied
   return NearestBin;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Cluster> ClusterBuilder::GetClusters(){

   return Clusters;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ClusterBuilder::SeedDeadWireCheck(std::vector<int> seeds_channel,std::vector<int> seeds_tick,int plane){

   if(seeds_channel.size() != seeds_tick.size()){
      std::cout << "Input channel/tick/signal vectors of different sizes! Exiting" << std::endl;
      return true;
   }

   // if there is zero or 1 seed, this check is meaningless
   if(seeds_channel.size() < 2) return false;

   // find lowest/highest channel seeds
   int min_ch=seeds_channel.at(0)-XOffset;
   int max_ch=seeds_channel.at(0)-XOffset;

   for(size_t i_s=1;i_s<seeds_channel.size();i_s++){
      if(seeds_channel.at(i_s)-XOffset < min_ch) min_ch = seeds_channel.at(i_s)-XOffset;
      if(seeds_channel.at(i_s)-XOffset > max_ch) max_ch = seeds_channel.at(i_s)-XOffset;
   }

   int ch=min_ch;

   while(ch <= max_ch){
      if(plane == kPlane0 && (std::find(DeadChannels_Plane0.begin(),DeadChannels_Plane0.end(),ch) != DeadChannels_Plane0.end())) return true;
      if(plane == kPlane1 && (std::find(DeadChannels_Plane1.begin(),DeadChannels_Plane1.end(),ch) != DeadChannels_Plane1.end())) return true;
      if(plane == kPlane2 && (std::find(DeadChannels_Plane2.begin(),DeadChannels_Plane2.end(),ch) != DeadChannels_Plane2.end())) return true;
      ch++;
   }

   return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawRaw(std::string rse,int plane,int pass){

   if(!DrawEverything) return;

   std::string dir = DisplayDir + "/" + rse + "/";
   system(("mkdir -p " + dir).c_str());

   h_Raw->SetContour(100);
   h_Raw->SetStats(0);
   h_Raw->SetTitle(rse.c_str());
   h_Raw->Draw("colz");

   std::string name;
   if(pass == -1) name = dir + "Plane" + std::to_string(plane) + "_Raw.pdf";
   if(pass == 0) name = dir + "Plane" + std::to_string(plane) + "_Fail_Raw.pdf";
   if(pass == 1) name = dir + "Plane" + std::to_string(plane) + "_Pass_Raw.pdf";
   c->Print(name.c_str());

   c->Clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawBinary(std::string rse,int plane,int pass){

   if(!DrawEverything) return;

   std::string dir = DisplayDir + "/" + rse + "/";
   system(("mkdir -p " + dir).c_str());

   Int_t colors[] = {0,4 }; // #colors >= #levels - 1
   gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);

   h_Binary->SetStats(0);
   h_Binary->SetTitle(rse.c_str());
   h_Binary->Draw("colz");

   std::string name;
   if(pass == -1) name = dir + "Plane" + std::to_string(plane) + "_Binary.pdf";
   if(pass == 0) name = dir + "Plane" + std::to_string(plane) + "_Fail_Binary.pdf";
   if(pass == 1) name = dir + "Plane" + std::to_string(plane) + "_Pass_Binary.pdf";
   c->Print(name.c_str());

   c->Clear();
   gStyle->SetPalette();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DrawClustered(std::string rse,int plane,int pass){

   if(!DrawEverything) return;

   std::string dir = DisplayDir + "/" + rse + "/";
   system(("mkdir -p " + dir).c_str());

   h_Clustered = (TH2D*)h_Binary->Clone();

   // Draw on dead wires - makes pdfs much bigger so use only if needed
   //if(plane != -1) DeadWireFill(plane);

   // Set color palette
   int nclusters = Clusters.size();

   if(nclusters == 0){
      Int_t colors[] = {2,3,4};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else if(nclusters == 1){
      Int_t colors[] = {2,3,4,6};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else if(nclusters == 2){
      Int_t colors[] = {2,3,4,6,7};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else if(nclusters == 3){
      Int_t colors[] = {2,3,4,6,7,8};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else if(nclusters == 4){
      Int_t colors[] = {2,3,4,6,7,8,46};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else if(nclusters == 5){
      Int_t colors[] = {2,3,4,6,7,8,46,12};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }
   else{
      Int_t colors[] = {2,3,4,6,7,8,46,12,40};
      gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
   }

   // set bin heights equal to cluster IDs
   for(size_t i_cl=0;i_cl<Clusters.size();i_cl++)
      for(size_t i_b=0;i_b<Clusters.at(i_cl).bins_x.size();i_b++)
         h_Clustered->SetBinContent(Clusters.at(i_cl).bins_x.at(i_b),Clusters.at(i_cl).bins_y.at(i_b),i_cl+2);

   // Draw the positions of the seeds
   for(size_t i_s=0;i_s<SeedChannels.size();i_s++){
      for(int i_c=SeedChannels.at(i_s)-1;i_c<SeedChannels.at(i_s)+2;i_c++)
         for(int i_t=SeedTicks.at(i_s)-5;i_t<SeedTicks.at(i_s)+6;i_t++)
            h_Clustered->SetBinContent(i_c,i_t,Clusters.size()+2);
   }

   // Set x and y ranges
   Focus();

   h_Clustered->SetStats(0);
   h_Clustered->SetTitle(rse.c_str());
   h_Clustered->Draw("colz");

   std::string name;
   if(pass == -1) name = dir + "Plane" + std::to_string(plane) + "_Clustered.pdf";
   if(pass == 0) name = dir + "Plane" + std::to_string(plane) + "_Fail_Clustered.pdf";
   if(pass == 1) name = dir + "Plane" + std::to_string(plane) + "_Pass_Clustered.pdf";
   c->Print(name.c_str());

   c->Clear();

   DrawBinary(rse,plane,pass);
   DrawRaw(rse,plane,pass); 

   gStyle->SetPalette();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::DeadWireFill(int plane){

   if(plane == kPlane0)
      for(size_t i=0;i<DeadChannels_Plane0.size();i++)
         if(DeadChannels_Plane0.at(i) > h_Clustered->GetXaxis()->GetBinLowEdge(1) && DeadChannels_Plane0.at(i) < h_Clustered->GetXaxis()->GetBinLowEdge(h_Clustered->GetNbinsX()))
            for(int j=0;j<h_Clustered->GetNbinsY();j++)
               h_Clustered->Fill(DeadChannels_Plane0.at(i),h_Clustered->GetYaxis()->GetBinCenter(j),-1);

   if(plane == kPlane1)
      for(size_t i=0;i<DeadChannels_Plane1.size();i++)
         if(DeadChannels_Plane1.at(i) > h_Clustered->GetXaxis()->GetBinLowEdge(1) && DeadChannels_Plane1.at(i) < h_Clustered->GetXaxis()->GetBinLowEdge(h_Clustered->GetNbinsX()))
            for(int j=0;j<h_Clustered->GetNbinsY();j++)
               h_Clustered->Fill(DeadChannels_Plane1.at(i),h_Clustered->GetYaxis()->GetBinCenter(j),-1);

   if(plane == kPlane2)
      for(size_t i=0;i<DeadChannels_Plane2.size();i++)
         if(DeadChannels_Plane2.at(i) > h_Clustered->GetXaxis()->GetBinLowEdge(1) && DeadChannels_Plane2.at(i) < h_Clustered->GetXaxis()->GetBinLowEdge(h_Clustered->GetNbinsX()))
            for(int j=0;j<h_Clustered->GetNbinsY();j++)
               h_Clustered->Fill(DeadChannels_Plane2.at(i),h_Clustered->GetYaxis()->GetBinCenter(j),-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::Focus(){

   // Find range of x and y spanned by clusters, only draw this range + a bit of padding

   int min_x=10000,max_x=-10000,min_y=10000,max_y=-10000;

   for(size_t i_c=0;i_c<Clusters.size();i_c++){

      Cluster c = Clusters.at(i_c);

      for(size_t i_b=0;i_b<c.bins_x.size();i_b++){
         if(c.bins_x.at(i_b) > max_x) max_x = c.bins_x.at(i_b);
         if(c.bins_x.at(i_b) < min_x) min_x = c.bins_x.at(i_b);
         if(c.bins_y.at(i_b) > max_y) max_y = c.bins_y.at(i_b);
         if(c.bins_y.at(i_b) < min_y) min_y = c.bins_y.at(i_b);
      }
   }

   double  range_x = max_x - min_x;
   double  range_y = max_y - min_y;

   h_Clustered->GetXaxis()->SetRange(min_x-range_x*0.2-100,max_x+range_x*0.2+100);
   h_Clustered->GetYaxis()->SetRange(min_y-range_y*0.2-500,max_y+range_y*0.2+500);
   h_Raw->GetXaxis()->SetRange(min_x-range_x*0.2-100,max_x+range_x*0.2+100);
   h_Raw->GetYaxis()->SetRange(min_y-range_y*0.2-500,max_y+range_y*0.2+500);
   h_Binary->GetXaxis()->SetRange(min_x-range_x*0.2-100,max_x+range_x*0.2+100);
   h_Binary->GetYaxis()->SetRange(min_y-range_y*0.2-500,max_y+range_y*0.2+500);

   //h_Clustered->SetBinContent(min_x-range_x*0.2+1,min_y-range_y*0.2+1,);
   h_Clustered->SetBinContent(min_x-range_x*0.2+1,min_y-range_y*0.2+2,Clusters.size()+1.0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ClusterBuilder::SetDisplayDir(std::string dir){

   DisplayDir = dir;

   system(("mkdir -p Displays/" + DisplayDir).c_str());
   system(("mkdir -p Displays/" + DisplayDir + "/Pass/").c_str());
   system(("mkdir -p Displays/" + DisplayDir + "/Fail/").c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
