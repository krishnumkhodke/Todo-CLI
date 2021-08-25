#include <iostream>
#include<fstream>
#include <ctime>
#include<stack>
using namespace std;

string current_date(){
    char d[100];
    struct tm tstruct;
    time_t t = time(0);
    tstruct = *localtime(&t);
    strftime(d,sizeof(d),"%Y-%m-%d",&tstruct);
    return d;
}

int length_of_file(const char* file_name){
    ifstream file(file_name);
    string line;
    int cnt=0;
    while(getline(file,line))cnt++;
    file.close();
    return cnt;
}

void print_usage()
{
    cout<<"Usage :-"<<endl;
    cout<<"$ ./todo add \"todo item\"  # Add a new todo"<<endl;
    cout<<"$ ./todo ls               # Show remaining todos"<<endl;
    cout<<"$ ./todo del NUMBER       # Delete a todo"<<endl;
    cout<<"$ ./todo done NUMBER      # Complete a todo"<<endl;
    cout<<"$ ./todo help             # Show usage"<<endl;
    cout<<"$ ./todo report           # Statistics"<<endl;


}

void add_todo(int argc,char*argv[])
{
    ofstream todo_file;
    todo_file.open("todo.txt",ios::app);
    string s1;
    s1+=argv[2];
    todo_file<<s1<<endl;
    cout<<"Added todo: \""<<s1<<"\""<<endl;
    todo_file.close();
}

void ls(){
    stack<string>st;
    string line;
    ifstream todo_file;
    int line_nos=0;

    todo_file.open("todo.txt");

    while(todo_file){
        getline(todo_file,line);
        st.push(line);
        line_nos++;
    }

    todo_file.close();
    if(line_nos<=1)cout<<"There are no pending todos!"<<endl;
    while(st.size()){
        if(st.top() != "")
            cout<<"["<<st.size()<<"] "<<st.top()<<endl;
        st.pop();
    }
}

void delete_line(const char *file_name, int n,int mode)
{
    string task_done = "x ";
    task_done += current_date();
    task_done += " ";

    ofstream done_file;
    done_file.open("done.txt",ios::app);
    ifstream is(file_name);
    ofstream ofs;
    ofs.open("temp.txt", ofstream::out);

    char c;
    int line_no = 1;
    while (is.get(c))
    {
        if (line_no != n)
            ofs << c;
        else if(mode==1 && line_no==n)task_done+=c;
        if (c == '\n')
        line_no++;
    }

    if(n>line_no-1 || n<=0){
        cout<<"Error: todo #"<<n<<" does not exist. Nothing deleted."<<endl;
        return ;
    }
    else cout<<"Deleted todo #"<<n<<endl;

    if(mode==1){
        done_file<<task_done;
        cout<<"Marked todo #"<<n<<" as done."<<endl;
    }

    ofs.close();
    is.close();
    done_file.close();
    remove(file_name);
    rename("temp.txt", file_name);
}

void report(){
    cout<<current_date();
    cout<<" Pending : "<<length_of_file("todo.txt")<<" Completed : "<<length_of_file("done.txt")<<endl;
}

int main(int argc,char*argv[])
{
    int a = argc;
    string op,s1;

    if(a==1)print_usage();
    else
    {
        op = argv[1];
        if(op=="add" && a==3)add_todo(argc,argv);
        else if(op=="add" && a==2)cout<<"Error: Missing todo string. Nothing added!"<<endl;
        else if(op=="ls")ls();
        else  if(op=="help")print_usage();
        else if(op=="del" && a==3)delete_line("todo.txt",stoi(argv[2]),0);
        else if(op=="del" && a==2)cout<<"Error: Missing NUMBER for deleting todo."<<endl;
        else if(op=="done" && a==3)delete_line("todo.txt",stoi(argv[2]),1);
        else if(op=="done" && a==2)cout<<"Error: Missing NUMBER for marking todo as done."<<endl;
        else if(op=="report")report();
        else cout<<"Error : Select valid operation"<<endl;
    }
    return 0;
}
