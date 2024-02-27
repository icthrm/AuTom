#include <iostream>
#include <climits>
#include <vector>
#include "mrcimg/mrc2img.h"
#include "mrcimg/img_util.h"
#include "dataf/dataf.h"
#include "opts.h"
#include "inpaint/inpaint_core.h"

using namespace std;

int main(int argc, char **argv) {
    opts param;
	param.diameter = -1;
    if(GetOpts(argc, argv, &param) != 1) {
        return 0;
    }

    util::MrcStack mrcs;
    if(!mrcs.Open(param.input)){
		std::cout<<"Cannot open mrc file \""<<param.input<<"\""<<std::endl;
		return -1;
    }
	
	util::FiducialStack fidstack;
	if(!fidstack.ReadFidsByFile(param.fids)){
		std::cout<<"Cannot open fiducial marker location file \""<<param.fids<<"\""<<std::endl;
		return -1;
	}
	
	if(param.diameter < 0){
		IplImage* tmplt;
		util::SeriesReadFromFile(&tmplt, "avgtmplt");
		param.diameter = tmplt->width;
		cvReleaseImage(&tmplt);
	}
	
	if(param.addfids != ""){
		util::FiducialStack addfidstack;
		addfidstack.ReadFidsByFile(param.addfids);
		for(int i = 0; i < fidstack.Size(); i++){
			std::vector<util::point2d>& fids = fidstack.V(i);
			std::vector<util::point2d>& addfids = addfidstack.V(i);
			for(int j = 0; j < addfids.size(); j++){
				fids.push_back(addfids[j]);
			}
		}
	}
	
	InpaintApp::InpaintingMain(mrcs, fidstack, param.diameter, param.output);
	
    mrcs.Close();

    return 0;
}
