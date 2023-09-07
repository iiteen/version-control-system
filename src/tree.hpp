#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string save_tree_and_return_hash(const string &);

struct Node
{
    string hash;
    string name; // may be name of directory or file
    string type; // it's a file or directory
};

class Tree
{
public:
    string directory_name = "root";
    vector<Node> components;

    Tree() {}
    Tree(const string& tree_hash) {     //complete hash of tree
        ifstream file(path_to_object(tree_hash));
        if (file.is_open()) {
            string line;
            getline(file, line);
            add_tree_name(line);
            while (getline(file, line)) {
                istringstream iss(line);
                Node node;
                iss >> node.hash >> node.name >> node.type;
                components.push_back(node);
            }
            file.close();
        }
        else {
            string sam = strerror(errno);
            print_err("unable to open file: " + sam);
        }
    }

    void add_tree_name(string& name)
    {
        directory_name = name;
    }

    void add_file(const string &filename)
    {
        Node file;
        file.name = filename;
        file.type = "file";
        convertFileToBlob(filename);
        file.hash = sha1_hash(readFileContents(filename));
        components.push_back(file);
    }
    void add_directory(const string &subtreename)
    {
        Node dir;
        dir.name = subtreename;
        dir.type = "directory";
        dir.hash = save_tree_and_return_hash(subtreename); // also blob is made
        components.push_back(dir);
    }
    void print_tree() const {
        cout << "Tree name: " << directory_name << endl;
        for (const Node& node : components) {
            cout << node.hash << " " << node.name << " " << node.type << endl;
        }
    }

    string save_tree_blob() {
        string sam = "sample_tree_file";
        ofstream file(sam);
        if (!file.is_open())
        {
            print_err("Error opening file for writing : " + string(strerror(errno)));
            return "error encountered";
        }
        
        file << directory_name << endl;

        for (auto it = components.begin(); it != components.end(); ++it)
        {
            const Node& item = *it;
            file << item.hash << " " << item.name << " " << item.type;
            if (it != prev(components.end())) {
                file << endl;
            }
        }
        file.close();

        convertFileToBlob(sam);
        string hash = sha1_hash(readFileContents(sam));
        deleteFile(sam);
        
        return hash;
    }
};

Tree version_directory(string);
Tree version_root(const unordered_set<string>&);

string save_tree_and_return_hash(const string &subtreename)
{
    string sam = "sample_tree_file";
    ofstream file(sam);
    if (!file.is_open())
    {
        print_err("Error opening file for writing : " + string(strerror(errno)));
        return "error encountered";
    }
    Tree subtree = version_directory(subtreename); // official blob made at this time of its files and folders
    file << subtree.directory_name << endl;

    for (auto it = subtree.components.begin(); it !=subtree.components.end(); ++it)
    {
        const Node &item = *it;
        file << item.hash << " " << item.name << " " << item.type;
        if(it != prev(subtree.components.end())){
            file << endl;
        }
    }
    file.close();

    convertFileToBlob(sam);
    string hash = sha1_hash(readFileContents(sam));
    deleteFile(sam);
    // cout << "subtree hash created" << endl;
    return hash;
}

string save_tree_and_return_hash(const unordered_set<string>& stagedFiles)
{
    string sam = "sample_tree_file";
    ofstream file(sam);
    if (!file.is_open())
    {
        print_err("Error opening file for writing : " + string(strerror(errno)));
        return "error encountered";
    }
    Tree subtree = version_root(stagedFiles); // official blob made at this time
    file << subtree.directory_name << endl;

    for (auto it = subtree.components.begin(); it != subtree.components.end(); ++it)
    {
        const Node &item = *it;
        file << item.hash << " " << item.name << " " << item.type;
        if (it != prev(subtree.components.end()))
        {
            file << endl;
        }
    }
    file.close();

    convertFileToBlob(sam);
    string hash = sha1_hash(readFileContents(sam));
    deleteFile(sam);
    // cout << "subtree hash created" << endl;
    return hash;
}
Tree version_root(const unordered_set<string> &stagedFiles)
{
    Tree tree1;
    // tree1.add_tree_name(root);

    for (auto &entry : stagedFiles)
    {
        tree1.add_file(entry);
    }
    return tree1;
}

Tree version_directory(string dir_name)
{
    Tree tree1;
    tree1.add_tree_name(dir_name);
    auto [directories, files] = listFileAndDirectories_path(dir_name);

    for (auto &entry : files)
    {
        tree1.add_file(entry);
    }
    for (auto &entry : directories)
    {
        tree1.add_directory(entry);
    }

    return tree1;
}