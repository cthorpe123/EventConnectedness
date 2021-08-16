#ifndef _Cluster_h_
#define _Cluster_h_

using namespace Connectedness;

struct Cluster {

int ID;

// Lists of bins in this cluster
std::vector<int> bins_x;
std::vector<int> bins_y;


};

#endif
