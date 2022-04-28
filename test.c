//Standards inputs outputs
#include <stdio.h>
#include <stdlib.h>

//Meseasure the time
#include <time.h>

//Rute on sys
#include <sys/stat.h>

//Pipes
#include <unistd.h>
#include <fcntl.h>

//Header
#include "node.h"

int main(){


    //buffer pipe
    char source_id[60];
    char dst_id[60];
    char hour_pipe[60];
    char answer[60];

    //time
    clock_t start, end;
    start= clock();

    //read from pipe
    int fd1;
    char * fifoprac = "/tmp/fifoprac";
    mkfifo(fifoprac, 0666);
    fd1 = open(fifoprac, O_RDONLY);
    read(fd1, source_id, sizeof(source_id));
    read(fd1, dst_id, sizeof(dst_id));
    read(fd1, hour_pipe, sizeof(hour_pipe));

    //parameters
    unsigned short int source_id_pipe = atoi(source_id);
    unsigned short int dst_id_pipe = atoi(dst_id);
    unsigned short int hour_id_pipe = atoi(hour_pipe);

    //read bin file
    FILE * file = fopen("complete.bin", "rb+");
    struct bucketFile * readBucket = (struct bucketFile *) malloc(sizeof(struct bucketFile));

    //save how many memory would to jump
    int jump = 0;

    //input
    unsigned short int input = source_id_pipe;
    unsigned short int hour = hour_id_pipe;
    unsigned  short int dst = dst_id_pipe;

    while (fread(readBucket, sizeof(struct bucketFile),1,file)){

        //printf("Source: %d\n",readBucket->sourceId);
        if(readBucket->sourceId == input){
            break;
        }
        jump = jump + sizeof(struct bucketFile)+readBucket->amount* sizeof(struct nodeFile);
        fseek(file,jump,SEEK_SET);
    }


    printf("Source: %d\tAmount: %d\n",readBucket->sourceId,readBucket->amount);

    //Read blues or nodes
    struct nodeFile * blue = (struct nodeFile * ) malloc(sizeof(struct nodeFile));
    jump = jump + sizeof(struct bucketFile);
    fseek(file,jump,SEEK_SET);
    int numberBlue = readBucket->amount;

    for(int i=0;i<= numberBlue ;i++){
        fread(blue, sizeof(struct nodeFile),1,file);
        //printf("I: %d\t",i);
        //printf("Source: %d\tDst: %d\tHour: %d\tMean: %.2f\n",blue->source_id,blue->dst_id,blue->hour,blue->mean_travel);
        if(blue->dst_id == dst && blue->hour == hour){
            break;
        }
        fseek(file,jump + sizeof(struct nodeFile)*i,SEEK_SET);
    }

    float final = blue->mean_travel;
    printf("Mean: %.2f",final);

    end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nfor loop took %f seconds to execute \n", cpu_time_used);

    //write pipe answer
    int ret = snprintf(answer, sizeof answer, "%f", final);

    fd1 = open(fifoprac, O_WRONLY);
    write(fd1, answer, sizeof(answer));
    close(fd1);

    return 0;

}