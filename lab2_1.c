#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>

void to_binary(int number, char *array){
    int i = 0;
    int size = 0;
    char stored[32];
    memset(stored, 0, sizeof(stored));
    for(int i = 0; number > 0; i++)
    {
        if (number%2 == 0) {
            stored[i] = '0';
        }else
        {
            stored[i] = '1';
        }
        number/=2;
    }
    size = strlen(stored);
    for(size_t i = 0; i < size; i++)
    {
        array[i] = stored[size - i - 1];
    }
}

int main(int argc, char const *argv[])
{
    int fd = -1, fd1 = -1;

    char *fileName, inputCharacters[255];

    char buffer;
    char answer[255];
    int sum = 0;

    memset(inputCharacters, 0, sizeof(inputCharacters));

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

    int i = 0;
    int j = 0;
    int k = 0;
    while(read(fd, &buffer, 1)){
        if (isdigit(buffer)) {
            if (i%2 == 0) {
                k = 1;
                if (j == 0) {
                    sum = atoi(&buffer);
                    printf("\na[%d] = %c - # - %dj - s - %d\n", i, buffer, j, sum);
                    j = 1;
                }else
                {
                    sum = atof(&buffer) + sum*10;
                    printf("\na[%d] = %c - # - %dj - s - %d\n", i, buffer, j, sum);
                }
                i++;
                while(read(fd, &buffer, 1) > 0){
                    if (!isdigit(buffer)) {
                        printf("\na[%d] = %c - x - %dj - s - %d\n", i, buffer, j, sum);
                        j = 0;
                        lseek(fd, (off_t)-1, SEEK_CUR);
                        break;
                    }else
                    {
                        sum = atoi(&buffer) + sum*10;
                        printf("\na[%d] = %c - # - %dj - s - %d\n", i, buffer, j, sum);
                        i++;
                    }
                }
            }else
            {
                write(fd1, &buffer, 1);
                printf("\na[%d] = %c - $ - %dj - s - %d\n", i, buffer, j, sum);
                i++;
                while(read(fd, &buffer, 1) > 0){
                    if (isdigit(buffer)) {
                        write(fd1, &buffer, 1);
                        printf("\na[%d] = %c - $ - %dj - s - %d\n", i, buffer, j, sum);
                        i++;
                    }else
                    {
                        lseek(fd, (off_t)-1, SEEK_CUR);
                        break;
                    }
                }
            }
        }else
        {
            printf("\na[%d] = %c - @ - %dj - s - %d\n", i, buffer, j, sum);
            write(fd1, &buffer, 1);
            i++;
        }

        if (k == 1) {
            memset(answer, 0, sizeof(answer));
            to_binary(sum, answer);
            printf("%s\n", answer);
            sum = 0;
            j = 0;
            write(fd1, answer, strlen(answer));
            memset(answer, 0, sizeof(answer));
        }
        
    }
    remove(fileName);
    rename("out.txt", fileName);
    return 0;
}