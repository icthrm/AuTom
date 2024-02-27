#include <iostream>
#include<fstream>
#include<string.h>
#include <stdlib.h>
#include<iomanip>
using namespace std;

void getxf1(char *buf);
void getxf2(char *buff);
/***.xf参数***/
float A11,A12,A21,A22,SHIFT1_X,SHIFT1_Y, B11,B12,B21,B22,SHIFT2_X,SHIFT2_Y,C11,C12,C21,C22,SHIFT3_X,SHIFT3_Y,DX3,DY3;
/*
 X'' =(A'11*A11+A'12*A21) X+ (A'11*A12+A'12*A22) Y+ A'11*DX+A'12*DY+DX'   
 Y'' =(A'21*A11+A'22*A21) X+ (A'21*A12+A'22*A22)Y+A'21*DX+ A'22 *DY+DY'
 */

int main(int argc, char **argv) {

	string 	filename1=argv[1];
	string      filename2=argv[2];
	string 	filename3=argv[3];
 
	ifstream infile1(filename1.c_str());
	ifstream infile2(filename2.c_str());
	ofstream oufile(filename3.c_str());
	if(!oufile)  cout<<"error !";
  
 	string tmp1, tmp2;
	
      while(getline(infile1,tmp1)&&getline(infile2,tmp2))  
    {  
      //  cout<<tmp1<<endl;
	//cout<<tmp2<<endl;
	getxf1((char*)tmp1.c_str());
	getxf2((char*)tmp2.c_str());
	C11=B11*A11+B12*A21 ;
	C12=B11*A12+B12*A22 ;
	C21=B21*A11+B22*A21;
	C22=B21*A12+B22*A22;
	//cout<<"A11="<<A11<<" A12="<<A12<<" A21="<<A21<<" A22="<<A22<<" SHIFT1_X="<<SHIFT1_X<<" SHIFT1_Y="<<SHIFT1_Y<<endl;
 	//cout<<fixed<<setprecision(8)<<"B11="<<B11<<" B12="<<B12<<" B21="<<B21<<" B22="<<B22<<" SHIFT2_X="<<SHIFT2_X<<" SHIFT2_Y="<<SHIFT2_Y<<endl;

	SHIFT3_X=B11*SHIFT1_X+ B12*SHIFT1_Y+SHIFT2_X;
        SHIFT3_Y=B21*SHIFT1_X+ B22 *SHIFT1_Y+SHIFT2_Y;
	
	
	oufile<<fixed<<setprecision(7)<<setw(12)<<C11<<setw(12)<<C12<<setw(12)<<C21<<setw(12)<<C22<<fixed<<setprecision(3)<<setw(12)<<SHIFT3_X<<setw(12)<<SHIFT3_Y<<endl;//12位右对齐	
    }
     infile1.close();
     infile2.close();
     oufile.close();
    return 0;
}

void getxf1(char *buf)
{
	char *p = strtok(buf, "\t");		
	A11=strtof(p,NULL);                 

	p = strtok(NULL, "\t");	 
	A12=strtof(p,NULL);
	
	p = strtok(NULL, "\t");
	A21=strtof(p,NULL);
	
	p = strtok(NULL, "\t");
	A22=strtof(p,NULL);
	
	p = strtok(NULL, "\t");
	SHIFT1_X=strtof(p,NULL);
	
	p = strtok(NULL, "\t");
	SHIFT1_Y=strtof(p,NULL);
        return;
}
void getxf2(char *buff)
{
 	char *pp = strtok(buff, "\t");      
 	B11=strtof(pp,NULL);                                     

 	pp = strtok(NULL, "\t");	  
  	B12=strtof(pp,NULL);  

 	pp = strtok(NULL, "\t");	 
 	B21=strtof(pp,NULL);
	 
 	pp = strtok(NULL, "\t");	 
 	B22=strtof(pp,NULL);
	 
  	pp = strtok(NULL, "\t");	 
  	SHIFT2_X=strtof(pp,NULL);
	 
	pp = strtok(NULL, "\t");	 
	SHIFT2_Y=strtof(pp,NULL);
        return;
}
