#include "keymatch/method.h"
#include "ANN/ANN.h"
#include <climits>

#define  _PT(X)		util::_point((X).kpt.x, (X).kpt.y)

/** @brief Compute likely matches between two sets of keypoints */
void mthd::ANNTreeMatch::MatchKeys(std::vector<pair>* matches, std::vector<util::feature>& featv1, 
						 std::vector<util::feature>& featv2, 
						 int dim, double ratio, int max_pts_visit) 
{
    // Create a search tree for featv2
    ANNkd_tree *tree;
    ANNpointArray pts;
    
    if(!IsCashUsed()){
	// Create a new array of points for featv2
	pts = annAllocPts(featv2.size(), dim);
	for (int i = 0; i < featv2.size(); i++) {
	    memcpy(pts[i], featv2[i].descr, sizeof(unsigned char)*dim);
	}
	tree = new ANNkd_tree(pts, featv2.size(), dim, 16);
    }
    else{
	tree = GetCash();
	if(!tree){
	    pts = annAllocPts(featv2.size(), dim);
	    for (int i = 0; i < featv2.size(); i++) {
		memcpy(pts[i], featv2[i].descr, sizeof(unsigned char)*dim);
	    }
	    
	    tree = new ANNkd_tree(pts, featv2.size(), dim, 16);
	    DoCash(tree);
	}
    }
    
    annMaxPtsVisit(max_pts_visit);
    matches->clear();
    // Now do the search 
    for(int i = 0; i < featv1.size(); i++){
        ANNidx nn_idx[2];
        ANNdist dist[2];

        tree->annkPriSearch(featv1[i].descr, 2, nn_idx, dist, 0.0);

        if (((double)dist[0]) < ratio*ratio*((double)dist[1])) {
	    matches->push_back(mthd::pair(_PT(featv1[i]), _PT(featv2[nn_idx[0]])));
        }
    }
    if(!IsCashUsed()){
	annDeallocPts(pts);
	delete tree;
    }
}

void mthd::ForceMatch::MatchKeys(std::vector<pair>* matches, std::vector< util::feature >& featv1,
						     std::vector< util::feature >& featv2,  
						     int world, float percent, double ratio)
{
    mthd::RangeWindow rangw;
    rangw.InitRange(world, percent);
    
    mthd::FeaturePlane* fplane;
    if(!IsCashUsed()){
	fplane = new FeaturePlane();
	fplane->Create(featv2);
    }
    else{
	fplane = GetCash();
	if(!fplane){
	    fplane = new FeaturePlane();
	    fplane->Create(featv2);
	    
	    DoCash(fplane);
	}
    }
    
    matches->clear();
    for(int i = 0; i < featv1.size(); i++){
	std::vector<const util::feature*> tmpfv;
	fplane->GetRange(&tmpfv, _PT(featv1[i]), rangw);
// 	
	int min_no_ = -1, submin_no_ = -1;
	float mindis = INT_MAX, submindis = INT_MAX;
	for(int j = 0; j < tmpfv.size(); j++){
	    float cdis;
	    //it is a test; try to use the information of orientation
	    if(rangw.IsOriInRange(featv1[i].kpt, (tmpfv[j])->kpt)){
		cdis = util::SquareDistance(featv1[i], *tmpfv[j]);
		if(cdis < mindis){
		    submindis = mindis; submin_no_ = min_no_;
		    mindis = cdis; min_no_ = j; 
		}
		else if(cdis < submindis){
		    submindis = cdis; submin_no_ = j;
		}
	    }
	}
	
	if(min_no_ != -1 && submin_no_ != -1){
	    if(mindis < ratio*ratio*submindis/*&& mindis < featv1[0].d*featv1[0].d*/){
		matches->push_back(mthd::pair(_PT(featv1[i]), _PT(*tmpfv[min_no_])));
	    }
	}
	else if(min_no_ != -1 && submindis == INT_MAX){
	    const static float threshold = featv1[0].d*featv1[0].d*.5f;
	    if(mindis < threshold){
		matches->push_back(mthd::pair(_PT(featv1[i]), _PT(*tmpfv[min_no_])));
	    }
	}
    }
    
    if(!IsCashUsed()){
	delete fplane;
    }
}
