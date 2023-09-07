#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
string username;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[1;35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";

#include "sha1.hpp"

#include "file_operations.hpp"
#include "setup.hpp"
#include "tree.hpp"
#include "commit.hpp"
#include "branch_change.hpp"
#include "head.hpp"

Head head;

#include "staging.hpp"
#include "stash.hpp"
#include "branch.hpp"


#define prefix cout<<MAGENTA<<username<<RESET<<" ("+head.active_branch+") "


void merge() {
    Branch master;
    string command;
    cin >> command;
    if (command == "ffm") {
        string branch;
        cin >> branch;
        Branch b1(branch);
        master.fast_forward_merge(b1);
    }
    else if (command == "twm") {
        string branch;
        cin >> branch;
        Branch b1(branch);
        master.three_way_merge(b1);
    }
    else {
        print_err("use some valid code of merge");
    }
    return;
}

int main() {

    cout << "enter your username: ";
    cin >> username;

    cout << branching_menu << endl;
    string command;
    prefix << GREEN << "$ " << RESET;
    cin >> command;
    while (command != "quit")
    {
        if (command == "init") {
            Branch master;
        }
        else if (command == "branch") {
            string branch_name;
            cin >> branch_name;
            Branch master(branch_name);
        }
        else if (command == "checkout") {
            string branch_name;
            cin >> branch_name;
            Branch master(branch_name);
            head.change_branch(master.branch_name);
        }
        else if (command == "delete") {
            string branch_name;
            cin >> branch_name;
            Branch master(branch_name);
            master.delete_branch();
        }
        else if (command == "stage") {
            Branch master;
            master.commit();
        }
        else if (command == "show") {
            list_branches();
        }
        else if (command == "menu") {
            cout << branching_menu << endl;
        }
        else if (command == "merge") {
            merge();
        }
        else if (command == "merges") {
            cout << merge_menu << endl;
        }
        else if (command == "stash") {
            Branch master;
            master.create_stash();
        }
        else if (command == "restash") {
            Branch master;
            master.restore_stash();
        }
        else {
            print_err("unknown command entered");
        }
        prefix << GREEN << "$ " << RESET;
        cin >> command;
    }
}