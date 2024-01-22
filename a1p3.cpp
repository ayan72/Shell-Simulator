#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <time.h>
#include <sys/times.h>
#include <sstream>
#include <iterator>
using namespace std;
#define MAXLINE 128
#define MAXWORD 20

//declaring a struct to store the pids and the command line
struct ProcessTable{
  pid_t pid;
  vector<string> cmd;
};
//https://man7.org/linux/man-pages/man2/times.2.html
//tms struct to calucate the end and start time.
struct tms start_time;
struct tms end_time;

//keep track of the number of processes executed.
int process_num = 0;

// exec function to execute the executable file
// arguments: cmd, which is a vector of strings.
// https://linuxhint.com/execvp-function-c/
void exec(const vector <string> &cmd){
  char *arg[6];
  arg[0] = (char *)(cmd[0].c_str());
  for (int i =1;i < cmd.size(); i++){
     arg[i] = (char *)(cmd[i].c_str());
  }
  arg[cmd.size()] = nullptr;
  execvp(arg[0],arg);
}

//Used to print the process table containing the pid and command.
// arguments: process, a vector of the struct ProcessTable
void print_table(vector <ProcessTable> &process){
    cout << "Process Table:\n";
    for (const auto &process_info : process) {
    cout << process_num << " [";
    cout << process_info.pid << ": ' ";
      for (const auto &word : process_info.cmd) {
          cout << word << " ";
      }
    cout << "']" << endl;
    process_num++;
    }
}


int main(int argc,char *argv[]) {
    //calling times to get the start time
    clock_t start = times(&start_time);

    //CHecking for the correctness of the arguments entered.
    if (argc != 2){
        cout<<"Enter the correct arguments\n";
        return 1;
    }
    int choice;
    choice = stoi(argv[1]);
    if (!(choice==0||choice==1||choice==-1)){
        cout<<"Enter 1, -1, or 0 as the second argument\n";
        return 1;
    }
    char *args[6];
    string line;

    // int count = 1;
    vector<string> cmd;
    vector <ProcessTable> process;

    //using getline to read the input file line by line
    while(getline(cin, line)){
      //skipping the line which are empty or start with '#'
      if(line.front()=='#' ||line.empty()){
        continue;
      }

      //calling strtok to tokenize the input line
      //	https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
      char *c = strtok((char *)(line.c_str())," ");
      while (c && cmd.size() < 5) {
          cmd.push_back(c);
          c = strtok(nullptr, " ");
      }

      //printing the command in the input file
      cout << "Print_cmd():  [ ";
      for (int i = 0; i < cmd.size(); i++){
        cout << cmd[i] << " ";
      }
      cout << "]";
      cout << endl;

      //creating a child and parent process using fork()
      //https://www.geeksforgeeks.org/fork-system-call/
      pid_t pid = fork();
        if (pid == -1) {
          // fork has failed
            perror("Fork has failed\n");
            exit(1);
        } else if (pid == 0) {
          //child process 
            exec(cmd);
            cerr << "child ("<< getpid() <<") :"<< "unable to execute '";
            cerr << cmd[0]<< "'\n";
            exit(1);
        } else {
          //parent process
            process.push_back({pid, cmd});
        }
        cmd.clear();
    }
    //printing the table
    print_table(process);
    if (choice ==0){
      //continue
    } else if(choice ==1){
      //wait for one process 
      int status;
      waitpid(process[0].pid,&status,0);
      cout <<"process("<<process[0].pid<<")";
      cout << " exited (status = "<<status<<")\n";
    } else if(choice ==-1){
      // wait for all the processes
      for (const ProcessTable &proc : process){
        int status;
        waitpid(proc.pid,&status,0);
        cout << " process ";
        cout << "( "<<proc.pid<<": ' ";
          for (int i =0;i < proc.cmd.size();i++){
            cout << proc.cmd[i]<<" ";
          }
        cout << "')" << " exited (status = "<<status<<")\n";
      }
    } else{
      cout << "Invalid wait option\n";
    }

    //calculate the time 
    clock_t end = times(&end_time);
    double clock_tick;
    clock_tick = sysconf(_SC_CLK_TCK);
    //dividing the time by the clock ticks to get the time in seconds
    double real = (end - start)/clock_tick;
    double user = (end_time.tms_utime - start_time.tms_utime)/clock_tick;
    double sys = (end_time.tms_stime - start_time.tms_utime)/clock_tick;
    double cuser = (end_time.tms_cutime - start_time.tms_cutime)/clock_tick;
    double csys = (end_time.tms_cstime - start_time.tms_cstime)/clock_tick;

    printf("real:      %7.2f\n",real);
    printf("user:      %7.2f\n",user);
    printf("sys:      %7.2f\n",sys);
    printf("child user:      %7.2f\n",cuser);
    printf("child sys:      %7.2f\n",csys);
    //waiting for all the processes to finish when option 0 or 1 is entered
    for (const ProcessTable &proc : process){
      int status;
      waitpid(proc.pid,&status,0);
    }
    //exit(EXIT_SUCCESS);

  return 0;
}
