#include <iostream>
#include <string>
#include<ctime>
using namespace std;

string get_formatted_time(); // it got deleted

class Commit {
public:
    string tree_hash;
    string parent = "nil";
    string author;
    string commit_message;
    string commit_time;
    string dir_name;

    Commit() {}
    Commit(string author, string dir_name) {
        this->author = author;
        this->dir_name = dir_name;
        commit_time = get_formatted_time();

        cout << "commit message : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any leftover characters
        getline(cin, commit_message);
        tree_hash = save_tree_and_return_hash(dir_name);
    }
    Commit(string author, string parent_commit, unordered_set<string> stagedFiles) {
        this->author = author;
        this->dir_name = root;
        commit_time = get_formatted_time();
        
        if (parent_commit != "") {
            parent = parent_commit;
        }

        cout << "commit message : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any leftover characters
        getline(cin, commit_message);
        
        tree_hash = save_tree_and_return_hash(stagedFiles);
    }
    Commit parent_commit() {
        if (parent == "nil") {
            return (*this);
        }
        vector<string> contents = content_from_hash(parent);
        Commit sam;
        sam.tree_hash = contents[0];
        sam.parent = contents[1];
        sam.author = contents[2];
        sam.commit_message = contents[3];
        sam.dir_name = contents[4];
        return sam;
    }

    void set_parent(const string parent) {
        this->parent = parent;  // parent = previous commit hash
    }
    
    string save_commit() {
        string sam = "sample_tree_file";
        ofstream file(sam);
        if (!file.is_open())
        {
            print_err("Error opening file for writing : " + string(strerror(errno)));
            return "error encountered";
        }

        file << tree_hash << endl;
        file << parent << endl;
        file << author << endl;
        file << commit_message << endl;
        file << commit_time << endl;
        file << dir_name;


        file.close();

        convertFileToBlob(sam);
        string hash = sha1_hash(readFileContents(sam));
        deleteFile(sam);
        return hash;
    }
};

Commit create_commit(string commit_hash) {
    vector<string> contents = content_from_hash(commit_hash);
    Commit sam;
    sam.tree_hash = contents[0];
    sam.parent = contents[1];
    sam.author = contents[2];
    sam.commit_message = contents[3];
    sam.dir_name = contents[4];
    return sam;
}

string setup_commit(const string username, const string parent_commit, unordered_set<string> stagedFiles) {
    Commit commit(username, parent_commit, stagedFiles);

    string sam = "sample_tree_file";
    ofstream file(sam);
    if (!file.is_open())
    {
        print_err("Error opening file for writing : " + string(strerror(errno)));
        return "error encountered";
    }

    file << commit.tree_hash << endl;
    file << commit.parent << endl;
    file << commit.author << endl;
    file << commit.commit_message << endl;
    file << commit.commit_time << endl;
    file << commit.dir_name;


    file.close();

    convertFileToBlob(sam);
    string hash = sha1_hash(readFileContents(sam));
    deleteFile(sam);
    // cout << "commit hash created" << endl;
    return hash;
}


class merge_commit {
public:
    string tree_hash;
    string parent1 = "nil";
    string parent2 = "nil";
    string author;
    string commit_message;
    string commit_time;
    string dir_name;

    merge_commit() {}
    merge_commit(string author, string parent_commit1, string parent_commit2, unordered_set<string> stagedFiles) {
        this->author = author;
        this->dir_name = root;
        commit_time = get_formatted_time();

        if (parent_commit1 != "") {
            parent1 = parent_commit1;
        }
        if (parent_commit2 != "") {
            parent2 = parent_commit2;
        }

        cout << "commit message : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any leftover characters
        getline(cin, commit_message);

        tree_hash = save_tree_and_return_hash(stagedFiles);
    }

    merge_commit(string author, string parent_commit1, string parent_commit2) {  // set hash later
        this->author = author;
        this->dir_name = root;
        commit_time = get_formatted_time();

        if (parent_commit1 != "") {
            parent1 = parent_commit1;
        }
        if (parent_commit2 != "") {
            parent2 = parent_commit2;
        }

        cout << "commit message : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any leftover characters
        getline(cin, commit_message);

        
    }
    
    string save_commit() {
        string sam = "sample_tree_file";
        ofstream file(sam);
        if (!file.is_open())
        {
            print_err("Error opening file for writing : " + string(strerror(errno)));
            return "error encountered";
        }

        file << tree_hash << endl;
        file << parent1 << endl;
        file << parent1 << endl;
        file << author << endl;
        file << commit_message << endl;
        file << commit_time << endl;
        file << dir_name;


        file.close();

        convertFileToBlob(sam);
        string hash = sha1_hash(readFileContents(sam));
        deleteFile(sam);
        return hash;
    }

    // merge_commit parent_commit() {
    //     if (parent == "nil") {
    //         return (*this);
    //     }
    //     vector<string> contents = content_from_hash(parent);
    //     merge_commit sam;
    //     sam.tree_hash = contents[0];
    //     sam.parent = contents[1];
    //     sam.author = contents[2];
    //     sam.commit_message = contents[3];
    //     sam.dir_name = contents[4];
    //     return sam;
    // }
};

string parent_hash(string child_hash) {
    return content_from_hash(child_hash)[1];
}


string setup_merge_commit(const string username, const string parent_commit1, const string parent_commit2, unordered_set<string> stagedFiles) {
    merge_commit commit(username, parent_commit1, parent_commit2, stagedFiles);

    string sam = "sample_tree_file";
    ofstream file(sam);
    if (!file.is_open())
    {
        print_err("Error opening file for writing : " + string(strerror(errno)));
        return "error encountered";
    }

    file << commit.tree_hash << endl;
    file << commit.parent1 << endl;
    file << commit.parent2 << endl;
    file << commit.author << endl;
    file << commit.commit_message << endl;
    file << commit.commit_time << endl;
    file << commit.dir_name;


    file.close();

    convertFileToBlob(sam);
    string hash = sha1_hash(readFileContents(sam));
    deleteFile(sam);
    // cout << "commit hash created" << endl;
    return hash;
}





string get_formatted_time() {
    time_t t = std::time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y %z", std::localtime(&t));
    return buffer;
}
