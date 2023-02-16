# TPs Template

This template will help you follow the lessons from [this OpenGL class](https://julesfouchy.github.io/Learn--OpenGL/).

**It works on all platforms (Windows, Linux, MacOS).**

> **Please read the first three chapters very carefully as this project requires some specific setup, in particular when downloading (cloning) the repository.**

- [Setting up](#setting-up)
  - [Creating a repository](#creating-a-repository)
  - [Downloading your repository](#downloading-your-repository)
  - [Compiling](#compiling)
- [Coding](#coding)
  - [Renaming the project](#renaming-the-project)
  - [Folder structure](#folder-structure)
  - [p6](#p6)

## Setting up

### Creating a repository

First, create your own repository based on this one. If you are using GitHub you can use this repository as a template:

![image](https://user-images.githubusercontent.com/45451201/217287736-20058d69-f0ed-40ff-b4e5-cfd852e2ba82.png)

Otherwise simply create a repository on your own and copy-paste all the files in this repo to your new repo.

### Downloading your repository

Run this command inside the directory where you want to put this project:

```
git clone --recursive your_repo_url
```

**NB:** The `--recursive` is very important here because [we make use of submodules](https://julesfouchy.github.io/Learn--Clean-Code-With-Cpp/lessons/git-submodules/).

### Compiling

If that is not already done, [install a compiler](https://julesfouchy.github.io/Learn--Clean-Code-With-Cpp/lessons/install-a-compiler/).

If that is not already done, [setup your IDE for C++ development](https://julesfouchy.github.io/Learn--Clean-Code-With-Cpp/lessons/ide/).

Once that is done, open the project folder in your IDE: it will detect the CMakeLists.txt file automatically and you can just run the project:
![image](https://user-images.githubusercontent.com/45451201/217290500-bd09bc81-861f-4da9-b3c6-fef5e28a83f6.png)

You should see this, with the circle following your mouse cursor:

![initial-window](https://user-images.githubusercontent.com/45451201/217267551-9134512a-6462-4637-963e-d1e5e9519f29.png)

## Coding

### Renaming the project

If you want to rename the project (called "Simple-p6-Setup" by default), you need to change it in 3 places:
- In the `CMakeLists.txt`, change the line `project(Simple-p6-Setup)`
- In the `.github/workflows/build_and_run_tests.yml`, change the line `TARGET: Simple-p6-Setup`
- In the `src/main.cpp`, change the line `auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};`

### Folder structure

All your source files (.cpp) and header files (.hpp) need to go in the `src` folder. It is recommended to have the corresponding .cpp and .hpp next to each other:

```
src
   main.cpp
   some_folder
      some_file.cpp
      some_file.hpp
      another_file.cpp
      another_file.hpp
   another_folder
      a_third_file.cpp
      a_third_file.hpp
```

### p6

*p6* is a 2D-drawing library that is designed to be extremely simple to use. In order to learn how to use it, [read their tutorials](https://julesfouchy.github.io/p6-docs/tutorials/the-context). (Note that you can skip the first chapter "Creating a project" as this template already does everything you need).