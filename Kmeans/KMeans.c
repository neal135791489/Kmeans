#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>

#define NumOfMeans 3
 void get_two_dimension(char* line, double** data, char *filename)
 {   
     FILE* stream = fopen(filename, "r");
     int i = 0;
     while (fgets(line, 1024, stream)){//逐行讀取
         int j = 0;
         char *tok; 
         char* tmp = strdup(line);
         for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n")){
             data[i][j] = atof(tok);//轉換成浮點數 
         }//字符串拆分操作 
         i++;
         free(tmp);
     }   
     fclose(stream);
 }   
int get_number_of_data(char *filename){//return number of row
    char line[1024];
    int i = 0;
    FILE *stream = fopen(filename, "r");
    while(fgets(line, 1024, stream)){
        i++;
    }
    fclose(stream);
    return i;
}

typedef struct pointStruct{
    double x;
    double y;
}point;

void setPoint(point *pt, double x, double y){
    pt->x = x;
    pt->y = y;
}
double calDistance(point *pt1, point *pt2){
    double dis =  sqrt(pow(pt1->x - pt2->x, 2) + pow(pt1->y - pt2->y, 2));
    return dis;
}
int findSmallest(double *dis, int size){//return 最小距離的index
    int smallest = 0;
    for(int i = 0; i < size; i++){
        if(dis[smallest] >= dis[i]){
            smallest = i;
        }
    }
    return smallest;
}
void output(int **class, int *classCount){
    for(int i = 0; i < NumOfMeans; i++){
        printf("Class %d:\n", i);
        for(int j = 0; j < classCount[i]; j++){
            printf("pt%d ", class[i][j]+1);
        }
        printf("\n");
    }
}
int main(){
    char filename[] = "KMeans.csv";//檔案處理
    char line[1024];
    double **data;
    int dataNum = get_number_of_data(filename);//存資料的數量
    data = (double**)malloc(dataNum * sizeof(double));//動態分配二維陣列存資料
    for(int i = 0; i < dataNum; i++){
        data[i] = (double*)malloc(2 * sizeof(double));
    }
    get_two_dimension(line, data, filename);
    printf("data number: %d\n", dataNum);
    for(int i = 0; i < dataNum; i++){
        printf("(%f, %f)\n", data[i][0], data[i][1]);
    }
    printf("\n");

    point mean[NumOfMeans];//中心點
    for(int i = 0; i < NumOfMeans; i++) setPoint(&mean[i], 0, 0);    
    point *pt = (point*)malloc(dataNum * sizeof(point));//動態陣列存point結構
    int* class[NumOfMeans];//動態二維陣列存分配到類的point of index
    for(int i = 0; i < NumOfMeans; i++){
        class[i] = (int*)malloc(dataNum * sizeof(int)); 
    }    
    int classCount[NumOfMeans] = {0};//class的計數器
    int change = 1;
    double sumOfX[NumOfMeans] = {0};
    double sumOfY[NumOfMeans] = {0};
    for(int i = 0; i < dataNum; i++){
        setPoint(&pt[i], data[i][0], data[i][1]);
    }    
    for(int i = 0; i < dataNum; i++){//一開始將前3點當中心
        double dis[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
        int smallest;
        for(int j = 0; j < NumOfMeans; j++){//計算i點到j點的距離
            if(i == j) continue;
            dis[j] = calDistance(&pt[i], &pt[j]);       
        }
        smallest = findSmallest(dis, 3);
        class[smallest][classCount[smallest]] = i;//將距離最小點存到j class
        classCount[smallest]++;
    }
    for(int i = 0; i < NumOfMeans; i++){//分別計算個class的mean
        for(int j = 0; j < classCount[i]; j++){
            sumOfX[i] += pt[class[i][j]].x;
            sumOfY[i] += pt[class[i][j]].y;
        }
        setPoint(&mean[i], sumOfX[i] / classCount[i], sumOfY[i] / classCount[i]);
    }
    output(class, classCount);
    printf("\n");
    while(change == 1){//若class中的點不再移動則分類完成 change == 0
        for(int i = 0; i < NumOfMeans; i++){
            classCount[i] = 0;
            sumOfX[i] = 0;
            sumOfY[i] = 0;
        }
        for(int i = 0; i < dataNum; i++){
            double dis[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
            int smallest;
            for(int j = 0; j < NumOfMeans; j++){
                dis[j] = calDistance(&pt[i], &mean[j]);       
            }
            smallest = findSmallest(dis, 3);
            class[smallest][classCount[smallest]] = i;
            classCount[smallest]++;
        }
        for(int i = 0; i < NumOfMeans; i++){
            for(int j = 0; j < classCount[i]; j++){
                sumOfX[i] += pt[class[i][j]].x;
                sumOfY[i] += pt[class[i][j]].y;
            }
            double averOfX = sumOfX[i] / classCount[i], averOfY = sumOfY[i] / classCount[i];
            if(mean[i].x == averOfX && mean[i].y == averOfY) change = 0;
            setPoint(&mean[i], averOfX, averOfY);
        }
    output(class, classCount);
    printf("\n");
    }      
}
