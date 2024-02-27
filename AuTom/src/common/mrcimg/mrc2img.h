#ifndef MRC2IMG_H__
#define MRC2IMG_H__

#include "mrcheader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cxcore.hpp>
#include <cv.hpp>


namespace util{
    
/** @brief MrcStack; only for MRC of float format. */
class MrcStack{
private:
    MRCheader* header;
    ExtendedHeader* extendedHeader;
    std::string name;
    std::ifstream in;
	std::ofstream out;
	IplImage** slices;
	bool is_cashed;

private:
	IplImage* ReadIplImage(size_t index);
	
private:
	void FixTitlePadding(char *label);
	
    bool AnalysLabel(const std::string& label , std::string& key , std::string&  value );
	
public:
	enum Mode{MODE_BYTE = 0, MODE_SHORT = 1, MODE_FLOAT = 2};
	
public:
    float inplane_rotation;//////////
	
public:
    MrcStack():header(NULL), extendedHeader(NULL), slices(NULL), is_cashed(false){}
    
    ~MrcStack(){
        if(header){
            delete header;            
        }
        
        if(extendedHeader){
            delete extendedHeader;
        }
    }
    
    bool Open(const char* filename);
    
    void Close(){FreeCache(); if(header){in.close(); out.close(); delete header; header = NULL;}}
    
    IplImage* GetIplImage(int index);
	
	void DoCaching();
	
	void FreeCache();
	
	IplImage* const& operator [](int idx);
	
	IplImage* const& operator [] (int idx) const;
    
    int Size() const{return header->nz;}
    
    int Width() const{return header->nx;}
    
    int Height() const{return header->ny;}
    
    const MRCheader& Header() const{return *header;}
    
    const char* Name() const{return name.c_str();}
    
    void PrintHeader(std::ostream& o = std::cout) const;
	
    void PrintExtendedHeader(std::ostream& o = std::cout) const;
    
	void CopyToNewStack(MrcStack& nmrcs) const;
	
	const MrcStack& operator = (const MrcStack& _mrcs);
	
	void SetHeader(Mode mode, float amin, float amean, float amax);
	
	void SetName(const char* __name);
	
	void WriteHeaderToFile(const char* __name = NULL);
	
	void AppendIplImageToFile(const IplImage* img);
};

}

#endif
