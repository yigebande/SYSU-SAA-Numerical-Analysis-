#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
clock_t start, stop;
double duration;
//椭圆上五个点的x，y坐标
double axes[5][2] = { {-2.312,0.5027},
					  {2.061,-1.257},
					  {-0.8545,-2.915},
					  {-3.519,-1.156},
					  {2.815,-2.815}
};
//创建A矩阵
void createA(double matA[][5], double matB[][2]);
//B
void createB(double mat[], double matB[][2]);
//Doolittle分解
void doolittle(double mat[][5]);

//打印分割线
void barrier();
//打印矩阵A
void printA(double mat[][5]);
void printB(double B[]);
//打印LU
void printLU(double mat[][5]);
void printL(double mat[][5]);
void printU(double mat[][5]);

//L
void getL(double matL[][5], double LU[][5]);
//U
void getU(double matU[][5], double LU[][5]);
//Ly=X, y
void getY(double y[], double L[][5], double B[]);
//Ux=y, x
void getX(double x[], double U[][5], double y[]);

void printY(double y[]);
void printX(double x[]);

void res(double x[]);

int main() {
	start = clock();
	//A系数矩阵
	double matA[5][5] = { 0 };
	//B
	double matB[5] = { 0 };
	//用于计算的系数矩阵A
	double calc_matA[5][5] = { 0 };
	//L,U
	double L[5][5] = { 0 };
	double U[5][5] = { 0 };
	//y,x
	double y[5] = { 0 };
	double x[5] = { 0 };
	createA(matA, axes);
	createA(calc_matA, axes);
	createB(matB, axes);
	doolittle(calc_matA);
	getL(L, calc_matA);
	getU(U, calc_matA);
	getY(y, L, matB);
	getX(x, U, y);
	printA(matA);
	printL(L);
	printU(U);
	stop = clock();
	duration = (double)(stop - start) / CLK_TCK;     //CLK_TCK为clock()函数的时间单位，即时钟打点
	printf("解方程所用时间：%f\n", duration);
	barrier();
	//system("pause");
	printX(x);
	res(x);
	return 0;
}

void createA(double matA[][5], double matB[][2]) {
	int i;
	for (i = 0; i < 5; ++i) {
		matA[i][0] = matB[i][0] * matB[i][1];
		matA[i][1] = matB[i][1] * matB[i][1];
		matA[i][2] = matB[i][0];
		matA[i][3] = matB[i][1];
		matA[i][4] = 1.0;
	}
}

void createB(double mat[], double matB[][2]) {
	int i;
	for (i = 0; i < 5; ++i) {
		mat[i] = -matB[i][0] * matB[i][0];
	}
}

void doolittle(double mat[][5]) {
	int i, j, k, l, m;
	double up = 0.0;
	double low = 0.0;
	//5x5矩阵共处理5次行，5次列，第1行不用处理
	for (i = 1; i < 5; ++i) {
		//第j行，第i-1列
		for (j = i; j < 5; ++j) {
			if (i == 1) {
				mat[j][i - 1] /= mat[0][0];
			}
			else {
				low = 0.0;
				for (l = 0; l < i - 1; ++l) {
					low += mat[j][l] * mat[l][i - 1];
				}
				mat[j][i - 1] = (mat[j][i - 1] - low) / mat[i - 1][i - 1];
			}
		}
		//第i行，第k列
		for (k = i; k < 5; ++k) {
			up = 0.0;
			for (m = 0; m < i; ++m) {
				up += mat[m][k] * mat[i][m];
			}
			mat[i][k] -= up;
		}
	}
}

void getL(double matL[][5], double LU[][5]) {
	int i, j;
	for (i = 0; i < 5; ++i) {
		for (j = 0; j <= i; ++j) {
			if (j == i) {
				matL[i][j] = 1;
			}

			else {
				matL[i][j] = LU[i][j];
			}
		}
	}
}

void getU(double matU[][5], double LU[][5]) {
	int i, j;
	for (i = 0; i < 5; ++i) {
		for (j = i; j < 5; ++j) {
			matU[i][j] = LU[i][j];
		}
	}
}

void getY(double y[], double L[][5], double B[]) {
	int i, j;
	double co = 0;
	for (i = 0; i < 5; ++i) {
		if (i == 0) {
			y[0] = B[0];
		}
		else {
			co = 0;
			for (j = 0; j < i; ++j) {
				co += y[j] * L[i][j];
			}
			y[i] = B[i] - co;
		}
	}
}

void getX(double x[], double U[][5], double y[]) {
	int i, j;
	double co = 0;
	for (i = 4; i >= 0; --i) {
		if (i == 4) {
			x[i] = y[i] / U[i][i];
		}
		else {
			co = 0;
			for (j = 4; j > i; --j) {
				co += x[j] * U[i][j];
			}
			x[i] = (y[i] - co) / U[i][i];
		}
	}
}


void barrier() {
	printf("----------------------------------------------------------\n");
}

void printA(double mat[][5]) {
	int i, j;
	printf("A = \n");
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			printf("%11.6lf", mat[i][j]);
		}
		printf("\n");
	}
	barrier();
}

void printLU(double mat[][5]) {
	int i, j;
	printf("LU = \n");
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			printf("%11.6lf", mat[i][j]);
		}
		printf("\n");
	}
	barrier();
}

void printL(double mat[][5]) {
	int i, j;
	printf("L = \n");
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			printf("%11.6lf", mat[i][j]);
		}
		printf("\n");
	}
	barrier();
}

void printU(double mat[][5]) {
	int i, j;
	printf("U = \n");
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			printf("%11.6lf", mat[i][j]);
		}
		printf("\n");
	}
	barrier();
}

void printY(double y[]) {
	;
}
void printX(double x[]) {
	int i;
	printf("The solution x[]= \n");
	for (i = 0; i < 5; ++i) {
		printf("%11.6lf", x[i]);
	}
	printf("\n");
	barrier();
}

void printB(double B[]) {
	int i;
	printf("B = \n");
	for (i = 0; i < 5; ++i) {
		printf("%11.6lf\n", B[i]);
	}
	barrier();
}

void res(double x[]) {
	printf("求得椭圆方程为：\n");
	printf("x^2+%.6f*xy+%.6lf*y^2+%.6lf*x+%.6lf*y+%.6lf=0\n", x[0], x[1], x[2], x[3], x[4]);
	printf("原椭圆方程为：\n");
	printf("x^2+%d*xy+%d*y^2+%d*x+%d*y+%d=0\n", (int)x[0] + 1, (int)x[1] + 1, (int)x[2] + 1, (int)x[3] + 1, (int)x[4] + 1);
}