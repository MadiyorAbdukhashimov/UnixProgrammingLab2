#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>

// U1610131 - Madiyor Abdukhashimov
// INHA University - Unix Programming Lab Assignment#2.2
// this programm takes file name and input random characters through arguments)
// One sample ./exeName FileName 123456789

void strReverse(char *array){
    char *stored = (char *) malloc(sizeof(char) * strlen(array));
    strncpy(stored, array, strlen(array));
    int size = strlen(array);
    memset(array, 0, sizeof(array));
    for(size_t i = 0; i < size; i++)
    {
        array[i] = stored[size - i - 1];
    }
}

int main(int argc, char const *argv[])
{
    int fd = -1, fd1 = -1;

    char *fileName, inputCharacters[255];

    char buffer[255];
    int size = 0;

    memset(inputCharacters, 0, sizeof(inputCharacters));
    memset(buffer, 0, sizeof(buffer));

    if (argc < 2) {
        printf("missing filename\n");
        exit(1);
    }else if (argc < 3){
        printf("Input is missing");
        printf("For example: 128,A16\n");
        exit(1);
    }
    
    if (strlen(argv[1]) > 255) {
        printf("FileName is too long\n");
        exit(1);
    }
    fileName = (char *)malloc(sizeof(char) * strlen(argv[1]));
    strcpy(fileName, argv[1]);

    strncpy(inputCharacters, argv[2], strlen(argv[2]));

    if((fd = open(fileName, O_CREAT|O_TRUNC|O_RDWR, 0777)) == -1){
        printf("Couldn't create file %s \n", fileName);
        exit(1);
    }

    if (write(fd, inputCharacters, (size_t)strlen(inputCharacters)) == -1) {
        printf("Couldn't write to %s \n", fileName);
        exit(1);
    }
    
    close(fd);

    if((fd = open(fileName, O_RDWR, 0777)) == -1){
        printf("Couldn't create file %s \n", fileName);
        exit(1);
    }

    if((fd1 = open("out.txt", O_CREAT|O_TRUNC|O_WRONLY)) == -1){
        printf("Couldn't create file %s \n", fileName);
        exit(1);
    }

    size = read(fd, buffer, 1024);

    if (size < 0) {
        printf("Could not read from file\n");
    }
    
    lseek(fd, (off_t)0, SEEK_SET);

    for(size_t i = 0; i < size /2; i++)
    {
        read(fd, &buffer[i], 1);
        write(fd1, &buffer[i], 1);
    }

    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, 1024);
    strReverse(buffer);
    write(fd1, buffer, strlen(buffer));

    remove(fileName);
    rename("out.txt", fileName);
    return 0;
}
