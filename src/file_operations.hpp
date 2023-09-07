#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
// for directory
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
// deleting a directory
#include <filesystem>
//
using namespace std::filesystem;

using namespace std;

string root;
string paths(const string);

string head_contents = ".git/HEAD/head";
string object_store = ".git/objects/";
string branch_store = ".git/branches/";
string variable_store = "variables/";
string stash_store = ".git/stashes/";

string paths(const string filename) {
    return root + "/" + filename;
}

void print_err(const string& content) {      //red
    cerr << "\033[1;31m" + content + "\033[0m" << endl;
}
void print_success(const string& content) { // green
    cout << "\033[1;32m" + content + "\033[0m" << endl;
}

bool create_folder(const string& path)
{
    int status;
    status = mkdir(path.c_str());
    if (status == 0)
    {
        // print_success("Directory created successfully");
        return true;
    }
    else
    {
        if (errno == EEXIST)
        {
            // print_success("Directory already exists");
            return true;
        }
        else
        {
            print_err("Error creating directory" + string(strerror(errno)));
            return false;
        }
    }
}

void create_file(const string fileName, const string content = "")
{
    ofstream file(fileName);
    if (!file.is_open())
    {
        print_err("Error opening " + fileName + " for writing " + string(strerror(errno)));
        return;
    }
    file << content;
    file.close();
    // cout << "file created " + fileName << endl;
}

string readFileContents(const string& filename) {       // full path
    ifstream file(filename);
    if (!file.is_open())
    {
        print_err("Failed to open file: " + filename + ". Error: " + string(strerror(errno)));
        return "";
    }
    string contents = "", line;
    while (getline(file, line))
    {
        contents += line;
        if (!file.eof()) {
            contents += "\n";
        }
    }

    file.close();
    return contents;
}

bool convertFileToBlob(const string& sourceFilename)
{
    string parent_directory_objects = object_store;
    ifstream sourceFile(sourceFilename, ios::binary);
    if (!sourceFile.is_open())
    {
        print_err("Failed to open source file: " + sourceFilename + ". Error: " + string(strerror(errno)));
        return false;
    }

    string filename = sha1_hash(readFileContents(sourceFilename));
    string directory = filename.substr(0, 2);
    directory = parent_directory_objects + directory;
    create_folder(directory);   // new object file will be created @ git/objects/
    // here path could be modified to create file at some specific location
    string blobFilename = directory + "/" + filename.substr(2);

    ofstream blobFile(blobFilename, ios::binary);
    if (!blobFile.is_open())
    {
        print_err("Failed to create blob file: " + blobFilename);
        return false;
    }

    // Read the content of the source file and write it to the blob file
    char buffer[1024];
    while (!sourceFile.eof())
    {
        sourceFile.read(buffer, sizeof(buffer));
        blobFile.write(buffer, sourceFile.gcount());
    }

    // close both files
    sourceFile.close();
    blobFile.close();

    // print_success("File converted to blob: " + blobFilename);
    return true;
}

bool deleteFile(const string& filename)
{
    try {
        remove(filename.c_str());
        // print_success("File '" + filename + "' deleted successfully");
        return true;
    }
    catch (const exception& e) {
        print_err("Error deleting file '" + filename + "':" + e.what());
        return false;
    }
}

bool deleteDirectory(const string& directoryPath)
{
    try
    {
        remove_all(directoryPath);
        // print_success("Directory '" + directoryPath + "' deleted successfully");
        return true;
    }
    catch (const exception& e)
    {
        print_err("Error deleting directory '" + directoryPath + "':" + e.what());
        return false;
    }
}

pair<vector<string>, vector<string>> listFileAndDirectories_path(const string& directory_path) {
    vector<string> directories;
    vector<string> files;
    try {
        for (const auto& entry : directory_iterator(directory_path)) {
            if (entry.path().filename().string()[0] == '.') {
                continue;
            }
            if (entry.is_directory()) {
                directories.push_back(entry.path().string());
            }
            else {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const filesystem_error& e) {
        string sam = e.what();
        print_err("Error: " + sam);
    }

    return make_pair(directories, files);
}

pair<vector<string>, vector<string>> listFileAndDirectories(const string& directory_path)
{
    vector<string> directories;
    vector<string> files;
    try
    {
        for (const auto& entry : directory_iterator(directory_path))
        {
            if (entry.is_directory())
            {
                directories.push_back(entry.path().filename().string());
            }
            else
            {
                files.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const filesystem_error& e)
    {
        string sam = e.what();
        print_err("Error: " + sam);
    }

    return make_pair(directories, files);
}

string path_to_object(string hash) {
    string directory = hash.substr(0, 2);
    directory = object_store + directory;
    string blobFile_path = directory + "/" + hash.substr(2);
    return blobFile_path;
}

vector<string> content_from_hash(string hash) {
    string content = readFileContents(path_to_object(hash));
    vector<string> lines;
    istringstream stream(content);
    string line;
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}