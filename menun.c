//
// Created by diego on 28/04/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){

    char source_id[60];
    char dst_id[60];
    char hour_pipe[60];
    char answer[60];

    unsigned  short int option;

    printf("---------------");
    printf("Bienvenido-------------------\n \n");
    printf("1. Ingresar origen\n2. destino  \n3. hora \n4. Buscar tiempo de viaje medio \n5. Salir\n \n");


    while(atoi(source_id) == 0 || atoi(source_id)>1160 || atoi(source_id)<0){
        printf("Source: ");
        fgets(source_id,60,stdin);
    }

    while(atoi(dst_id) == 0 || atoi(dst_id)>1160 || atoi(dst_id)<0){
        printf("Dst: ");
        fgets(dst_id,60,stdin);
    }

    while(1){
        printf("Hour: ");
        fgets(hour_pipe,60,stdin);
        if(atoi(hour_pipe)<=23 || atoi(hour_pipe)>=0){
            break;
        }
    }


    int fd;
    char * fifoprac = "/tmp/fifoprac";
    mkfifo(fifoprac, 0666);
    fd = open(fifoprac, O_WRONLY);

    //write data in pipe
    write(fd, source_id, sizeof(source_id));
    write(fd, dst_id, sizeof(dst_id));
    write(fd, hour_pipe, sizeof(hour_pipe));
    close(fd);

    fd = open(fifoprac, O_RDONLY);
    read(fd, answer, sizeof(answer));
    close(fd);

    printf("Source: %d\n",atoi(source_id));
    printf("Dst_id: %d\n",atoi(dst_id));
    printf("Hour: %d\n",atoi(hour_pipe));

    if( atof(answer) ==0){
        printf("Nand");
    }else{
         printf("Mean: %.2f\n", atof(answer));
    }

    return 0;
}


