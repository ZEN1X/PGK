#ifndef PREPAREDATA_H
#define PREPAREDATA_H

extern float FunctionData[100][3];
extern int NoPoints;

void PrepareData(int fun);

double Shepard(double x, double y, float data[100][3], int data_num);

#endif /* PREPAREDATA_H */
