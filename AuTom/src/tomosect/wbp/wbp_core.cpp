#include "wbp_core.h"
#include "mrcimg/mrcheader.h"
#include "wbp.h"
#include <string.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>

#ifndef     PI_180
#define     PI_180          0.01745329252
#endif


int WbpRecSliceXZ(const char* inputmrc, const char* anglefile, const char* outputmrc, int thickness, float y_fractile)
{
    assert(y_fractile > 0 && y_fractile < 1);
    
    Volume vol;
    memset(&vol, 0, sizeof(Volume));
    
    std::vector<float> angles;
    std::ifstream in(anglefile);
    if(!in.good()){
	return false;
    }
    
    //read tilt angles
    while(in.good()){
	float angle;
	in>>angle;
	if(in.fail()){
	    break;
	}
	angles.push_back(angle*PI_180+vol.offset);
    }
    
    in.close();
    
    MrcHeader  *inhead, *outhead;

    inhead = new MrcHeader();
    outhead = new MrcHeader();
    
    MrcReadHead(inhead, inputmrc);
    
    vol.nx = inhead->nx;
    vol.ny = inhead->ny;
    vol.nz = thickness;

    if (vol.pitch >= 0){
        vol.extnz =(int)(vol.nz + tan(vol.pitch)*vol.ny);
    }
    else{
        vol.extnz =(int)(vol.nz - tan(vol.pitch)*vol.ny);
    }
    
    vol.x0 = (float)vol.nx/2.0;
    vol.y0 = (float)vol.ny/2.0;
    vol.z0 = (float)vol.nz/2.0+vol.zshift;
    
    Slice slc;
    memset(&slc, 0, sizeof(Slice));

    slc.nx = vol.nx;
    slc.nz = vol.nz;
    slc.x0 = vol.x0;
    slc.z0 = vol.z0;
    slc.zstart = 0;
    slc.zend = slc.nz-1;

    slc.data = new float[slc.nx*slc.nz];
    
    MrcInitHead(outhead);
    outhead->mode = MRC_MODE_FLOAT;
    outhead->nx = vol.nx;
    outhead->nz = 1;
    outhead->ny = vol.nz;
    outhead->nzstart = 0;
    
    int proj_width = vol.nx;
    float* all_proj_gray = new float[angles.size()*proj_width+1];
    memset(all_proj_gray, 0, sizeof(float)*(angles.size()*proj_width+1));
    int* all_pixc = new int[angles.size()*proj_width];
        
    int **all_light_path;
    
    all_light_path = new int*[angles.size()*proj_width];
    int ystart = (int)(y_fractile*vol.ny);
    slc.z0 = vol.extnz/2.0 + vol.zshift-(ystart-0)*tan(vol.pitch);
    memset(slc.data, 0, sizeof(float)*slc.nx*slc.nz);
    
    DecideAllLightPath(all_light_path, all_pixc, angles, proj_width, slc);
    MrcReadSlice(inputmrc, ystart, 'y', all_proj_gray);
    
    BackProjectAllLight(all_proj_gray, angles.size(), proj_width, all_light_path, all_pixc, &slc);
    
    MrcUpdateHead(outhead, (void*)slc.data);
    MrcSave(*outhead, (void*)slc.data, outputmrc);
    
    delete inhead;
    delete outhead;
    delete [] slc.data;
    
    delete [] all_proj_gray;
    delete [] all_pixc;
    
    for(int i = 0; i < angles.size()*proj_width; i++){
	if(all_light_path[i] != NULL){
	    delete [] all_light_path[i];
	}
    }
    
    return 1;
}
