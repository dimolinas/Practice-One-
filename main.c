#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#define MAX 1161;

int hashFunction( int key){
    return key % MAX;
}
void readCsv(struct bucket * bucketArray){

    FILE * c = fopen("bogota.csv","r");

    if(!c){
        printf("Can´t open the file\n");
        exit(1);
    }
    else{
        printf("File was read\n");
        char buffer[2048];
        unsigned short  int row,column;
        row = 0;
        column = 0;
        unsigned  short int source_id,dts_id,hour,index;
        float mean_travel;

        while (fgets(buffer,1024, c)) {

            column = 0;
            row++;
            //printf("Row: %d\t",row);

            // To avoid printing of column
            if (row == 1)
                continue;

            // Splitting the data
            char * value = strtok(buffer, ",");

            while (value) {
                // Column 1
                if (column == 0) {
                    source_id = atoi(value);
                    //printf("SourceId: %d\t",source_id );
                }

                // Column 2
                if (column == 1) {
                    dts_id = atoi(value);
                    //printf("dstId: %d\t",dts_id);
                }

                // Column 3
                if (column == 2) {
                    hour = atoi(value);
                    //printf("Hod: %d\t",hour);
                }

                if (column == 3){
                    mean_travel = atof(value);
                    //printf("MeanTravel: %.1f",mean_travel);
                }

                value = strtok(NULL, ",");
                column++;
            }

            //add node to chaining hash table
            index = hashFunction(source_id);
            insert(&((bucketArray)+index)->next,source_id,dts_id,hour,mean_travel);
            (bucketArray+index)->amount++;
            (bucketArray+index)->sourceId = source_id;

            //printf("\n");
        }

        // Close the file
        fclose(c);
    }

}

int main() {

    //size of each struct
    printf("Size: %d\n", sizeof(struct Node));

    //ideas for Hash Table
    struct bucket * bucketArray = malloc(1161 * sizeof(struct bucket));

    for(int i=0;i<1161;i++){
        bucketArray[i].amount = 0;
    }

    //Fill Hash Table
    readCsv(bucketArray);

    //printf("LenghtIndex: %d\n",bucketArray[23].amount);
    //printList(&bucketArray[23].next);

    //create file
    FILE * file = fopen("complete.bin","wb+");
    int i = 1;

    while(i<1161){

        //test
        printf("Bucket: %d\tAmount: %d----------------------------------------------------------------\n",bucketArray[i].sourceId,bucketArray[i].amount);

        if(bucketArray[i].amount != 0){
            //Create one bucket for to save in the file
            struct bucketFile * new_bucket = (struct bucketFile *) malloc(sizeof(struct bucketFile));
            new_bucket->sourceId = bucketArray[i].sourceId;
            new_bucket->amount = bucketArray[i].amount;
            printf("Source: %d\tAmount: %d\n",new_bucket->sourceId,new_bucket->amount);
            fwrite(new_bucket,sizeof(struct bucketFile),1, file);
        }

        //through linkedList
        int j = 0;
        //head of linked from bucket position
        struct Node * tmp = bucketArray[i].next;
        while(tmp){
            //test info
            printf("Source: %d\tDst: %d\tHour: %d\tMean: %.2f\n",tmp->source_id,tmp->dst_id,tmp->hour,tmp->mean_travel);

            //create one node for to save in the file continue to bucket or other nodes
            struct nodeFile * new_node = (struct nodeFile *) malloc(sizeof(struct nodeFile));
            new_node->source_id = tmp->source_id;
            new_node->dst_id = tmp->dst_id;
            new_node->hour = tmp->hour;
            new_node->mean_travel = tmp->mean_travel;

            fwrite(new_node, sizeof(struct nodeFile),1,file);

            //increment in node and position
            j++;
            tmp = tmp->next;
        }

        i++;
    }
    fclose(file);
    //query in hash table
    float query = searchDstId(&(bucketArray+23)->next,502,14);
    printf("Mean :%.2f\n",query);

    return 0;

}

