/*
 * Name: Pan Zhang, Aodi Zheng
 * Student ID:180108960, 183041160
 * Emails:zhan8960@mylaurier.ca,zhen1160@mylaurier.ca
 * GitHub login ID: zpppQAQ, aodi990211
 * URL: https://github.com/zpppQAQ/CP386_Assignment4_group18
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 100
#define MAX_RES_NUM 100
#define MAX_CUSTOMER 100
#define COMMAND_MAX_CHAR 100
int availableRes[MAX_RES_NUM] = {0};
int maxRes[MAX_CUSTOMER][MAX_RES_NUM] = {0};
int allocationRes[MAX_CUSTOMER][MAX_RES_NUM] = {0};
int needRes[MAX_CUSTOMER][MAX_RES_NUM] = {0};
int safeSeuence[MAX_CUSTOMER] = {0};
void printAvailableRes(int resNum){
    printf("Currently Available resources:");
    for(int i = 0; i < resNum; ++i){
        printf(" %d",availableRes[i]);
    }
    printf("\n");
}

void printAvailable(int availableResTmp[MAX_RES_NUM],int resNum){
    printf("Available Resources:\n");
    for(int i = 0; i < resNum; ++i){
        printf("%d ",availableResTmp[i]);
    }
    printf("\n");
}

void printTwoArr(int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM],int customerNum,int resNum){
    for(int i = 0; i < customerNum; ++i){
        for(int j = 0; j < resNum; ++j){
            printf("%d ",newAllocationRes[i][j]);
        }
        printf("\n");
    }
}

void printAllocation(int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM],int customerNum,int resNum){
    printf("Allocated Resources:\n");
    printTwoArr(newAllocationRes,customerNum,resNum);
}

void printMax(int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM],int customerNum,int resNum){
    printf("Maximum Resources:\n");
    printTwoArr(newAllocationRes,customerNum,resNum);
}

void printNeed(int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM],int customerNum,int resNum){
    printf("Need Resources:\n");
    printTwoArr(newAllocationRes,customerNum,resNum);
}



int safe(int availableResTmp[MAX_RES_NUM],int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM],int customerNum,int resNum){
    // printMax(maxRes,customerNum,resNum);
    // printAvailable(availableResTmp,resNum);
    // printAllocation(newAllocationRes,customerNum,resNum);
    int work[MAX_RES_NUM]={0};
    int finish[MAX_CUSTOMER]={0};
    int need[MAX_CUSTOMER][MAX_RES_NUM]={0};
    // int securitySeq[MAX_CUSTOMER] = {0};
    int currentFinshNum = 0;
    for(int i = 0; i < customerNum; ++i){
        for(int j = 0; j < resNum; ++j){
            need[i][j] = maxRes[i][j] - newAllocationRes[i][j];
        }
    }
    // printNeed(need,customerNum,resNum);
    for(int i = 0; i < MAX_RES_NUM; ++i){
        work[i] = availableResTmp[i];
    }
    while(1){
        int findOneSati = 0;
        for(int i = 0; i < customerNum; ++i){
            if(finish[i] == 1 ){
                continue;
            }
            int allResSati = 1;
            for(int j = 0; j < resNum; ++j){
                if(need[i][j] > work[j]){
                    allResSati = 0;
                    break;
                }
            }
            if(allResSati == 1){
                for(int m = 0; m < resNum; ++m){
                    work[m]+= newAllocationRes[i][m];
                }
                finish[i] = 1;
                safeSeuence[currentFinshNum] = i;
                // printf("current finish num %d  %d\n",currentFinshNum,i);
                // printMax(maxRes,customerNum,resNum);
                // printAvailable(work,resNum);
                // printAllocation(newAllocationRes,customerNum,resNum);
                // printNeed(need,customerNum,resNum);
                currentFinshNum++;
                findOneSati = 1;
                break;
            }
        }        
        if(currentFinshNum == customerNum){
            return 1;
        }
        if(findOneSati == 0){
            return 0;
        }
    }
    return 0;
}

int main(int argc,char * argv[]) {
    int resourceNum = argc-1;
    for(int i = 0; i < resourceNum; ++i){
        availableRes[i] = atoi(argv[i+1]);
    }
    int customerCount = 0;
    FILE *fp;
    fp = fopen("./sample4_in.txt","r");
    char line[100];
    char *lines[MAX_LINE];
    while(!feof(fp)){
        if(fgets(line,100,fp) != NULL){
            lines[customerCount] = malloc(sizeof(line) * sizeof(char));
            strcpy(lines[customerCount], line);
            customerCount++;
        }
    }
	for (int k = 0; k < customerCount; k++) {
		char *token = NULL;
		token = strtok(lines[k], ",");
        int j = 0;
		while (token != NULL) {
            // printf("%d ",atoi(token));
            maxRes[k][j] = atoi(token);
			token = strtok(NULL, ",");
            j++;
		}
	}
    printf("Number of Customer: %d \n",customerCount);
    printAvailableRes(resourceNum);
    printf("Maximum resources from file:\n");
    for(int i = 0; i < customerCount; ++i){
        for(int j = 0; j < resourceNum; ++j){
            printf("%d ",maxRes[i][j]);
        }
        printf("\n");
    }
    char command[COMMAND_MAX_CHAR];
    while(1){
        printf("Enter Command:");
        scanf("%s",command);
        int newAllocationRes[MAX_CUSTOMER][MAX_RES_NUM] = {0};
        int newAvailableRes[MAX_RES_NUM] = {0};
        for(int i = 0; i < MAX_CUSTOMER; i++){
            for(int j = 0; j <MAX_RES_NUM; ++j ){
                newAllocationRes[i][j] = allocationRes[i][j];
            }
        }
        for(int j = 0; j <MAX_RES_NUM; ++j ){
            newAvailableRes[j] = availableRes[j];
        }
        if(strcmp(command,"RQ") == 0){
            int customerid;
            scanf("%d",&customerid);
            for(int i = 0; i < resourceNum; ++i){
                int resCount;
                scanf("%d",&resCount);
                newAllocationRes[customerid][i] += resCount;
                newAvailableRes[i] -= resCount;
            }
            if(safe(newAvailableRes,newAllocationRes,customerCount,resourceNum) == 1){
                printf("State is safe, and request is satisfied\n");
                for(int i = 0; i < resourceNum; ++i){
                    availableRes[i] = newAvailableRes[i];
                    allocationRes[customerid][i] = newAllocationRes[customerid][i];
                }
                for(int i = 0; i < customerCount; ++i){
                    for(int j = 0; j < resourceNum; ++j){
                        needRes[i][j] = maxRes[i][j] - allocationRes[i][j];
                    }
                }
            }
        }
        else if(strcmp(command,"RL") == 0){
            int customerid;
            scanf("%d",&customerid);
            for(int i = 0; i < resourceNum; ++i){
                int resCount;
                scanf("%d",&resCount);
                allocationRes[customerid][i] -= resCount;
                availableRes[i] += resCount;
            }           
            printf("The resources have been released successfully\n") ;
        }
        else if(strcmp(command,"Status") == 0){
            printAvailable(availableRes,resourceNum);
            printMax(maxRes,customerCount,resourceNum);
            printAllocation(allocationRes,customerCount,resourceNum);
            printNeed(needRes,customerCount,resourceNum);
        }
        else if(strcmp(command,"Run") == 0){
            int customerid;
            if(safe(newAvailableRes,newAllocationRes,customerCount,resourceNum) == 1){
                printf("Safe Sequence is:");
                for(int i = 0; i < customerCount; ++i){
                    printf(" %d",safeSeuence[i]);
                }
                printf("\n");
                for(int i = 0; i < customerCount; ++i){
                    printf("--> Customer/Thread %d\n",safeSeuence[i]);
                    printf("Allocated resources:");
                    for(int k = 0;k < resourceNum; ++k){
                        printf(" %d",allocationRes[safeSeuence[i]][k]);
                    }
                    printf("\n");
                    printf("Needed:");
                    for(int j = 0; j < resourceNum; ++j){
                        printf(" %d",maxRes[safeSeuence[i]][j] - allocationRes[safeSeuence[i]][j]);
                    }
                    printf("\n");
                    printf("Available:");
                    for(int j = 0; j < resourceNum; ++j){
                        printf(" %d",availableRes[j]);
                    }
                    printf("\n");
                    printf("Thread has started\n");
                    printf("Thread has finished\n");
                    printf("Thread is releasing resources\n");
                    for(int j = 0; j < resourceNum; ++j){
                        availableRes[j] += allocationRes[safeSeuence[i]][j];
                    }
                    printf("New Available:");
                    for(int j = 0; j < resourceNum; ++j){
                        printf(" %d",availableRes[j]);
                    }
                    printf("\n");
                }
            }            
        }
        else if(strcmp(command,"Exit") == 0){
            break;
        }
    }
	return 0;
}
