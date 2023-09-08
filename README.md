# Simple Version Control System

This project is a simple version control system, inspired by Git. It provides basic functionality for managing and collaborating on code repositories.

## Features

- **Initialize a repository:** Create a new repository to start tracking your code.
- **Create branches:** Create new branches to work on different features or bug fixes in parallel.
- **Checkout branches:** Switch between branches to work on different parts of the codebase.
- **Delete branches:** Delete branches that are no longer needed.
- **Fast-forward merge:** Merge changes from one branch into another using a fast-forward merge, if possible.
- **Three-way merge:** Merge changes from two branches using a three-way merge, if a fast-forward merge is not possible.
- **Stashing:** Temporarily save changes that are not ready to be committed, and restore them later.
- **Staging area:** Stage changes before committing them, to review and organize your changes.

## Getting Started
- To get started with this version control system, clone the repository and follow the instructions in the documentation to set up your environment and start using the system.
```shell
gh repo clone iiteen/version_control_system
```
- unzip the files, this vcs could be started by running `first.cpp file in src`
- You might be thrown error regarding a library (filesystem). To resolve this, either use latest mingw version [(MYSYS2)](https://www.msys2.org/) or incorporate filesystem library in your c++ code.
- you can create your working directory anywhere except in the src folder
- 


**example of basic commands can be seen from video link provided**
[video](https://drive.google.com/uc?id=1Y0xREYoJrl9bAcB1uyNKuc7DFIc_KC8y&export=download)
- video file is large to view online, so just download it.

## limitations of this project
- this project could only track files in the working directory. If any file is in the subdirectory of working directory, it won't ne tracked. But this feature could be easily incorporated.
- errors are not managed very well in this project. so just use commands in the right way.
- some functions may not work exactly like git, but tried my best to mimic them.
- **In real when merging using three-way merge, you will be provided with notepad to edit files while conflicts are encountered. But that part, when notepad pops up is not recorded in video as it was recorded using xbox recorder and it records only 1 window at a time. By default I was recording vscode so notepad didn't appear in video.**
- Internals are left to be explained. They are not covered in video.

I hope you find this project useful and welcome any feedback or contributions!
