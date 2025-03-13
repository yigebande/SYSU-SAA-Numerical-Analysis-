#include<stdio.h>
#include<stdlib.h>
#include<time.h>

clock_t start, stop;  //clock_t为clock()函数返回的变量类型
double duration;
//系数矩阵的行列数 
const int COL = 5;
const int ROW = 5;
//椭圆轨道的点坐标集 
double axes[][2] = {{-2.312, 0.5027},
					{2.061, -1.257},
					{-0.8545, -2.915},
					{-3.519, -1.156},
					{2.815, -2.815}
};

//创建系数矩阵 
double** matCreate(double mat[][2]);
//打印系数矩阵 
void PrintMatA(double** mat); 
//打印矩阵B 
void PrintMatB(double* mat);
//打印上三角矩阵 
void PrintMatUP(double** mat); 
//创建矩阵B 
double* makeB();
//求上三角矩阵 
double** Upper(double** mat, double *matB);
//求解线性方程组，即矩阵X 
double* makeX(double** mat); 
//打印矩阵X 
void PrintMatX(double* mat);
// 释放二维矩阵的内存
void freeMatrix(double** mat); 
// 释放一维向量的内存
void freeVector(double* vec);

int main() {
	start = clock();
	double duration;

	double** MatA; 			//系数矩阵A 
	double** Calc_MatA;		//用于计算的系数矩阵A 
	double** UP; 			//上三角矩阵UP 
	double** Calc_UP;		//用于计算的上三角矩阵 
	double* MatB; 			//矩阵B 
	double* MatX; 			//矩阵X 
	//开始计算
	MatA = matCreate(axes); 
	Calc_MatA = matCreate(axes); 
	MatB = makeB();
	UP = Upper(Calc_MatA, MatB); //求上三角矩阵 
	Calc_UP = Upper(Calc_MatA, MatB);
	MatX = makeX(Calc_UP); //求矩阵X 
	//完成计算，开始打印
	PrintMatA(MatA);
	PrintMatB(MatB);
	PrintMatUP(UP);
	PrintMatX(MatX); 
//	释放内存 
	freeMatrix(MatA);
	freeMatrix(Calc_MatA);
	freeMatrix(UP);
	freeMatrix(Calc_UP);
	freeVector(MatB);
	freeVector(MatX);
	
	stop = clock();
    duration = (double)(stop - start) / CLK_TCK;     //CLK_TCK为clock()函数的时间单位，即时钟打点
  	printf("解方程所用时间：%lf \n", duration);
  	system("pause"); 

	return 0;
}

double** matCreate(double mat[][2]) {
	int i;
	double **ret;
//	使用动态内存创建一个5x5的矩阵 
	ret = (double**)malloc(ROW * sizeof(double*));
	for(i = 0; i < 5; i++) {
		ret[i] = (double*)malloc(COL * sizeof(double));
	}
	for(i = 0; i < 5; i++) {
		ret[i][0] = mat[i][0] * mat[i][1]; 		// xy 
		ret[i][1] = mat[i][1] * mat[i][1]; 		// y^2
		ret[i][2] = mat[i][0]; 					// x
		ret[i][3] = mat[i][1]; 					// y
		ret[i][4] = 1.0; 						// 1
	}
	return ret;
}

void PrintMatA(double** mat) {
	int i, j;
	printf("A =\n");
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 5; j++) {
			printf("%10.6lf", mat[i][j]);
		}
		printf("\n");
	}
	printf("-----------------------------------------------------------------\n");
}

void PrintMatB(double* mat) {
	int i;
	printf("B =\n");
	for(i = 0; i < 5; i++){
		printf("%10.6lf\n", mat[i]);
	}
	printf("-----------------------------------------------------------------\n");
}

void PrintMatUP(double** mat) {
	int i, j;
	printf("上三角 =\n");
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 6; j++) {
			printf("%11.6lf", mat[i][j]);
		}
		printf("\n");
	}
	printf("-----------------------------------------------------------------\n");
}

double* makeB(){
	int i;
	double* ret = (double*)malloc(5 * sizeof(double));
	for(i = 0; i < 5; i++) {
		ret[i] = -axes[i][0] * axes[i][0]; // -x^2
	}
	return ret;
}

double** Upper(double** mat, double* matB) {
	int i;
	int rows, rows1, cols;	//rows表示当前行，rows1表示当前列的下一行，即需要进行消元的行，cols表示消元进行的列的序数 
	double u_coefficient;	//消元系数 
	//创建一个6x6的矩阵，防止数组过小赋值出错
	double **ret;
	ret = (double**)malloc((ROW + 1) * sizeof(double*));
	for(i = 0; i < 5; i++) {
		ret[i] = (double*)malloc((COL + 1) * sizeof(double));
	}
//	开始消元 
	for(rows = 0; rows < ROW - 1; rows++) {				//从第0行开始，遍历到倒数第2行 
		if(rows == 0) {									//如果是第0行，则将原矩阵第0行元素赋值给ret的第0行元素 
			for(i = 0; i < COL; i++){
				ret[rows][i] = mat[rows][i];
			}
			ret[rows][COL] = matB[rows];				//在第5行把矩阵B的元素赋值给ret的第5行元素 
		}
		for(rows1 = rows + 1; rows1 < ROW; rows1++) {				//rows1从rows的下一行开始遍历 
			u_coefficient = mat[rows1][rows] / mat[rows][rows];		//更新消元系数 
			for(cols = rows; cols < COL; cols++) {					//cols从rows的主元位置开始遍历 
				if (rows == cols) {
					ret[rows1][cols] = 0.0;
					mat[rows1][cols] = ret[rows1][cols];
				}
				else {
					ret[rows1][cols] = mat[rows1][cols] - u_coefficient * mat[rows][cols];
					mat[rows1][cols] = ret[rows1][cols];				//更新mat 
				}
			}
			ret[rows1][cols] = matB[rows1] - u_coefficient * matB[rows]; 
			matB[rows1] = ret[rows1][cols];
		}
	}
	return ret;
}

double* makeX(double** mat) {
	int i, j;
	double temp;
//	创建一个一维数组 
	double* ret = (double*)malloc(5 * sizeof(double));
//	从下往上进行回代 
	for(i = ROW - 1; i >= 0; i--) {
		if(i == ROW - 1) {
			ret[i] = mat[i][ROW] / mat[i][ROW - 1]; 
		}
		else {
			temp = 0;
			for(j = i + 1; j < COL; j++) {
				temp += mat[i][j] * ret[j];					//对第i+1到第COL个多项式x求和 
			}
			ret[i] = (mat[i][COL] - temp) / mat[i][i];		//求出x 
		}
	}
	return ret;
}

void PrintMatX(double* mat) {
	int i;
	printf("X =\n");
	for(i = 0; i < 5; i++){
		printf("%10.6lf\n", mat[i]);
	}
	printf("-----------------------------------------------------------------\n");
}

void freeMatrix(double** mat) {
	int i;
    for (i = 0; i < 5; i++) {
        free(mat[i]); // 释放每一行
    }
    free(mat); // 释放指针数组本身
}

void freeVector(double* vec) {
    free(vec);
}

