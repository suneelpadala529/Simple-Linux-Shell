#include <unistd.h>     // getpid(), getcwd()
#include <sys/types.h>  // type definitions, e.g., pid_t
#include <sys/wait.h>   // wait()
#include <signal.h>     // signal name constants and kill()
#include <iostream>
#include <vector>
#include <string.h>

using namespace std;
struct dir_stack
{
  char* path_value;
  dir_stack* next;
};

dir_stack* paths = NULL;

void insert_path(char* pathv)
{
    struct dir_stack* new_path = (struct dir_stack*)malloc(sizeof(struct dir_stack));
    new_path->path_value = (char *)malloc(sizeof(char)*100);
    if(new_path!=NULL){
        strcpy(new_path->path_value, pathv);
        new_path->next = paths;
        paths = new_path;
    }
    cout<<"\n Dir info Pushed into Stack \n\n";
}

char* pop_path(){
    char* pathv = (char *)malloc(sizeof(char)*100);
    if(paths!=NULL)
    {
        strcpy(pathv,paths->path_value);
        dir_stack* temp=paths;
        paths = paths->next;
        free(temp);
        cout<<"\n\nDir Poped Out From the stack\n\n";
        return(pathv);
    }
    else
    {
        strcpy(pathv,"_UNDERFLOW");
        return(pathv);
    }
}

void print_paths()
{
    dir_stack* temp = paths;
    if(temp == NULL)
        cout<<"\nStack is UnderFlow\n\n";
    else
        cout<<"Stack data :: \n";
    while(temp!=NULL)
    {
        cout<<temp->path_value<<"\n";
        temp = temp->next;
    }
}

int main()
{
    int i;
    char *path = strdup("");
    vector<string> p;
    while(true)
    {
        //Display Command line Interface.
        cout <<"SUNEEL-SRIJITH-ADEEB@CTech-19"<<"$";
        char input_command[128];
        cin.getline(input_command, 128);

        vector<char*> args;
        char *recv_cmd = strtok(input_command, " ");
        char *tmp = recv_cmd;
        while(tmp != NULL)
        {
            args.push_back(tmp);
            tmp = strtok(NULL, " ");
        }
        //assign arguments to array of strings
        char **argv = new char*[args.size()+1];
        cout<<"\nReceived command and arguments :: ";
        for(int k = 0;k < args.size();k++)
        {
            argv[k] = args[k];
            cout<<" "<<argv[k];
        }
        cout<<"\n\n";
        //Assign last node to NULL
        argv[args.size()] = NULL;

        if (strcmp(input_command, "") == 0)
        {
            cout<<"\n";
        }
        else
        {
            if (strcmp(recv_cmd, "exit") == 0)
            {
                return 0;
            }
            else if (!strcmp(recv_cmd, "cd"))
            {
                if (argv[1] == NULL)
                {
                    chdir("..");
                    cout <<"\ncurrent directory ::"<<getcwd(NULL, 0)<<" \n\n";
                }

                else
                {
                    int temp=0;
                    temp = chdir(argv[1]);
                    if(temp==-1)
                    {
                        cout<<"Entered path does not exist in the directory\n";
                    }
                    cout <<"\ncurrent directory ::"<<getcwd(NULL, 0)<<" \n\n";
                }
            }
            else if(!strcmp(recv_cmd, "pushd"))
            {
                char cwd[1000];
                insert_path(getcwd(cwd, sizeof(cwd)));
                if(argv[1] == NULL)
                {
                    chdir("..");
                }
                else
                {
                    chdir(argv[1]);
                }
            }
            else if(!strcmp(recv_cmd, "popd"))
            {
                char* new_path = pop_path();
                if(!strcmp(new_path, "_UNDERFLOW")){
                    cout<<"Stack is UnderFlow\n";
                }
                else{
                    chdir(new_path);
                }
            }
            else if(!strcmp(recv_cmd, "dirs"))
            {
                print_paths();
            }
            else if(!strcmp(recv_cmd,"path"))
            {
                if(argv[1] == NULL)
                {
                    if(p.size()==0)
                        cout<<"\nPath variable does not contain any paths info \n\n";
                    else
                    {
                        for(int i=0;i<p.size();i++)
                        {
                            cout<<p[i];
                            if(i<p.size()-1)
                                cout<<":";
                        }
                        cout<<"\nPath cmd completed \n\n";
                    }
                }
                else if(!strcmp(argv[1],"+"))
                {
                    if(argv[2] != NULL)
                    {
                    for (int i = 2; i < args.size(); i++)
                        p.push_back(argv[i]);
                    }
                    else
                        cout<<"\nInclude Path \n\n";
                }
                else if(!strcmp(argv[1],"-"))
                {
                    if (argv[2]==NULL)
                        cout<<"\n Path required to delete \n\n";
                    else
                    {
                        if(p.size()==0)
                            cout<<"\nPath variable does not contain any Info to Remove\n\n";
                        else
                        {
                            int index=0;
                            char* t=strdup("");
                            int *found_flag, *err_flag;
                            err_flag = new int(args.size());
                            for (int i = 2; i < args.size(); i++)
                            {
                                found_flag[i]=0;
                                err_flag[i] = 0;
                            }
                            for (int i=2; i < args.size(); i++)
                            {
                                for(int j=0;j<p.size();j++)
                                {

                                    strcpy(t, p[j].c_str());
                                    if(!strcmp(t,argv[i]))
                                    {
                                        err_flag[i]=0;
                                        index=j;
                                        p.erase(p.begin()+index);
                                        found_flag[i] =1;
                                        break;
                                    }
                                    else
                                        err_flag[i] = 1;
                                }
                            }
                            for (int i = 2; i < args.size(); i++)
                            {
                                if(err_flag[i])
                                    cout<< argv[i]<<" : Path Not Found \n";
                            }
                        }
                    }
                }
                else
                {
                    cout<<"\n\nInvalid Syntax of Path variable\nUsage: \n1) Add Path: path + /folder/to/include/ \n2) Delete path: path - /folder/to/delete \n";
                }
            }
            else
            {
                pid_t process_id = fork();
                if(process_id < 0)
                {
                    perror("\nInternal error: cannot fork.\n");
                    return -1;
                }
                else if(process_id == 0)
                {   // Child Process.
                    int temp=0;
                    temp= execlp(getcwd(NULL,0),*argv,NULL);
			//cout<<"temp="<<temp;
                    if(temp==-1)
                        cout<<"Command doesn't exist in "<<getcwd(NULL, 0)<<"\n\n";
                        for(int k=0;k<p.size();k++)
                        {
                            string s(*argv);
                            p[k]+=s;
                            strcpy(path, p[k].c_str());
                            cout<<"Command exist in "<<path<<"\n\n";
                            execvp(path, argv);
                        }
                        fprintf(stderr,"Command doesn't exist in the paths \n\n");
                        //return -1;
                }
                else
                {   // Parent Process.  Wait for the child.
                    if (waitpid(process_id, 0, 0) < 0 )
                    {
                        perror("Internal error: cannot wait for child." );
                        return -1;
                    }
                }
            }
        }
    }
    free(path);
    return 0;
}
