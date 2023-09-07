#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// for directory
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
// deleting a directory
#include <filesystem>
//
using namespace std::filesystem;


using namespace std;
void initial_root_setup(){
    cout << "enter the working directory (full path) [use '/']: ";
    cin >> root;

    head_contents = paths(head_contents);
    branch_store = paths(branch_store);
    object_store = paths(object_store);
    stash_store = paths(stash_store);

    create_folder(variable_store); // values of variables

    create_file(variable_store+"root", root);
}
void root_setup_from_variables(){
    root = readFileContents(variable_store + "root");

    head_contents = paths(head_contents);
    branch_store = paths(branch_store);
    object_store = paths(object_store);
    stash_store = paths(stash_store);
}
void initial_setup(){

    initial_root_setup();

    create_folder(paths(".git"));
    create_folder(paths(".git/HEAD"));     // data contained by head -> active branch
    create_folder(paths(".git/objects"));  // name of objects
    create_folder(paths(".git/branches")); // name of branches
    create_folder(paths(".git/stashes")); // stash objects
}