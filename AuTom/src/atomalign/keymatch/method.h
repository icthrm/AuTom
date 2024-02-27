#ifndef MATCH_METHOD_H__
#define MATCH_METHOD_H__

#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>
#include "dataf/keypoint.h"
#include "ANN/ANN.h"
#include "support.h"

/* the maximum number of keypoint NN candidates to check during BBF search */
#define MAX_PTS_VISIT 200

/* threshold on ratio of Euclidean distance between NN and 2nd NN */
#define RATIO_THR  0.49//0.4//0.6

#define NULPT util::_point(-99999,-99999)

namespace mthd{

typedef std::pair<util::_point, util::_point> pair;

//should change the return format
class ANNTreeMatch{
private:
    bool cashused;
    ANNkd_tree** pkd_tree;
    int nxt_idx;
    int size;
private:
    void FreeMem()
    {
	if(pkd_tree){
	    for(int i = 0; i < size; i++){
		if(pkd_tree[i]){
		    ANNpointArray pts = pkd_tree[i]->thePoints();
		    annDeallocPts(pts);
		    delete pkd_tree[i];
		}
	    }
	    delete [] pkd_tree;
	}
    }
    
    void DoCash(ANNkd_tree* kdt)
    {
	if(!pkd_tree[nxt_idx]){
	    pkd_tree[nxt_idx] = kdt;
	}
    }
    
    ANNkd_tree* GetCash()
    {
	if(!cashused){
	    return NULL;
	}
	return pkd_tree[nxt_idx];
    }
    
    bool IsCashUsed() { return cashused;}
    
public:
    ANNTreeMatch():cashused(false), pkd_tree(NULL), nxt_idx(-1), size(-1){}
    
    ~ANNTreeMatch()
    {
	FreeMem();
    }
    
    void DestoryEngine()
    {
	FreeMem();
	
	pkd_tree = NULL;
	cashused = false;
	nxt_idx = -1;
	size = -1;
    }
    
public:
    void InitEngine(int cash_size)
    {
	size = cash_size;
	cashused = true;
	pkd_tree = new ANNkd_tree*[cash_size];
	for(int i = 0; i < size; i++){
	    pkd_tree[i] = NULL;
	}
    }
    
    void SetCashIndex(int index)
    {
	nxt_idx = index;
    }
    
    void MatchKeys(std::vector<pair>* matches, std::vector<util::feature>& featv1, 
				 std::vector<util::feature>& featv2, int dim = FEATURE_MAX_D,
				 double ratio = RATIO_THR, int max_pts_visit = 200);
};

class ForceMatch{
private:
    bool cashused;
    FeaturePlane** pfplane;
    int nxt_idx;
    int size;
private:
    void FreeMem()
    {
	if(pfplane){
	    for(int i = 0; i < size; i++){
		if(pfplane[i]){
		    delete pfplane[i];
		}
	    }
	    delete [] pfplane;
	}
    }

    void DoCash(FeaturePlane* kdt)
    {
	if(!pfplane[nxt_idx]){
	    pfplane[nxt_idx] = kdt;
	}
    }
    
    FeaturePlane* GetCash()
    {
	if(!cashused){
	    return NULL;
	}
	return pfplane[nxt_idx];
    }    
    
    bool IsCashUsed() { return cashused;}
    
public:
    ForceMatch():cashused(false), pfplane(NULL), nxt_idx(-1){}
    
    ~ForceMatch(){
	FreeMem();
    }
    
    void DestoryEngine()
    {
	FreeMem();
	
	pfplane = NULL;
	cashused = false;
	nxt_idx = -1;
	size = -1;
    }

public:
    void InitEngine(int cash_size)
    {
	size = cash_size;
	cashused = true;
	pfplane = new FeaturePlane*[cash_size];
	for(int i = 0; i < size; i++){
	    pfplane[i] = NULL;
	}
    }

    void SetCashIndex(int index)
    {
	nxt_idx = index;
    }
    
    void MatchKeys(std::vector<pair>* matches, std::vector<util::feature>& featv1, 
				 std::vector<util::feature>& featv2, int world, 
				 float percent = .15f, double ratio = RATIO_THR);
};

}

#endif