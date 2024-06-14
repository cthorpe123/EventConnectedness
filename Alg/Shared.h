#ifndef _Shared_h_
#define _Shared_h_

#include <fstream>

using std::string;
using std::vector;

namespace Connectedness {

enum e_Planes{kPlane0,kPlane1,kPlane2,kMAXPlane};
const vector<string> str_Planes = {"Plane0","Plane1","Plane2"}; 

inline std::map<std::tuple<int,int,int>,std::vector<int>> MakeSeedMap(std::string input_file){

  std::ifstream tracklist(input_file);
  std::map<std::tuple<int,int,int>,std::vector<int>> track_index_m;
  while(!tracklist.eof()){
    int run,sub,evt,muon,proton,pion;
    tracklist >> run >> sub >> evt >> muon >> proton >> pion;   
    track_index_m[{run,sub,evt}] = {muon,proton,pion};      
  }

  return track_index_m;

}

inline std::vector<int> GetSeeds(int run,int sub,int evt,const std::map<std::tuple<int,int,int>,std::vector<int>>& track_index_m){

  if(track_index_m.find({run,sub,evt}) == track_index_m.end()){
    //std::cout << "Event " << run << " " << sub << " " << evt << " not found in track seed list" << std::endl;
    throw std::invalid_argument("Event " + std::to_string(run) + " " + std::to_string(sub) + " " + std::to_string(evt) + " not found in track seed list");
  }
       
  return track_index_m.at({run,sub,evt}); 

}

}

#endif

