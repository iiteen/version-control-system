#include <iostream>
#include <string>
using namespace std;

#define main_branch "master"

class Head;

class Head{     // reference to the current branch or current commit
    public:
        string active_branch;
        Head(){

            if(exists(variable_store.substr(0,variable_store.length()))){       // extracting data from root variable to locate the whole working directory
                root_setup_from_variables();
            }

            if(exists(head_contents)){
                active_branch = readFileContents(head_contents);        // points to last active branch
            }else{
                initial_setup();
                active_branch = main_branch;        // default points to master branch
            }
            // create_file(head_contents, active_branch);  // it will generate error
        }
        void change_branch(const string& branch) {    // if wanted to change active branch
            active_branch = branch;
            create_file(head_contents, active_branch);
            update_working_directory(branch);
        }
        ~Head(){
            create_file(head_contents, active_branch);
        }
};