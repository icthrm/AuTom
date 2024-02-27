#include "support.h"
#include <algorithm>
#include <iostream>

void mthd::FeaturePlane::Create(std::vector< util::feature >& featv)
{
    std::sort(featv.begin(), featv.end(), FeatureSmaller);
    std::vector<util::feature>::const_iterator ibegin, iend;
    
    ibegin = featv.begin();
    iend = featv.begin();
    while(ibegin != featv.end()){
	int count = 0;
	while(iend != featv.end() && (*iend).kpt.x == (*ibegin).kpt.x){
	    count++;
	    iend++;
	}
	lnode nde;  nde.x = (*ibegin).kpt.x; nde.size = count;
	nde.ptr = new util::feature[count];
	
	util::feature* dest = nde.ptr;
	std::copy(ibegin, iend, dest);
	xlist.push_back(nde);
	
	ibegin = iend;
    }
}

void mthd::FeaturePlane::GetRange(std::vector< const util::feature*>* featv, const util::_point& location, const RangeWindow& range) const
{
    featv->clear();
    
    int xmin = location.x-range.Radius();
    int xmax = location.x+range.Radius();
    
    int ymin = location.y-range.Radius();
    int ymax = location.y+range.Radius();
    
    int xlmid = location.x+20<xlist.size()?location.x+20:xlist.size()/2;//xlist.size()/2;
    int xlbegin = 0, xlend = xlist.size()-1;
    while(xlbegin <= xlend){
	if(xlist[xlmid].x < xmin){
	    xlbegin = xlmid+1;
	    xlmid = (xlbegin+xlend)/2;
	}
	else if(xlist[xlmid].x > xmax){
	    xlend = xlmid-1;
	    xlmid = (xlbegin+xlend)/2;
	}
	else{
	    for(int i = xlmid; i >= 0&&xlist[i].x >= xmin; i--){
		int ylmid = xlist[i].size/2;
		int ylbegin = 0, ylend = xlist[i].size-1;
		while(ylbegin <= ylend){
		    if((xlist[i].ptr)[ylmid].kpt.y < ymin){
			ylbegin = ylmid+1;
			ylmid = (ylbegin+ylend)/2;
		    }
		    else if((xlist[i].ptr)[ylmid].kpt.y > ymax){
			ylend = ylmid-1;
			ylmid = (ylbegin+ylend)/2;
		    }
		    else{
			for(int j = ylmid; j >= 0&&(xlist[i].ptr)[j].kpt.y >= ymin; j--){
			    featv->push_back(&((xlist[i].ptr)[j]));
			}

			for(int j = ylmid+1; j < xlist[i].size&&(xlist[i].ptr)[j].kpt.y <= ymax; j++){
			    featv->push_back(&((xlist[i].ptr)[j]));
			}
			break;
		    }
		}
	    }
	    for(int i = xlmid+1; i < xlist.size()&&xlist[i].x <= xmax; i++){
		int ylmid = xlist[i].size/2;
		int ylbegin = 0, ylend = xlist[i].size-1;
		while(ylbegin <= ylend){
		    if((xlist[i].ptr)[ylmid].kpt.y < ymin){
			ylbegin = ylmid+1;
			ylmid = (ylbegin+ylend)/2;
		    }
		    else if((xlist[i].ptr)[ylmid].kpt.y > ymax){
			ylend = ylmid-1;
			ylmid = (ylbegin+ylend)/2;
		    }
		    else{
			for(int j = ylmid; j >= 0&&(xlist[i].ptr)[j].kpt.y >= ymin; j--){
			    featv->push_back(&((xlist[i].ptr)[j]));
			}

			for(int j = ylmid+1; j < xlist[i].size&&(xlist[i].ptr)[j].kpt.y <= ymax; j++){
			    featv->push_back(&((xlist[i].ptr)[j]));
			}
			break;
		    }
		}
	    }
	    break;
	}
    }
    return;
}

int mthd::FeaturePlane::Print(std::ostream& o) const
{
    int count = 0;
    for(int i = 0; i < xlist.size(); i++){
	o<<"["<<i<<"]("<<xlist[i].size<<")#"<<xlist[i].x<<":";
	count += xlist[i].size;
	for(int j = 0; j < xlist[i].size; j++){
	    assert((xlist[i].ptr)[j].kpt.x == xlist[i].x);
	    o<<((xlist[i].ptr)[j]).kpt.y<<" ";
	}
	o<<std::endl;
    }
    o<<std::endl;
    return count;
}

