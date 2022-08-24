#ifndef _EventAssembler_h_
#define _EventAssembler_h_

#include <vector>

#include "TFile.h"
#include "TTree.h"

using std::vector;

namespace Connectedness {

   struct Event {

      UInt_t          EventID;
      Int_t           run;
      Int_t           subrun;
      Int_t           event;
      vector<int>     Wire_Channel_Plane0;
      vector<int>     Wire_Tick_Plane0;
      vector<double>  Wire_Signal_Plane0;
      vector<int>     Wire_Channel_Plane1;
      vector<int>     Wire_Tick_Plane1;
      vector<double>  Wire_Signal_Plane1;
      vector<int>     Wire_Channel_Plane2;
      vector<int>     Wire_Tick_Plane2;
      vector<double>  Wire_Signal_Plane2;
      vector<int>     TrackStart_Channel_Plane0;
      vector<int>     TrackStart_Time_Plane0;
      vector<int>     TrackStart_Channel_Plane1;
      vector<int>     TrackStart_Time_Plane1;
      vector<int>     TrackStart_Channel_Plane2;
      vector<int>     TrackStart_Time_Plane2;
      vector<double>  TrackStart_X;
      vector<double>  TrackStart_Y;
      vector<double>  TrackStart_Z;
      vector<double>  TrackDir_X;
      vector<double>  TrackDir_Y;
      vector<double>  TrackDir_Z;
      vector<int>     CaloStart_Channel_Plane0;
      vector<int>     CaloStart_Time_Plane0;
      vector<double>  CaloStart_X_Plane0;
      vector<double>  CaloStart_Y_Plane0;
      vector<double>  CaloStart_Z_Plane0;
      vector<int>     CaloStart_Channel_Plane1;
      vector<int>     CaloStart_Time_Plane1;
      vector<double>  CaloStart_X_Plane1;
      vector<double>  CaloStart_Y_Plane1;
      vector<double>  CaloStart_Z_Plane1;
      vector<int>     CaloStart_Channel_Plane2;
      vector<int>     CaloStart_Time_Plane2;
      vector<double>  CaloStart_X_Plane2;
      vector<double>  CaloStart_Y_Plane2;
      vector<double>  CaloStart_Z_Plane2;
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
         vector<int>     *Wire_Channel_Plane0=0;
         vector<int>     *Wire_Tick_Plane0=0;
         vector<double>  *Wire_Signal_Plane0=0;
         vector<int>     *Wire_Channel_Plane1=0;
         vector<int>     *Wire_Tick_Plane1=0;
         vector<double>  *Wire_Signal_Plane1=0;
         vector<int>     *Wire_Channel_Plane2=0;
         vector<int>     *Wire_Tick_Plane2=0;
         vector<double>  *Wire_Signal_Plane2=0;
         vector<int>     *TrackStart_Channel_Plane0=0;
         vector<int>     *TrackStart_Time_Plane0=0;
         vector<int>     *TrackStart_Channel_Plane1=0;
         vector<int>     *TrackStart_Time_Plane1=0;
         vector<int>     *TrackStart_Channel_Plane2=0;
         vector<int>     *TrackStart_Time_Plane2=0;
         vector<double>  *TrackStart_X=0;
         vector<double>  *TrackStart_Y=0;
         vector<double>  *TrackStart_Z=0;
         vector<double>  *TrackDir_X=0;
         vector<double>  *TrackDir_Y=0;
         vector<double>  *TrackDir_Z=0;
         vector<int>     *CaloStart_Channel_Plane0=0;
         vector<int>     *CaloStart_Time_Plane0=0;
         vector<double>  *CaloStart_X_Plane0=0;
         vector<double>  *CaloStart_Y_Plane0=0;
         vector<double>  *CaloStart_Z_Plane0=0;
         vector<int>     *CaloStart_Channel_Plane1=0;
         vector<int>     *CaloStart_Time_Plane1=0;
         vector<double>  *CaloStart_X_Plane1=0;
         vector<double>  *CaloStart_Y_Plane1=0;
         vector<double>  *CaloStart_Z_Plane1=0;
         vector<int>     *CaloStart_Channel_Plane2=0;
         vector<int>     *CaloStart_Time_Plane2=0;
         vector<double>  *CaloStart_X_Plane2=0;
         vector<double>  *CaloStart_Y_Plane2=0;
         vector<double>  *CaloStart_Z_Plane2=0;
   };

}

#endif
