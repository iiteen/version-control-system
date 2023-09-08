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
- you can create your working directory anywhere except in the src folder.
- when the program launches it will ask for the path for your working directory, just provide the full path.
- then it will ask for your username, provide anything you like, its just for checking who made the commit.
- this is first time you are starting this project in a working directory, so use `init` command
  ```shell
  init
  ```
- a staging area will be opened, you need to read all the commands once. you can add files using add command.
  - **only first time when you were asked for path of working directory, you were expected to provide full path. Now everywhere you are required to provide relative paths only.**
  - ```shell
    add example.txt
    ```
-  after adding all the required files, you can proceed to commit.
  ```shell
commit
```
- enter the commit message
- Now further commands could be viewed from the video, Thanks.


**example of basic commands can be seen from video link provided**
[video](https://drive.google.com/uc?id=1Y0xREYoJrl9bAcB1uyNKuc7DFIc_KC8y&export=download)
- video file is large to view online, so just download it.

## points which need attention.
- when you will run `first.cpp` and first time use `init` then this vcs will create a directroy `.git` in working directory, it contains internal files, commit and all other stuffs.
- the problem is above commands will create a new directory `variables` in src directory. This directory links this source code to your working directory, as a consequence this source code can no longer be used for any other new working directory.
- But this could be managed. Just make another copy of src folder and take it in some other directory, in the new `src` folder delete `variables` directory. Now this new `src` folder could be used for any other working directory.

## limitations of this project
- this project could only track files in the working directory. If any file is in the subdirectory of working directory, it won't ne tracked. But this feature could be easily incorporated. (time issues and project deadline).
- errors are not managed very well in this project. so just use commands in the right way.
- some functions may not work exactly like git, but tried my best to mimic them.
- **In real when merging using three-way merge, you will be provided with notepad to edit files while conflicts are encountered. But that part, when notepad pops up is not recorded in video as it was recorded using xbox recorder and it records only 1 window at a time. By default I was recording vscode so notepad didn't appear in video.**
- Internals are left to be explained. They are not covered in video.

I hope you find this project useful and welcome any feedback or contributions! 😊
