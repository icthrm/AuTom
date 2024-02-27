#ifndef datatype_H__
#define datatype_H__


/** holds feature data relevant to detection */
struct detection_data
{
  int r;
  int c;
  int octv;
  int intvl;
  double subintvl;
  double scl_octv;
};


#include "cxcore.hpp"

/** max feature descriptor length */
#define FEATURE_MAX_D 128

struct __feature
{
  double x;                      /**< x coord */
  double y;                      /**< y coord */
  double scl;                    /**< scale of a Lowe-style feature */
  double ori;                    /**< orientation of a Lowe-style feature */
  int d;                         /**< descriptor length */
  double descr[FEATURE_MAX_D];   /**< descriptor */
  void* feature_data;            /**< user-definable data */
};


namespace util{

    
}

#endif