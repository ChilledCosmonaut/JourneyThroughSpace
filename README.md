# GameLab 1 Project Repository

|  General Info  | |
| ---|---|
| Working Title |  |
| Final Title |  |
| Participant 1 | Name, E-Mail, sNumber |
| Participant 2 | Name, E-Mail, sNumber |
| Participant N (if applicable) | Name, E-Mail, sNumber |
|Engine Type | |
|Start-Date| 2/10/2017 |
|Study Program| Games Engineering B.Sc.|

### Abstract

Describe your Project in a few sentences.
Be precise.

## GitLab Informations and Repository Management

This is a skeleton repository for a GameLab 1 Project including documentation.
It contains two main folders:

- Code
- Documentation

The folder *Code* will house the actual game project.
The repository is already configured so that Unreal or Unity projects placed below `Code`, i.e. `Code/MyUnrealProjectFolder` or `Code/MyUnityProjectFolder` will be correctly handled by Git.
This includes using GitLFS for large binary files and ignoring redundant data to keep the repository small.
As long as you place your files and projects at the correct location, it should work out-of-the-box.

Read more about git in the [Atlassian Git Tutorials](https://de.atlassian.com/git).

The folder *Documentation* contains the report documenting the project. 

### LaTeX Further Reading
- [Beginners Tutorial](https://www.dante.de/tex/TeXAnfaenger.html)
- [LaTeX for Windows](https://www.miktex.org)
- [LaTeX for Mac](http://www.tug.org/mactex/)

### Project and Source Control

#### Avoiding Clutter with .gitignore
Gitignore files allow to exclude certain patterns from being versioned.
This is necessary to avoid unnecessary (and possibly harmful) cluttering of your repository.
Especially the automatically generated project and cache files of VisualStudio, Unity, or Unreal projects should be ignored.

You can find [a selection of *.gitignore* files publicly available on GitHub](https://github.com/github/gitignore).

##### Quick Check if .gitignore is working

Your *.gitignore* is not correctly set up, if
* your repository contains Folders such as `Library`, `DerivedDataCache` or `Saved`
* `cache` files, `visual studio` project files etc. are `shown as modified` before commiting with your git client

In this case, check your setup.
Be aware that *.gitignore* is the actual, required filename!


#### Versioning Binary Assets with Git LFS and .gitattributes
Gitattribute files define file types to be handled through the Git Large File Storage (Git LFS) System.
Git, in a nutshell, calculates differences between two versions of a file based on line-wise comparisons.
This system is not meant to (and does not handle well) binary files, such as assets, images, meshes, etc.
Even minimal changes might add the whole size of the file to the projects history.
Git LFS identifies iterations of binary files using a hash in the repository, but stores the actual binary data transparently in a seperate data silo.

To let Git LFS track a certain file (e.g. recursively all *.jpg*), execute this command:

	> git lfs track *.jpg

This command creates the following entry in the *.gitattributes* file:

	*.jpg filter=lfs diff=lfs merge=lfs -text


Git LFS is installed on all Workstations in E37 and the GameLabs.
For your private computer, you can [download Git LFS here](https://git-lfs.github.com/).


#### Further Reading: 
* [Epic on Git for Unreal](https://wiki.unrealengine.com/Git_source_control_(Tutorial)#Workarounds_for_dealing_with_binary_files_on_your_Git_repository)
* [GitLFS](https://www.git-lfs.com)
* [Git](https://www.git-scm.com)