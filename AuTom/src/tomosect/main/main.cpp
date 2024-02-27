#include <iostream>
#include "opts.h"
#include "wbp/wbp_core.h"

using namespace std;


int main(int argc, char **argv) {
//     EnvironmentSet();
    opts param;
    if(GetOpts(argc, argv, &param) != 1){
	return 0;
    }
    
    WbpRecSliceXZ(param.input, param.inputangle, param.output, param.thickness, param.y);
//     BaseLineHaving(param.output, NULL);
    
    return 0;
}
