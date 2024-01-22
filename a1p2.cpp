#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <string>

using namespace std;

int main(int argc,char *argv[]){
    string second(argv[1]);
    //cout << second << endl;

    //validating the argument entered by the user
    if(argc<2){
        cout<< "Missing the second Argument (w or s)\n";
        return 1;
    }
    if(second != "w" && second !="s"){
        cout<<"The second argument should be w or s\n";
        return 1;
    }
    //forking a child and parent process
    //https://man7.org/linux/man-pages/man2/fork.2.html
    pid_t pid;
    pid = fork();
    if (pid<0){
        perror("Fork Error\n");
    }
    else if (pid ==0){
        //child process
        //using execlp to execute the executable file
        // 	https://linux.die.net/man/3/execlp
        if(execlp("./myclock", "myclock", "out1", (char *) NULL)<0){
            perror("Exec error\n");
        }
    }
    else {
        //parent process
        //check for the argument and select the entered mode
        if(second=="w"){
            cout<<"wait mode...\n";
            int status;
            waitpid(pid,&status,0);
        }
        else if(second=="s"){
            cout<<"sleep mode...\n";
            sleep(120);
        }
        else{
            cout <<"Enter w or s in the second argument\n";
        }
    }
    return 0;
}
