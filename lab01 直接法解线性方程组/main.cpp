#include<stdio.h>
#include<stdlib.h>
#include<time.h>

clock_t start, stop;  //clock_tΪclock()�������صı�������
double duration;
//ϵ������������� 
const int COL = 5;
const int ROW = 5;
//��Բ����ĵ����꼯 
double axes[][2] = {{-2.312, 0.5027},
					{2.061, -1.257},
					{-0.8545, -2.915},
					{-3.519, -1.156},
					{2.815, -2.815}
};

//����ϵ������ 
double** matCreate(double mat[][2]);
//��ӡϵ������ 
void PrintMatA(double** mat); 
//��ӡ����B 
void PrintMatB(double* mat);
//��ӡ�����Ǿ��� 
void PrintMatUP(double** mat); 
//��������B 
double* makeB();
//�������Ǿ��� 
double** Upper(double** mat, double *matB);
//������Է����飬������X 
double* makeX(double** mat); 
//��ӡ����X 
void PrintMatX(double* mat);
// �ͷŶ�ά������ڴ�
void freeMatrix(double** mat); 
// �ͷ�һά�������ڴ�
void freeVector(double* vec);

int main() {
	start = clock();
	double duration;

	double** MatA; 			//ϵ������A 
	double** Calc_MatA;		//���ڼ����ϵ������A 
	double** UP; 			//�����Ǿ���UP 
	double** Calc_UP;		//���ڼ���������Ǿ��� 
	double* MatB; 			//����B 
	double* MatX; 			//����X 
	//��ʼ����
	MatA = matCreate(axes); 
	Calc_MatA = matCreate(axes); 
	MatB = makeB();
	UP = Upper(Calc_MatA, MatB); //�������Ǿ��� 
	Calc_UP = Upper(Calc_MatA, MatB);
	MatX = makeX(Calc_UP); //�����X 
	//��ɼ��㣬��ʼ��ӡ
	PrintMatA(MatA);
	PrintMatB(MatB);
	PrintMatUP(UP);
	PrintMatX(MatX); 
//	�ͷ��ڴ� 
	freeMatrix(MatA);
	freeMatrix(Calc_MatA);
	freeMatrix(UP);
	freeMatrix(Calc_UP);
	freeVector(MatB);
	freeVector(MatX);
	
	stop = clock();
    duration = (double)(stop - start) / CLK_TCK;     //CLK_TCKΪclock()������ʱ�䵥λ����ʱ�Ӵ��
  	printf("�ⷽ������ʱ�䣺%lf \n", duration);
  	system("pause"); 

	return 0;
}

double** matCreate(double mat[][2]) {
	int i;
	double **ret;
//	ʹ�ö�̬�ڴ洴��һ��5x5�ľ��� 
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
	printf("������ =\n");
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
	int rows, rows1, cols;	//rows��ʾ��ǰ�У�rows1��ʾ��ǰ�е���һ�У�����Ҫ������Ԫ���У�cols��ʾ��Ԫ���е��е����� 
	double u_coefficient;	//��Ԫϵ�� 
	//����һ��6x6�ľ��󣬷�ֹ�����С��ֵ����
	double **ret;
	ret = (double**)malloc((ROW + 1) * sizeof(double*));
	for(i = 0; i < 5; i++) {
		ret[i] = (double*)malloc((COL + 1) * sizeof(double));
	}
//	��ʼ��Ԫ 
	for(rows = 0; rows < ROW - 1; rows++) {				//�ӵ�0�п�ʼ��������������2�� 
		if(rows == 0) {									//����ǵ�0�У���ԭ�����0��Ԫ�ظ�ֵ��ret�ĵ�0��Ԫ�� 
			for(i = 0; i < COL; i++){
				ret[rows][i] = mat[rows][i];
			}
			ret[rows][COL] = matB[rows];				//�ڵ�5�аѾ���B��Ԫ�ظ�ֵ��ret�ĵ�5��Ԫ�� 
		}
		for(rows1 = rows + 1; rows1 < ROW; rows1++) {				//rows1��rows����һ�п�ʼ���� 
			u_coefficient = mat[rows1][rows] / mat[rows][rows];		//������Ԫϵ�� 
			for(cols = rows; cols < COL; cols++) {					//cols��rows����Ԫλ�ÿ�ʼ���� 
				if (rows == cols) {
					ret[rows1][cols] = 0.0;
					mat[rows1][cols] = ret[rows1][cols];
				}
				else {
					ret[rows1][cols] = mat[rows1][cols] - u_coefficient * mat[rows][cols];
					mat[rows1][cols] = ret[rows1][cols];				//����mat 
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
//	����һ��һά���� 
	double* ret = (double*)malloc(5 * sizeof(double));
//	�������Ͻ��лش� 
	for(i = ROW - 1; i >= 0; i--) {
		if(i == ROW - 1) {
			ret[i] = mat[i][ROW] / mat[i][ROW - 1]; 
		}
		else {
			temp = 0;
			for(j = i + 1; j < COL; j++) {
				temp += mat[i][j] * ret[j];					//�Ե�i+1����COL������ʽx��� 
			}
			ret[i] = (mat[i][COL] - temp) / mat[i][i];		//���x 
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
        free(mat[i]); // �ͷ�ÿһ��
    }
    free(mat); // �ͷ�ָ�����鱾��
}

void freeVector(double* vec) {
    free(vec);
}

