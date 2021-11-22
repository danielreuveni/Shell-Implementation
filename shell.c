/*
Daniel Reuveni
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>


char *token;
int dir;
struct commands {
    pid_t child_pid;
    char full_command[100];
    char command[50];
    char *status;
    int running;
};

//ls implementation
 void ls(){
    struct dirent *d;
    char directory[1000];
    const char *dir = getcwd(directory, sizeof(directory));
	  DIR *dh = opendir(dir);
	  if (dh == NULL)
	{
		if (errno = ENOENT)
		{
      //if the directory doesn't exist
			printf("directory doesn't exist\n");
      fflush(stdout);
		}
    

 }
 if (dh != NULL) {
 while((d=readdir(dh))!=NULL) {
 if((!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")|| (*d->d_name) == '.' ))
            {
            }
       else {
         //print the names of the files
        printf("%s" ,d->d_name);
        fflush(stdout);
        printf("  ");
        fflush(stdout);
            }
    }
 }
        printf(" \n");
        fflush(stdout);

 return;
 }
//ls -l implementation
 void ls_l() {
    DIR *dp;
    struct dirent *dirp;
    const char *dir = getenv("PWD");
    int count = 0;
    struct stat fileStat;
    dp = opendir(dir);
    if (dp) {
      //count the blocks in directory
        while ((dirp = readdir(dp)) != NULL)
    {
        stat(dirp->d_name,&fileStat);   
        count += fileStat.st_blocks;
    }

    closedir(dp);
  }
    printf("total %d\n", count);
    fflush(stdout);

    
    if ((dp = opendir(dir)) == NULL)
        printf("can’t open %s", dir);
    
    //print the permossions for each file
    while ((dirp = readdir(dp)) != NULL){
        stat(dirp->d_name,&fileStat);   
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" %ld ",fileStat.st_nlink);
        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group  *gr = getgrgid(fileStat.st_gid);
        printf("%s %s ", pw->pw_name, gr->gr_name);
        printf("%ld ", (long)fileStat.st_size);
        const time_t * t = &fileStat.st_mtime;
        strtok(ctime(t), " ");
        char* time = ctime(t);
        char* month;
        //print the last editing date
        int i;
        for (i = 0; i < 2; i++)
        {
            month = strtok (NULL, " ");
            printf ("%s ", month);
        }
        month = strtok (NULL, ":");
        printf ("%s:", month);
        month = strtok (NULL, ":");
        printf("%s ", month);
        printf("%s ", dirp->d_name);
        printf("\n");
        fflush(stdout);

        
    }
    return;

 }
//jobs implementation
void jobs(struct commands history_tmp[100], int k) {
    for (int i = 0; i <= k; i++) {
    if (history_tmp[i].child_pid == 0 || waitpid(history_tmp[i].child_pid, NULL, WNOHANG)) {
            //if the process is done and isn't running in background
            history_tmp[i].status = "DONE";
        }
        else {
          //if the proccess is still running
            history_tmp[i].status = "RUNNING";
            if (history_tmp[i].running)
              printf("%s\n", history_tmp[i].full_command);
              fflush(stdout);
        }
  }

  return;
}
//history implementation
void historyCmd(struct commands history_tmp[100], int k) {
  int i;
  for (i = 0; i < k; i++) {
    //print the whole commands in there status (running or not)
    if (history_tmp[i].child_pid == 0 || waitpid(history_tmp[i].child_pid, NULL, WNOHANG)) {
            history_tmp[i].status = "DONE";
        }
        else {
            history_tmp[i].status = "RUNNING";
        }
        printf("%s %s\n", history_tmp[i].full_command, history_tmp[i].status);
        fflush(stdout);
  }
        printf("%s %s\n", history_tmp[i].full_command, history_tmp[i].status);
        fflush(stdout);
  return;
}
//split the input string to words and save its in command list
int createCommandList(char **command_list, char *input) {
  int length, i = 0;
  char*word;
  char copy[100];
  strcpy(copy, input);
  word = strtok(copy, " ");
  command_list[i] = word;
  //if the command is "echo", we need to ignore from " character
if (!strcmp(command_list[0], "echo")) {
      char *str1 = input, *str2 = input;
      char *c = "\"";
    while (*str1) {
        *str2 = *str1++;
        str2 += (*str2 != *c);
    }
    *str2 = '\0';
    }
  word = strtok(input, " ");
  while (word != NULL) {
    command_list[i] = word;
    word = strtok(NULL, " ");
    i++;
  }
  return i;
}
//cd implementation
void cd(char* path, char prev_path[100], char* command_list[100], int k, int length) {
      char current_path[100];
    if (getcwd(current_path, sizeof(current_path)) == NULL) {
        printf("error\n");
        fflush(stdout);
    }
    else {
        if (length == 0) {
          //if the command is "cd" alone, go to home directory
          int dir = chdir(getenv("HOME"));
          if (dir == -1) {
                printf("chdir failed\n");
                fflush(stdout);
            }
            else {
                //set the previous path
                strcpy(prev_path, current_path);
            }
        }
        else if (length >= 2) {
              printf("Too many arguments\n");
              fflush(stdout);
        }
        else
          { 
            //if the command is "cd ~" , go to home directory
            if (strstr(path, "~")) {
              if (!strcmp(path, "~"))
               strcpy(path, getenv("HOME"));
              else {
                strcpy(current_path, getenv("HOME"));
                path++;
                strcat(current_path, path);
                path--;
                strcpy(path, current_path);
              }
            }
            //if the command is "cd -" alone, go to previous directory
            else if (!strcmp(path, "-"))
               strcpy(path, prev_path);
            int dir = chdir(path);
            if (dir == -1) {
                printf("chdir failed\n");
                fflush(stdout);
            }
            else {
                strcpy(prev_path, path);
            }
    }

}
  return;
}

int main() {
    dir = chdir(getenv("PWD"));
    struct commands history[100];
    int k = 0;
    int length = 0;
    pid_t pid;
    char* command_list[100], prev_path[100], path[100]; 

    while (1) {
    int i = 0, j = 0;
    char input[100], input2[100], input3[100];
    char *tmp;
    
    printf("$ ");
    fflush(stdout);
    scanf(" %99[^\n]s", input);

    strcpy(input2, input);
    strcpy(history[k].full_command, input);
    //split the input into array of strings
    length = createCommandList(command_list, input) - 1;
    history[k].status = "RUNNING";
    strcpy(history[k].command, input);
    history[k].child_pid = 0;
    history[k].running = 0;

    if (!strcmp(input, "history")) {
        historyCmd(history, k);
    }
    else if (!strcmp(input, "ls -l")){
        ls_l();
    }
    else if ((!strcmp(input, "ls")) && strlen(input) == 2 ){
        ls();
    }
    else if (!strcmp(input, "exit")){
        exit(0);
    }
    else if (!strcmp(input, "jobs")){
        jobs(history,k);
    }
    else if (!strcmp(command_list[0], "cd")){
        cd(command_list[1], prev_path, command_list, k, length);
    }

    //if the command is not: "jobs", "cd", "exit", "ls" or "history":
    else {
      pid = fork();
      history[k].child_pid = pid;
      if (pid < 0) {
        printf("fork failed\n");
        fflush(stdout);
      }
      if (pid == 0) {
        if (strcmp(command_list[length], "&")) {
                command_list[length+1] = NULL;
                }
                else 
                  command_list[length] = NULL;
                
                int status_code = execvp(command_list[0], command_list);
                if (status_code == -1) {
                  printf("exec failed\n");
                  fflush(stdout);
                  exit(0);
        }
      }
      else {
        if (!strcmp(command_list[length], "&")) {
            char full_command[100]= {0};
            for (j = 0; j < length; j++) {
                    strcat(full_command, command_list[j]);
                        if ( j < length - 1) {
                            strcat(full_command, " ");
                        }
                    }
                    history[k].running = 1;
                    strcpy(history[k].full_command, full_command);
        }
        
        else {
          int st = waitpid(pid, NULL, 0);
           if (st == -1) {
          printf("An error occurred\n");
          fflush(stdout);
          }
        }
      }
    }
    k++;
    }
    
    return 0;
}

