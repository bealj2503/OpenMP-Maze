#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

char **arr;
int size;
int seed;
int rank0Ctr = 0;
int rank1Ctr = 0;
int rank2Ctr = 0;
int rank3Ctr = 0;
typedef struct index{
    int x;
    int y;
}Index;
Index *validArr;
int indexOfValidArr;
typedef struct stack{
    int top;
    int max;
    Index *array;
}Stack;

Stack *createStack(int max){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->max = max;
    stack->top = -1;
    stack->array = (Index *)malloc(stack->max * sizeof(Index));
    return stack;
}

int isFull(Stack *stack){
    return stack->top == stack->max -1;
}

int isEmpty(Stack *stack){
    return stack->top == -1;
}

void push(Stack *stack, int x, int y){
    if(isFull(stack)){
        return;
    }
    Index index;
    index.x = x;
    index.y = y;
    stack->array[++stack->top] = index;
}

Index pop(Stack *stack){
    if(isEmpty(stack)){
        Index ret;
        ret.x = -1;
        ret.y = -1;
        return ret;
    }
    return stack->array[stack->top--];
}
Index getDirection(Index curr, int direction){
    Index ret;
    if(direction == 1){
        ret.x = curr.x-2;
        ret.y = curr.y;
    }
    if(direction == 2){
        ret.x = curr.x;
        ret.y = curr.y+2;
    }
    if(direction == 3){
        ret.x = curr.x+2;
        ret.y = curr.y;
    }
    if(direction == 4){
        ret.x = curr.x;
        ret.y = curr.y-2;
    }
    return ret;
}
Index createIndex(int x, int y){
    Index index;
    index.x = x;
    index.y = y;
    return index;
}
void indexPrint(Index index, int count){
    printf("x: %d y: %d iter: %d\n",index.x, index.y, count);
}
int isValid(Index next){
    int x = next.x;
    int y = next.y;
    //printf("x: %d y: %d\n",x,y);
    if(x < 1 || x > size-2 || y < 1 || y >size-2){
        return 0;
    }
    if(arr[x][y] != '.'){
        return 0;
    }
    for(int i= 0;i<indexOfValidArr;i++){
        if(validArr[i].x == next.x && validArr[i].y == next.y){
            return 0;
        }
    }
    return 1;

}
void incRank(int rank){
    if(rank == 0){
        rank0Ctr ++;
    }
    else if(rank == 1){
        rank1Ctr ++;
    }
    else if(rank == 2){
        rank2Ctr ++;
    }
    else if(rank == 3){
        rank3Ctr ++;
    }
}
void claim(Index curr, Index next){
    int rank = omp_get_thread_num();
    char toAdd = rank +'0';
    if(curr.x < next.x){

        {
#           pragma omp critical
            {
                
                arr[next.x][curr.y] = toAdd;
                arr[next.x-1][curr.y] = toAdd;
            }          
            
        }
    }
    if(curr.x > next.x ){
        {
#           pragma omp critical
            {
                arr[next.x][curr.y] = toAdd;
                arr[next.x+1][curr.y] = toAdd;
            }
        }
    }
    if(curr.y < next.y){
        {
#           pragma omp critical
            {       
                arr[curr.x][next.y] = toAdd;
                arr[curr.x][next.y-1] = toAdd;
            }

        }
    }
    if(curr.y > next.y){
        {
#           pragma omp critical
            { 
                arr[curr.x][next.y] = toAdd;
                arr[curr.x][next.y+1] = toAdd;
            }       
        }
    }
}
void shuffle(Index *array, int n){
    srand(seed);
    for(int i = 0; i< 100;i++){
        for(int j = 0;j<n; j++){
            size_t pos = j + rand() /(RAND_MAX / (n-j)+1);
            Index temp = array[pos];
            array[pos] = array[j];
            array[j] = temp;
        }
    }
}

int main(int argc, char *argv[]){
    int thread_count = 1;
    if(strcmp(argv[0],"./mazep")==0){
        thread_count = 4;
        if(argc == 3 && strcmp(argv[1], "-n") == 0){
            size = atoi(argv[2]);
        }
        else if(argc == 5 && strcmp(argv[1], "-n") == 0 && strcmp(argv[3], "-s") == 0){
            size = atoi(argv[2]);
            seed = atoi(argv[4]);
        }
        else{
            size = 11;
            seed = 0;
        }
	if (size <= 3){
	    printf("Size must be atleast 3 or greater, exiting.\n");
	    return 0;
	}
	else if((size%2) == 0){
	    printf("Size must be odd, exiting.\n");
	    return 0;
	}
    }
    else{
        if(argc == 3 && strcmp(argv[1], "-n") == 0){
            size = atoi(argv[2]);
        }
        else if(argc == 5 && strcmp(argv[1], "-n") == 0 && strcmp(argv[3], "-s") == 0){
            size = atoi(argv[2]);
            seed = atoi(argv[4]);
        }
        else{
            size = 11;
            seed = 0;
        }
	if (size <= 3){
	    printf("Size must be atleast 3 or greater, exiting.\n");
	    return 0;
	}
	else if((size%2) == 0){
	    printf("Size must be odd, exiting.\n");
	    return 0;
	}
    }
    arr = malloc(sizeof(char *)*size);
    validArr = malloc(sizeof(Index)*(size*size));
    for(int i = 0;i<size;i++){
        arr[i]= malloc(size);
        for(int j = 0;j<size;j++){
            arr[i][j] = '.';
        }
    }
    
#   pragma omp parallel num_threads(thread_count)
    {
        Index curr;
        int myRank = omp_get_thread_num();
        
        if(myRank == 0){
            curr.x = 1;
            curr.y = 1;   
        }else if(myRank == 1){
            curr.x = 1;
            curr.y = size-2;
        }
        else if(myRank == 2){
            curr.x = size-2;
            curr.y = 1;
        }
        else if(myRank == 3){
            curr.x = size-2;
            curr.y = size-2;
        }
        Stack *stack = createStack(size * size);
        if(arr[curr.x][curr.y] == '.' && isValid(curr) == 1){
#           pragma omp critical
            {
                    arr[curr.x][curr.y] = myRank+'0';
                    validArr[indexOfValidArr] = curr;
                    indexOfValidArr ++; 
            }
            
            push(stack,curr.x,curr.y);
        }
        
        while(isEmpty(stack)!= 1){
            int validCount = 0;
            curr = pop(stack);
            Index directions[4];
            int valid[4];
            directions[0] = getDirection(curr,1);
            directions[1] = getDirection(curr,2);
            directions[2] = getDirection(curr,3);
            directions[3] = getDirection(curr,4);
            for(int i = 0;i<4;i++){
                if(isValid(directions[i])== 1){
#                   pragma omp critical
                    {
                        validArr[indexOfValidArr] = directions[i];
                        indexOfValidArr++;
                    }
                    validCount++;
                    valid[i] = 1;
                }else{
                    valid[i] = 0;
                }
                
            }
            if(validCount != 0){
                Index validDirs[validCount];
                int validIndex = 0;
                for(int i = 0;i<4;i++){
                    if(valid[i] == 1){
                        validDirs[validIndex] = directions[i];
                        validIndex ++;
                    }
                }
                shuffle(validDirs, validIndex);
                for(int i = 0;i<validIndex;i++){
                    push(stack,validDirs[i].x,validDirs[i].y);
                    claim(curr,validDirs[i]);

                    
                }
            }
        }
        free(stack->array);
        free(stack);
    }
    for(int i = 0;i<size;i++){
        for(int j = 0;j<size;j++){
            printf("%c ",arr[i][j]);
            if(arr[i][j] != '.'){
                int rank = arr[i][j] -'0';
                incRank(rank);
            }
        }
        free(arr[i]);
        printf("\n");
    }
    free(arr);
    free(validArr);
    if(thread_count == 1){
        printf("Rank 0 Claimed %d spaces\n",rank0Ctr);
    }
    else{
        printf("Rank 0 Claimed %d spaces\n",rank0Ctr);
        printf("Rank 1 Claimed %d spaces\n",rank1Ctr);
        printf("Rank 2 Claimed %d spaces\n",rank2Ctr);
        printf("Rank 3 Claimed %d spaces\n",rank3Ctr);
    }
    return 0;
}