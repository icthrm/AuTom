#ifndef SUPPORT_H__
#define SUPPORT_H__

#include <cmath>
#include <sys/stat.h>
#include "dataf/keypoint.h"
#include <vector>

namespace mthd{
    /** @brief the struct and class defined following will be used in the process of keymatch*/
class RangeWindow{			//it is a pre-model for the matching of keys
private:
    int rad;
    float percent;
    int world;
    constexpr static float  ratio = 0.55f;
    constexpr static const float rot_limit = M_PI/6;
public:
    RangeWindow():rad(0){}
    ~RangeWindow(){}
    
    inline bool IsInRange(const util::keypt_t& origin, const util::keypt_t& range) const
    {
	return fabs(range.x-origin.x) < world/4 && fabs(range.y-origin.y) < world/4;
    }
    
    inline bool IsOriInRange(const util::keypt_t& origin, const util::keypt_t& range) const
    {
	return fabs(origin.orient-range.orient) <= rot_limit;
    }
    
    inline void InitRange(int _world = 512, float _percent = 0.1f)
    {
	world = _world;
	percent = _percent;
	rad = world*percent*ratio;
    }
    
    inline int Radius() const {return rad;}
    
    inline float RotationLimit() const{return rot_limit;}
};

/** @brief a very important class for forcematch; a two dimenstion struct*/
class FeaturePlane{
private:
    typedef struct _lnode{
	int x;
	int size;
	util::feature* ptr;
    }lnode;
    
    friend class RangeWindow;
    
private:
    std::vector<lnode> xlist;
    int z;
private:
    static bool FeatureSmaller(const util::feature& feat1, const util::feature& feat2)
    {
	return (feat1.kpt.x < feat2.kpt.x)||(feat1.kpt.x == feat2.kpt.x && feat1.kpt.y < feat2.kpt.y);
    }
    
public:
    FeaturePlane(int _z = 0):z(_z){}
    
    ~FeaturePlane()
    {
	for(int i = 0; i < xlist.size(); i++){
	    delete [] xlist[i].ptr;
	}
	xlist.clear();
    }
    
    void Create(std::vector<util::feature>& featv);
    
    /** @brief this function gets the features which is in range of referred rangewindow*/
    void GetRange(std::vector<const util::feature*>* featv, const util::_point& location, const RangeWindow& range) const;
    
    int Print(std::ostream& o) const;
    inline int Z() const{return z;}
    inline void SetZ(int z_){z = z_;}
};


    
}

#endif
