#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

// void print(string input) {
//     cout << input << endl;
// }
Tree get_files_in_branch(const string& branch_name) {
    string commit_hash = readFileContents(branch_store + branch_name);
    // print(commit_hash);
    string tree_hash = content_from_hash(commit_hash)[0];
    // print(tree_hash);
    Tree tree(tree_hash);
    // tree.print_tree();

    return tree;
}

vector<string> read_file_in_branch(string file_hash) {
    return content_from_hash(file_hash);
}


/*void update_working_directory(const string& old_branch, const string& new_branch) {
    //list of files in old and new branch
    vector<Node> old_files = get_files_in_branch(old_branch).components;
    vector<Node> new_files = get_files_in_branch(new_branch).components;


    // remove files that are not in the new branch
    for (const auto& file : old_files) {
        auto it = find_if(new_files.begin(), new_files.end(), [&file](const Node& node) {
            return node.name == file.name;
            });
        if (it == new_files.end()) {
            remove(file.name.c_str());
        }
    }

    //update or create files
    for (const auto& file : new_files) {
        vector<string> lines = read_file_in_branch(file.hash);
        ofstream out(file.name);
        for (size_t i = 0; i < lines.size(); ++i) {
            out << lines[i];
            if (i != lines.size() - 1) {
                out << endl;
            }
        }
        out.close();
    }
}
*/

void update_working_directory(const string& new_branch_name) {
    //list of files in old and new branch
    vector<Node> new_files = get_files_in_branch(new_branch_name).components;


    // remove all the files in working directory except for folders whose names start with "."
    // files in any folders will be unaffected in this version of code
    for (const auto& entry : directory_iterator(root)) {
        if (entry.is_regular_file()) {
            remove(entry.path());
        }
    }

    //update or create files
    for (const auto& file : new_files) {
        vector<string> lines = read_file_in_branch(file.hash);
        ofstream out(file.name);
        for (size_t i = 0; i < lines.size(); ++i) {
            out << lines[i];
            if (i != lines.size() - 1) {
                out << endl;
            }
        }
        out.close();
    }
}