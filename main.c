#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<stdbool.h>

// поиск образца в строке
int search(char* str1, char* str2, int a, int b)
{
    int flag = 0, count = 0, pos, i;
    if (a > b) {
        return -1;
    }
    for (i = 0; i < b - a + 1; i++) {
        if (str1[0] == str2[i]) {
            count++;
            for (int j = 0; j < a - 1; j++) {
                pos = i;
                if (str1[j] == str2[i + j]) {
                    flag = 1;
                } else {
                    flag = 0;
                    break;
                }
            } if (flag == 1) {
                break;
            }
        } 
    }
    if (flag == 1 && count > 0) {
        

        
        return i;

    }
    return -1;
}

int main()
{
    int fd1[2], fd2[2], fd3[2];
    char ans[10];
    
    if (pipe(fd1)==-1) 
    { 
        perror("pipe error\n");
        exit(1);
    } 
    if (pipe(fd2)==-1) 
    { 
        perror("pipe error\n");
        exit(1);
    } 
        if (pipe(fd3)==-1) 
    { 
       perror("pipe error\n"); 
       exit(1);
    } 

  
    pid_t p = fork();
    if (p < 0) {
        perror("fork error\n");
        exit(1);

    } else if(p > 0) {
        char str1[100], str2[100];
        int size1, size2;
        // считывание образца
        size1 = read(0, str1, 100); 
        // считывание строки
        size2 = read(0, str2, 100); 

        // передача образца в дочерний процесс
        write(fd1[1], str1, size1); 
        close(fd1[1]);
        // передача строки в дочерний процесс
        write(fd2[1], str2, size2); 
        close(fd1[1]);
        wait(NULL); // ожидание дочернего процесса



        int size_ans;
        // считывание результата из дочернего процесса
        size_ans = read(fd3[0], ans, 10); 
        ans[size_ans] = '\n';
        
        write(1, ans, size_ans + 1); 
        
    } else {
        char child_str1[100], child_str2[100];
        int c_size1, c_size2;

        // считывание образаца из родительского процесса
        c_size1 = read(fd1[0], child_str1, 100); 
        close(fd1[0]);
        // считывание строки из родительского процесса
        c_size2 = read(fd2[0], child_str2, 100); 
        close(fd2[0]);

        // проверка наличия образаца в строке
        int pos;
        pos = search(child_str1, child_str2, c_size1, c_size2);
        if (pos == -1) {
            ans[0] = '-';
            ans[1] = '1';
            ans[2] = '\0';
        } else {
            if (pos >= 10) {
                ans[0] = (pos / 10) % 10 + '0';
                ans[1] = pos % 10 + '0';
                ans[2] = '\0';
            } else {
                ans[0] = pos + '0';
                ans[1] = '\0';

            }
        }
        
        write(fd3[1], ans, strlen(ans) + 1);
        exit(0);
    }
}