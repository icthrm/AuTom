#include "function.h"
#include <stdlib.h>
#include<memory.h>
#include<time.h>


#ifndef TEXT_LINE_MAX
#define TEXT_LINE_MAX 500
#endif

#ifndef Tilt_MAX
#define Tilt_MAX 200
#endif

#ifndef TRUE
#define TRUE  0
#endif
#ifndef FALSE
#define FALSE 1
#endif
void random_angle(float *tilt, int prjnum, int *numsequence) {
	srand((int) time(0));
	float *tilt_angle_temp = (float *) malloc(sizeof(float) * prjnum);

	memset(tilt_angle_temp, 0, sizeof(float) * prjnum);

	for (int i = 0; i < prjnum; i++)
		tilt_angle_temp[i] = i;
	int countnum = prjnum;
	for (int i = 0; i < prjnum; i++) {
		int res = rand() % countnum;
		numsequence[i] = tilt_angle_temp[res];
		for (int j = res; j < countnum - 1; j++)
			tilt_angle_temp[j] = tilt_angle_temp[j + 1];
		countnum = countnum - 1;
	}
	memset(tilt_angle_temp, 0, sizeof(float) * prjnum);
	for (int i = 0; i < prjnum; i++) {
		tilt_angle_temp[i] = tilt[numsequence[i]];
	}
	for (int i = 0; i < prjnum; i++) {
		tilt[i] = tilt_angle_temp[i];
	}
	free(tilt_angle_temp);

}
int read_coef(float *data,  FILE *f_coef, int id)
{
    char *lstr;
    lstr=(char *)malloc(TEXT_LINE_MAX);

    int i=0;
    while(fgets(lstr,TEXT_LINE_MAX,f_coef) != NULL)
    {
        data[i]=atof(lstr);

        i++;
    }
    free(lstr);
        return TRUE;
}

