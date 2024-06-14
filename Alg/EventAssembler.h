#ifndef _EventAssembler_h_
#define _EventAssembler_h_

#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"

#include "Shared.h"

using std::vector;
using std::string;

namespace Connectedness {


   struct Event {

      UInt_t          EventID;
      Int_t           run;
      Int_t           subrun;
      Int_t           event;
      vector<vector<int>*>  Wire_Channel;
      vector<vector<int>*>  Wire_Tick;
      vector<vector<double>*>  Wire_Signal;
      vector<int>*     TrackIndex;
      vector<vector<int>*>     TrackStart_Channel;
      vector<vector<int>*>     TrackStart_Time;
      vector<double>  *TrackStart_X;
      vector<double>  *TrackStart_Y;
      vector<double>  *TrackStart_Z;
      vector<double>  *TrackDir_X;
      vector<double>  *TrackDir_Y;
      vector<double>  *TrackDir_Z;
   };

   class EventAssembler {

      public:

         // Setters and getters
         void SetFile(std::string infilename);
         void Close();

         Event GetEvent(int i);
         Long64_t GetNEvents(){ return nEvents; }

      private:

         // Input file and event tree
         TFile * f_in=nullptr;
         TTree * t_in=nullptr;
         int nEvents;

         UInt_t          EventID;
         Int_t           run;
         Int_t           subrun;
         Int_t           event;
         vector<vector<int>*>     Wire_Channel;
         vector<vector<int>*>     Wire_Tick;
         vector<vector<double>*>  Wire_Signal;
         vector<int>*     TrackIndex=0;
         vector<vector<int>*>     TrackStart_Channel;
         vector<vector<int>*>     TrackStart_Time;
         vector<double>  *TrackStart_X=0;
         vector<double>  *TrackStart_Y=0;
         vector<double>  *TrackStart_Z=0;
         vector<double>  *TrackDir_X=0;
         vector<double>  *TrackDir_Y=0;
         vector<double>  *TrackDir_Z=0;

   };

}

#endif
