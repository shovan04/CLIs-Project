#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h> // For mkdir
#include <cerrno>
#include <algorithm> // for std::transform
#include <cctype>    // For errno
#include <cstring>   // For strerror
#include <vector>
#include <cstdlib> // for system

using namespace std;
bool createFolder(const string &folderName);
void createFiles(const string &folderName);

int main()
{
    string folderName;
    bool ErrorCreateProject;

    do
    {
        ErrorCreateProject = false;
        cout << "Enter Folder/Project Name: ";
        getline(cin, folderName);
        // Use getline to read entire line with spaces
        replace(folderName.begin(), folderName.end(), ' ', '-');

        if (!createFolder(folderName))
        {
            ErrorCreateProject = true;
            cout << "Please try again.\n" << endl;
        }
    } while (ErrorCreateProject);
    cout << "\nProject Created Successfully.\nHappy Coding :)\n" << endl;
    return 0;
}
bool createFolder(const string &folderName)
{
    if (mkdir(folderName.c_str()) == 0)
    {
        createFiles(folderName);
        return true;
    }
    else
    {
        if (errno == EEXIST)
        {
            cerr << "\nError: Folder '" << folderName << "' already exists." << endl;
        }
        else
        {
            cerr << "Error: " << strerror(errno) << endl;
        }
        return false;
    }
}

void createFiles(const string &folderName)
{
    // Define filenames and content
    string uppercaseFolderName = folderName;
    transform(uppercaseFolderName.begin(), uppercaseFolderName.end(), uppercaseFolderName.begin(), ::toupper);

    vector<string> filenames = {"index.html", "style.css", "script.js"};
    vector<string> contents = {"<!DOCTYPE html>\n\
<html lang='en'>\n\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <link rel='stylesheet' href='style.css'>\n\
    <title>" + uppercaseFolderName + "</title>\n\
</head>\n\n\
<body>\n\
    <h1>Welcome to " + uppercaseFolderName + "</h1>\n\
    <p>This is a sample HTML file for folder " + uppercaseFolderName + ".</p>\n\
</body>\n\t\
<script src='script.js' async defer></script>\n\
</html>","body {\n\tmargin: 0;\n\tpadding: 0;\n}",
"console.log('JS File for " + uppercaseFolderName + " Project.')"};

    // Create files in the folder
    for (size_t i = 0; i < filenames.size(); ++i)
    {
        string filePath = folderName + "/" + filenames[i];
        ofstream outputFile(filePath);
        if (!outputFile)
        {
            cerr << "Error: Failed to create file '" << filePath << "'." << endl;
            continue; // Skip to the next file
        }
        outputFile << contents[i] << endl;
        outputFile.close();
    }

    string openFolderCmd = "cd " + folderName + " && code . && exit";
    system(openFolderCmd.c_str());
}
