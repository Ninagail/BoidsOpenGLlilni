# OpenGL TPs Template

This template will help you follow the lessons from [this OpenGL class](https://julesfouchy.github.io/Learn--OpenGL/).

**It works on all platforms (Windows, Linux, MacOS).**

> **Please read the first three chapters very carefully as this project requires some specific setup, in particular when downloading (cloning) the repository.**

- [Setting up](#setting-up)
  - [Creating a repository](#creating-a-repository)
  - [Downloading your repository](#downloading-your-repository)
  - [Compiling](#compiling)
- [Coding](#coding)
  - [Folder structure](#folder-structure)
- [Switching TP](#switching-tp)

## Setting up

### Creating a repository

First, create your own repository based on this one. If you are using GitHub you can use this repository as a template:

![image](https://user-images.githubusercontent.com/45451201/217287736-20058d69-f0ed-40ff-b4e5-cfd852e2ba82.png)

Otherwise simply create a repository on your own and copy-paste all the files in this repo to your new repo, and add p6 as a submodule manually.

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

You should see this:

![initial-window](https://user-images.githubusercontent.com/45451201/219445041-f6d881eb-7dea-42ad-8360-31a8d9eca66d.png)

## Coding

### Folder structure

All your files for the current TP will go in the folder with the same name (e.g. `TP1`). All the files that you want to reuse in all the TPs will go in `src-common`. All of your assets (textures, 3D models, etc.) will go in the `assets` folder.

## Switching TP

To switch between the different TPs, simply change the current CMake target:
![Change CMake Target](https://user-images.githubusercontent.com/45451201/219449536-0ade5391-0f64-404d-958d-067667594686.png)