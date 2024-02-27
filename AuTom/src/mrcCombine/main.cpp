#include "mrcimg/mrcheader.h"
#include <fstream>
#include "opts.h"
#include <cstdlib>
#include<iostream>
#include<cstring>
int getImageSize(MrcHeader& head){
    int psize=0;
    int imageSize=0;
    switch(head.mode){
    case MRC_MODE_BYTE :
        psize=sizeof(unsigned char);
        break;
    case MRC_MODE_SHORT :
        psize=sizeof(short);
        break;
    case MRC_MODE_FLOAT :
        psize=sizeof(float);
        break;
    }
    imageSize=head.nx*head.ny*psize;
    return imageSize;
}


void PrintHeader(std::ostream& o,MrcHeader* header)  {
    o<<"\n nx: "<<header->nx;         /*  # of Columns                  */
    o<<"\n ny: "<<header->ny;         /*  # of Rows                     */
    o<<"\n nz: "<<header->nz;         /*  # of Sections.                */
    o<<"\n mode: "<<header->mode;       /*  given by #define MRC_MODE...  */

    o<<"\n nxstart: "<<header->nxstart;    /*  Starting point of sub image.  */
    o<<"\n nystart: "<<header->nystart;
    o<<"\n nzstart: "<<header->nzstart;

    o<<"\n mx: "<<header->mx;         /* Grid size in x, y, and z       */
    o<<"\n my: "<<header->my;
    o<<"\n mz: "<<header->mz;

    o<<"\n xlen: "<<header->xlen;       /* length of x element in um.     */
    o<<"\n ylen: "<<header->ylen;       /* get scale = xlen/nx ...        */
    o<<"\n zlen: "<<header->zlen;

    o<<"\n alpha: "<<header->alpha;      /* cell angles, ignore */
    o<<"\n beta: "<<header->beta;
    o<<"\n gamma: "<<header->gamma;

    o<<"\n mapc: "<<header->mapc;       /* map coloumn 1=x,2=y,3=z.       */
    o<<"\n mapr: "<<header->mapr;       /* map row     1=x,2=y,3=z.       */
    o<<"\n maps: "<<header->maps;       /* map section 1=x,2=y,3=z.       */

    o<<"\n amin: "<<header->amin;
    o<<"\n amax: "<<header->amax;
    o<<"\n amean: "<<header->amean;

    o<<"\n ispg: "<<header->ispg;       /* image type */
    o<<"\n nsymbt: "<<header->nsymbt;     /* space group number */


    /* 64 bytes */

    o<<"\n next: "<<header->next;
    o<<"\n sizeof header: "<<sizeof(MRCheader);
    o<<"\n creatid: "<<header->creatid;  /* Creator id, hvem = 1000, DeltaVision = -16224 */


    o<<"\n blank: "<<header->blank;

    o<<"\n nint: "<<header->nint;
    o<<"\n nreal: "<<header->nreal;
    o<<"\n sub: "<<header->sub;
    o<<"\n zfac: "<<header->zfac;

    o<<"\n min2: "<<header->min2;
    o<<"\n max2: "<<header->max2;
    o<<"\n min3: "<<header->min3;
    o<<"\n max3: "<<header->max3;
    o<<"\n min4: "<<header->min4;
    o<<"\n max4: "<<header->max4;


    o<<"\n idtype: "<<header->idtype;
    o<<"\n lens: "<<header->lens;
    o<<"\n nd1: "<<header->nd1;     /* Devide by 100 to get o<<header-> value. */
    o<<"\n nd2: "<<header->nd2;
    o<<"\n vd1: "<<header->vd1;
    o<<"\n vd2: "<<header->vd2;
    o<<"\n tiltangles: "<<header->tiltangles[0]<<" "<<header->tiltangles[1]<<" "
     <<header->tiltangles[2]<<" "<<header->tiltangles[3]<<" "<<header->tiltangles[4]<<" "
     <<header->tiltangles[5]<<" ";  /* 0,1,2 = original:  3,4,5 = current */


    o<<"\n xorg: "<<header->xorg;
    o<<"\n yorg: "<<header->yorg;
    o<<"\n zorg: "<<header->zorg;
    o<<"\n cmap: "<<header->cmap[0]<<header->cmap[1]<<header->cmap[2]<<header->cmap[3];
    o<<"\n stamp: "<<header->stamp[0]<<header->stamp[1]<<header->stamp[2]<<header->stamp[3];
    o<<"\n rms: "<<header->rms;

    for(int i = 0; i < header->nlabl; i++) {
        o<<"\n labels["<<i<<"]: "<<header->labels[i];
    }
    o.flush();
}
bool readImages(std::ifstream& in,MrcHeader& head , int slice,char* data){
    int psize=0;
    int imageSize=0;
    switch(head.mode){
    case MRC_MODE_BYTE :
        psize=sizeof(unsigned char);
        break;
    case MRC_MODE_SHORT :
        psize=sizeof(short);
        break;
    case MRC_MODE_FLOAT :
        psize=sizeof(float);
        break;
    }
    imageSize=head.nx*head.ny*psize;
   
	in.seekg(sizeof(MrcHeader)+head.next+slice*head.nx*head.ny*psize,std::ios::beg);    
	
    in.read(data,imageSize);

    return true;
}

int main(int argc ,char** argv){
    options opt;
    MrcHeader inheader;
    MrcHeader addheader;
    MrcHeader outheader;
    
    
    printf("argc is %d\n",argc);
    if(1==GetOpts(argc,argv,&opt)){
        if(!MrcReadHead(&inheader,opt.input.data()) || !MrcReadHead(&addheader,opt.addMrc.data())){
            printf("read file error\n");
            exit(1);
        }
//         PrintHeader(std::cout,&header);
//         PrintHeader(std::cout,&addheader);
        if(inheader.nx!= addheader.nx || inheader.ny!=addheader.ny){
            printf("stack images are not the same size.\n");
            exit(1);
        }
        
        if(inheader.mode !=addheader.mode){
            printf("stack images don't have the same pixel size.\t ");
            exit(1);
        }
        
        memcpy(&outheader,&inheader,sizeof(MrcHeader));
        outheader.nz=inheader.nz+addheader.nz;
        outheader.next=inheader.next+addheader.next;
        PrintHeader(std::cout,&outheader);
        PrintHeader(std::cout,&inheader);

        std::string tempFilename="___tempmrcfile.mrc";
        std::ofstream out;
        out.open(tempFilename.data(), std::ios::out);
        
        std::ifstream in;
        in.open(opt.input.data());
        
        std::ifstream add;
        add.open(opt.addMrc.data());
        
        
        //write head to out file
        out.seekp(std::ios::beg);
        out.write(reinterpret_cast<const char*>(&outheader),sizeof(MrcHeader));
        if(!out.good()){            
            printf(" out not good \t");            
        }
        char extendedHeader[128];
        in.seekg(1024);
        in.read(extendedHeader,128);
        
        for(int i=0;i<outheader.nz;i++){
            out.write(extendedHeader,128);
        }

        
        int imageSize=getImageSize(addheader);
        printf("imagesize is %d",imageSize);
        char* data=new char[imageSize];
        
        for(int i=0;i<inheader.nz;i++){
            printf("copying slice %d from file %s \n",i,opt.input.data());
            readImages(in,inheader,i,data);
            out.write(data,imageSize);

        }
        for(int i=0;i<addheader.nz;i++){
            printf("copying slice %d from file %s \n",i,opt.addMrc.data());
            readImages(add,addheader,i,data);
            out.write(data,imageSize);

        }
        
        
        in.close();
        add.close();
        out.close();
        
        std::string cmd="mv ___tempmrcfile.mrc ";
        cmd.append(opt.input);
        system(cmd.data());
        
    }
    
    
    
    
    
}
