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
#include <sys/stat.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

void CreateAndInsertDataToFile(const char *filename, const string &content);
void CreateAllFiles(const string &dirName, const string projName, bool useRouterView);
string get_cwd();
bool folderExists(const std::string &folderName)
{
#ifdef _WIN32
  std::string command = "if exist \"" + folderName + "\" (exit 0) else (exit 1)";
#else
  std::string command = "test -d \"" + folderName + "\"";
#endif

  return system(command.c_str()) == 0; // Returns true if folder exists
}

void CreateDirectory(const string &path)
{
#ifdef _WIN32
  _mkdir(path.c_str());
#else
  mkdir(path.c_str(), 0777);
#endif
}

string get_cwd()
{
  char cwd[PATH_MAX];
  if (GetCurrentDir(cwd, sizeof(cwd)) != NULL)
  {
    string fullPath(cwd);
    size_t lastSlash = fullPath.find_last_of("/\\");
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
  if (folderExists(projectName))
  {
    cout << "Error: A folder already exists." << endl;
    exit(1); // Exit the program
  }

  string command = "mkdir " + projectName + " && cd " + projectName;

  system(command.c_str()); // Create project directory

  bool useRouterView;
  cout << "Do you want to use router-view in this project? (Y/N): ";
  string response;
  cin >> response;
  useRouterView = (response == "yes" || response == "Yes" || response == "Y");

  CreateAllFiles(projectName, projectName, useRouterView);
}

void initYCmd()
{
  string projName = get_cwd();

  bool useRouterView;
  cout << "Do you want to use router-view in this project? (yes/no): ";
  string response;
  cin >> response;
  useRouterView = (response == "yes" || response == "Yes");

  CreateAllFiles("", projName, useRouterView); // Pass an empty string for the current directory
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
void CreateAllFiles(const string &directoryName, const string projName, bool useRouterView)
{
  string baseDir = directoryName.empty() ? "." : directoryName; // Use current directory if empty

  // Create `src` directory
  string srcDir = baseDir + "/src";
  CreateDirectory(srcDir);
  // Create `index.ts` mid part & start server log
  string indexSrcMidTs, indexSrcStartLogTs, indexSrcImportTs;

  // Create `package.json` file
  string packageJson = R"({
  "name": ")" + projName +
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

  CreateAndInsertDataToFile((baseDir + "/package.json").c_str(), packageJson);

  // Create `.env` file
  CreateAndInsertDataToFile((baseDir + "/.env").c_str(), "PORT=8090");

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

  CreateAndInsertDataToFile((baseDir + "/tsconfig.json").c_str(), tsConfigJson);

  // Create router and view directories if router-view is used
  if (useRouterView)
  {
    string routesDir = srcDir + "/routes";
    string viewsDir = srcDir + "/views";
    CreateDirectory(routesDir);
    CreateDirectory(viewsDir);

    // Create empty index.ts in /routes
    string indexRouteTs = R"(import { Router } from "express";
import helloRouter from "./hello.route.js";

const router = Router();
// Register all your router here.
router.use("/hello", helloRouter);

export default router;)";
    CreateAndInsertDataToFile((routesDir + "/index.ts").c_str(), indexRouteTs);

    // Create empty hello.route.ts in /routes
    string helloRouteTs = R"(import { Router } from "express";
import { helloView } from "../views/hello.view.js";

const helloRouter = Router();
// Define your routes end-points here.
helloRouter.get("/", helloView);

export default helloRouter;)";
    CreateAndInsertDataToFile((routesDir + "/hello.route.ts").c_str(), helloRouteTs);

    // Create empty hello.view.ts in /views
    string helloViewTs = R"(import { Request, Response } from "express";

export const helloView = (req: Request, res: Response) => {
  return res.send({
    status: 200,
    message: "Hello From Route-View Express Typescript Project",
  });
};)";
    CreateAndInsertDataToFile((viewsDir + "/hello.view.ts").c_str(), helloViewTs);

    indexSrcImportTs = R"(import express, { Application } from "express";
import router from "./routes/index.js";)";
    indexSrcMidTs = R"(// * API Path
app.use("/api/v1", router);
)";
    indexSrcStartLogTs = "http://localhost:${PORT}/api/v1/hello/";
  }
  else
  {
    indexSrcImportTs = R"(import express, { Application, Request, Response } from "express";)";
    indexSrcMidTs = R"(
app.get("/", (req: Request, res: Response) => {
  res.send("Hello from TypeScript Express App!");
});
)";
    indexSrcStartLogTs = "http://localhost:${PORT}/";
  }
  string indexTs = indexSrcImportTs + R"(
import { configDotenv } from "dotenv";
import cors from "cors";
const app: Application = express();
// * .env Config
configDotenv({
  path: ".env",
});
const PORT = process.env.PORT || 7000;

// * Middleware
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

)" + indexSrcMidTs +
                   R"(
app.listen(PORT, () =>
  console.log(`Server is running. Visit )" +
                   indexSrcStartLogTs + R"(`)
);)";

  CreateAndInsertDataToFile((srcDir + "/index.ts").c_str(), indexTs);

  cout << "\nSuccess! Now run:" << endl;
  cout << (directoryName.empty() ? "" : "\tcd " + baseDir) << endl; // Output current directory
  cout << "\tyarn (or 'npm i' if you use npm)\n"
       << endl;
  cout << "Now you can start developing your TypeScript Express app by running 'yarn dev' (or 'npm run dev' if you use npm) and edit the /src/index.ts file for changes.\n"
       << endl;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cerr << "Usage: ExpressTs <command>" << endl
         << "Use:\n    1. ExpressTs init\n    2. ExpressTs -y";
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
    cerr << "Invalid command! Use:\n    1. ExpressTs init\n    2. ExpressTs -y" << endl;
  }

  return 0;
}
