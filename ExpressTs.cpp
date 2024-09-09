#include <iostream>
#include <string>
#include <limits.h>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
#include <direct.h> // Windows
#define GetCurrentDir _getcwd
#else
#include <unistd.h> // Linux/macOS
#define GetCurrentDir getcwd
#endif

using namespace std;

void CreateAndInsertDataToFile(const char *filename, const string &content);
void CreateAllFiles(const string &dirName);

string get_cwd()
{
    char cwd[PATH_MAX];
    if (GetCurrentDir(cwd, sizeof(cwd)) != NULL)
    {
        string fullPath(cwd);
        // Extract the last part (the directory name)
        size_t lastSlash = fullPath.find_last_of("/\\"); // Handle both '/' and '\\'
        return fullPath.substr(lastSlash + 1);
    }
    else
    {
        perror("Error getting current directory");
        exit(1);
    }
}
// Function to handle `init` command
void initCmd()
{
    string projectName;
    cout << "Enter project name: ";
    cin >> projectName;

    string command = "mkdir " + projectName + " && cd " + projectName + " && mkdir src";
    system(command.c_str()); // Create project directory

    CreateAllFiles(projectName);
}

void initYCmd()
{
    string projectName = get_cwd();

    string command = "mkdir " + projectName + " && cd " + projectName + " && mkdir src";
    system(command.c_str()); // Create project directory

    CreateAllFiles(projectName);
}

void CreateAndInsertDataToFile(const char *filename, const string &content)
{
    ofstream file;
    file.open(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    file << content;
    file.close();
}

// Function to create all necessary files in the given project directory
void CreateAllFiles(const string &directoryName)
{
    // Create `package.json` file
    string packageJson = R"({
  "name": ")" + directoryName +
                         R"(",
  "version": "1.0.0",
  "main": "dist/index.js",
  "type": "module",
  "scripts": {
    "start": "node dist/index.js",
    "watch": "tsc -w",
    "server": "nodemon dist/index.js",
    "dev": "concurrently \"npm run watch\" \"npm run server\""
  },
  "keywords": [],
  "author": "",
  "license": "ISC",
  "description": "",
  "devDependencies": {
    "@types/cors": "^2.8.17",
    "@types/express": "^4.17.21",
    "@types/node": "^20.14.11",
    "concurrently": "^8.2.2",
    "nodemon": "^3.1.4",
    "typescript": "^5.5.3"
  },
  "dependencies": {
    "cors": "^2.8.5",
    "dotenv": "^16.4.5",
    "express": "^4.19.2"
  }
})";

    CreateAndInsertDataToFile((directoryName + "/package.json").c_str(), packageJson);

    // Create `.env` file
    CreateAndInsertDataToFile((directoryName + "/.env").c_str(), "PORT=8090");

    // Create `tsconfig.json` file
    string tsConfigJson = R"({
    "compilerOptions": {
      "rootDirs": ["src"],
      "outDir": "dist",
      "lib": ["es2020"],
      "target": "es2020",
      "module": "Node16",
      "moduleResolution": "Node16",
      "esModuleInterop": true,
      "types": ["node"]
    },
    "include": ["src"]
})";

    CreateAndInsertDataToFile((directoryName + "/tsconfig.json").c_str(), tsConfigJson);

    string indexTs = R"(import express, { Application, Request, Response } from "express";
import "dotenv/config";
import cors from "cors";
const app: Application = express();
const PORT = process.env.PORT || 7000;

// * Middleware
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

app.get("/", (req: Request, res: Response) => {
  res.send("Hello from TypeScript Express App!");
});

app.listen(PORT, () =>
  console.log(`Server is running. Visit http://localhost:${PORT}`)
);)";

    CreateAndInsertDataToFile((directoryName + "/src/index.ts").c_str(), indexTs);

    cout << "\nSuccess! Now run:" << endl;
    cout << "\tcd " << directoryName << endl;
    cout << "\tyarn (or 'npm i' if you use npm)\n"
         << endl;
    cout << "Now you can start developing your TypeScript Express app by running 'yarn dev' (or 'npm run dev' if you use npm) and edit the /src/index.ts file for changes.\n"
         << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: expressTs <command>" << endl
             << "Use:\n    1. expressTs init\n    2. expressTs -y";
        return 1;
    }

    string command = argv[1];

    if (command == "init")
    {
        initCmd(); // Call interactive initialization
    }
    else if (command == "-y")
    {
        initYCmd(); // Call non-interactive initialization
    }
    else
    {
        cerr << "Invalid command! Use:\n    1. expressTs init\n    2. expressTs -y" << endl;
    }

    return 0;
}