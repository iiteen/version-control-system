#include <iostream>
#include <vector>
#include<string>
#include <fstream>

using namespace std;

string fileName(string);
string fileName_stash(string);

class Stash {
public:
    string name;
    string message;
    vector<string> components;

    //constructor for creating stash
    Stash(string& branch_name) {
        name = branch_name;
        auto [_, files] = listFileAndDirectories_path(root);
        for (auto file : files) {
            components.push_back(file);
        }
        add_message();
        save_files_to_stash();
        save_stash();
    }

    //constructor for accessing existing stash
    Stash(string branch_name, string just_a_differentiator) {
        string source = stash_store + branch_name;

        ifstream file(source, ios::binary);
        if (file.fail()) {
            throw runtime_error("Error: unable to open the file " + source);
        }

        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        
        name = lines[0];
        message = lines[1];
        for (size_t i = 2; i < lines.size(); i++) {
            components.push_back(lines[i]);
        }

        restore_stash();

        clear_stash(branch_name);
    }

    void clear_stash(string branch_name) {
        for (auto file : components) {
            if (remove(fileName_stash(file).c_str()) != 0) {
                print_err("unable to delete the stash_stored files " + string(strerror(errno)));
            }
        }
        if (remove((stash_store + branch_name).c_str()) != 0) {
            print_err("unable to delete stash object " + string(strerror(errno)));
        }
    }
    void restore_stash() {

        //clear working directory
        for (const auto& entry : directory_iterator(root)) {
            if (entry.is_regular_file()) {
                remove(entry.path());
            }
        }

        //load stashed file in working directory
        for (auto file : components) {
            string destination = file;
            string source = fileName_stash(file);

            ifstream src(source, ios::binary);
            ofstream dest(destination, ios::binary);

            dest << src.rdbuf();
        }
        
    }
    void save_files_to_stash() {
        for (auto file : components) {
            string source = file;
            string destination = fileName_stash(file);

            ifstream src(source, ios::binary);
            ofstream dest(destination, ios::binary);

            dest << src.rdbuf();
        }
    }
    void save_stash() {
        string destination = stash_store + name;
        
        ofstream file(destination, ios::binary);
        file << name << endl;
        file << message << endl;
        for (auto entry : components) {
            file << entry << endl;
        }
        file.close();
    }

    void add_message() {
        cout << "enter message for stash: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any leftover characters
        getline(cin, message);
    }
};

string fileName(string file_path) {
    size_t pos = file_path.find_last_of("/\\");
    return file_path.substr(pos + 1);
}
string fileName_stash(string file_path_in_working_directory) {
    return stash_store + fileName(file_path_in_working_directory);
}