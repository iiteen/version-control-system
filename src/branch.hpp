#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <unordered_set>
using namespace std;

#define main_branch "master"

class Branch;
// class Stash;

string branching_menu = R"(
    welcome to vcs, some vcs commands.
    
    init                               {creates the default repo} (if it's your first time then choose this.)
    branch   <branch_name>             {creates a new branch with given name}
    checkout <branch_name>             {shifts to given branch>}
    delete   <branch_name>             {deletes a non-active branch}
    stage                              {allows to stage on current branch}
    show                               {show all branches}
    quit                               {to close the vcs}
    menu                               {shows menu again}
    merge <merge-type> <branch_name>   {merges active branch with branch_name using merge-type}
    merges                             {shows codes for various merge-types}
    stash                              {stashes active branch}
    restash                            {change the state of working directory same as the previously stashed version}
)";


string merge_menu = R"(
    welcome to merge menu, some merge-types.

    fast forward merge                  ffm
    three way merge                     twm
)";

void solve_conflict(string);

class Branch
{ // named reference to a commit
public:
    string commit_hash = "";
    string branch_name;

    Branch() // last active or default branch
    {
        if (exists(head_contents)) {
            branch_name = readFileContents(head_contents); // becomes last active branch
            load_data_from_branch();
        }
        else {
            branch_name = main_branch; // becomes default active branch but it is not pointing to any commit

            // // start commiting
            // // doubt--------------------------------------------------------- what should we assume at start
            // string dir_name;
            // cout << "let's setup repo, we are creating a commit for you root directory." << endl;
            // // cout << "directory to be committed: ";
            // // cin >> dir_name;
            // dir_name = root; // assuming that first time its neccesary to commit the entire root directory
            // commit_hash = setup_commit(username, dir_name, "");     // ""-> as no parent

            StagingArea stage;
            cout << staging_menu << endl;
            stage.take_input();
            commit_hash = stage.commit(""); // parent passes as nil

            save_branch();
            create_file(head_contents, head.active_branch);
        }
    }
    Branch(const string& branchName)
    { // points to last commit on active branch if not exists already
        branch_name = branchName;
        if (exists(branch_store + branch_name)) {
            load_data_from_branch();
        }
        else {
            string past_branch = readFileContents(head_contents);
            load_data_from_other_branch(past_branch);
        }
        save_branch();
    }
    Branch(const string& hash, const string& branch)
    {
        branch_name = branch;
        commit_hash = hash;
        save_branch();
    }

    void save_branch()
    {
        ofstream file(branch_store + branch_name);
        if (!file.is_open())
        {
            print_err("Error opening " + branch_store + branch_name + " for writing " + string(strerror(errno)));
            return;
        }
        file << commit_hash;
        file.close();
        // cout << "branch saved" << endl;
    }
    bool delete_branch() { // only deletes the pointer to the commit, hence inefficient
        if (branch_name == head.active_branch) {
            print_err("don't be oversmart just delete non-active branches");
            return false;
        }
        // cout << "trying to delete the branch" << endl;
        return deleteFile(branch_store + branch_name);
    }

    void commit()
    { // check it later
        StagingArea stage;
        cout << staging_menu << endl;
        stage.take_input();
        commit_hash = stage.commit(commit_hash);

        save_branch();
    }

    void fast_forward_merge(Branch& initial_branch) {
        commit_hash = initial_branch.commit_hash;
        save_branch();
        initial_branch.delete_branch();
        update_working_directory(branch_name);
    }

    void three_way_merge(Branch b2) {
        Tree tree1(content_from_hash(commit_hash)[0]);
        Tree tree2(content_from_hash(b2.commit_hash)[0]);


        merge_commit commit(username, commit_hash, b2.commit_hash);

        Tree tree;

        vector<Node> list1 = tree1.components;
        vector<Node> list2 = tree2.components;

        for (auto it = list1.begin(); it != list1.end(); ++it) {
            bool found = false;
            for (auto jt = list2.begin(); jt != list2.end(); ++jt) {
                if (it->name == jt->name) {
                    if (it->hash == jt->hash) {
                        // Node which are also in list2, i.e. nodes which have same name and same hash
                        tree.components.push_back(*it);
                        list2.erase(jt);
                    }
                    else {
                        // nodes which are also in list1 and list2 (same name but different hash)


                        string sam = "sample_merged_file";
                        ofstream file(sam);
                        if (!file.is_open()) {
                            print_err("Error opening file for writing : " + string(strerror(errno)));
                            return;
                        }

                        vector<string> lines1 = content_from_hash(it->hash);
                        vector<string> lines2 = content_from_hash(jt->hash);

                        for (const string& line : lines1) {
                            file << line << endl;
                        }
                        for (const string& line : lines2) {
                            file << line << endl;
                        }
                        file.close();
                        cout << "conflict created in file: " << it->name << endl;

                        //resolve conflicts between B and B'
                        solve_conflict(sam);

                        convertFileToBlob(sam);
                        it->hash = sha1_hash(readFileContents(sam));
                        deleteFile(sam);

                        tree.components.push_back(*it);
                        list2.erase(jt);
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                // nodes which are not in list2 but in list1
                tree.components.push_back(*it);
            }
        }

        // now add all the remaining elements from list2 to new list(tree.components)
        tree.components.insert(tree.components.end(), list2.begin(), list2.end());

        string hash = tree.save_tree_blob();
        commit.tree_hash = hash;
        string merge_commit_hash = commit.save_commit();

        commit_hash = merge_commit_hash;
        save_branch();

        update_working_directory(branch_name);
    }

    void create_stash() {
        if (head.active_branch == branch_name) {
            Stash stash_object(branch_name);
        }
        else {
            print_err("this action could be performed only on active branch");
        }
    }

    void restore_stash() {
        if (head.active_branch == branch_name) {
            try {
                Stash stash_object(branch_name, "load existing stash");
            }
            catch (const runtime_error& e) {
                print_err(e.what());
            }
        }
        else {
            print_err("this action could be performed only on active branch");
        }
    }

    void load_data_from_branch()
    {
        ifstream file(branch_store + branch_name);
        if (!file.is_open())
        {
            print_err("Error opening branch for extracting: " + string(strerror(errno)));
            cout << branch_store + branch_name << endl;
            return;
        }

        file >> commit_hash;
    }
    void load_data_from_other_branch(const string& branch)
    {
        ifstream file(branch_store + branch);
        if (!file.is_open())
        {
            string sample = strerror(errno);
            print_err("Error opening branch for extracting: " + sample);
            cout << branch_store + branch << endl;
            return;
        }

        file >> commit_hash;
    }
};

void list_branches() {
    auto [directories, files] = listFileAndDirectories(branch_store);
    cout << GREEN << "Branches:" << RESET << endl;
    for (auto& filename : files)
    {
        cout << (filename == head.active_branch ? "* " : "  ") << filename << endl;
    }
}


string find_just_before_common_ancestor(string commit1, string commit2) {    //assuming that commit1 will be of active branch
    unordered_set<string> visited;
    string ancestor = "nil";
    while (commit1 != "nil") {
        visited.insert(commit1);
        commit1 = parent_hash(commit1);
    }
    string sam_commit = commit2;
    while (commit2 != "nil" && (visited.find(commit2) == visited.end())) {
        sam_commit = commit2;  // storing last value
        commit2 = parent_hash(commit2);
    }
    if (commit2 == "nil") {
        return "no common ancestor";
    }
    else {
        return sam_commit;
    }
}

void solve_conflict(string filename) {
#ifdef __APPLE__

    //code for macOS
    string command = "open -a TextEdit " + filename;
    system(command.c_str());

#elif _WIN32

    //code for windows
    string command = "notepad.exe " + filename;
    system(command.c_str());

#endif
}




// here rebasing is fully not implemented as it lacks features of diffs and patches
void rebase(Branch master, Branch b1) {
    string b1_tail_hash = find_just_before_common_ancestor(master.commit_hash, b1.commit_hash);
    Commit b1_tail = create_commit(b1_tail_hash);
    b1_tail.parent = master.commit_hash;
    b1_tail.save_commit();
    //fast forward merge
    master.fast_forward_merge(b1);
}