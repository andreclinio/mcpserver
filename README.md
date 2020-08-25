# Introduction

Soon...

# Using the Library

Some images and examples are better than full explanations...
Create project directory
```
[user@host:˜] mkdir myproj
[user@host:˜] cd myproj
```

Create a conan file (setting up the dependencies). See a simple example below.
```
[user@host:~/myproj] vi conanfile.txt
```

Create a directory to store generated files (no need to include in version control)
```
[user@host:~/myproj/] mkdir conan
[user@host:~/myproj/conan] cd conan
[user@host:~/myproj/conan] conan install ..
```

Analyze the the build info makefile that can be included in your makefile
```
[user@host:~/myproj/conan] more conanbuildinfo.mak 
```


### Conanfile simplest exmaple
```
[requires]
mcpserver/0.0.2

[generators]
make

[imports]
lib, *.lib* -> ./libs # Copies all dylib files from packages lib folder to my "bin" folder
```

## Development HOWTO's

### Local Build 
```
# clone repository and build locally
[user@host:˜] git clone https://github.com/andreclinio/mcpserver.git
[user@host:˜] cd mcpserver
[user@host:˜/mcpserver] make rebuild
...
yada yada yada
...

# list results
[user@host:~/mcpserver] ls include lib
include:
mcprequesttype.hpp      mcproute.hpp            mcprouteinfo.hpp        mcpserver.hpp           version.hpp

lib:
libmcpserver.a
``` 


### Version Release 
```
[user@host:~/mcpserver] git status 
Your branch is up to date with 'origin/master'.

[user@host:~/mcpserver] ./release.bash
<enter the data>
...
yada yada yada
...
```

### Install library inside Conan local repository
First, check existing versions
```
[user@host] conan search mcpserver
Existing package recipes:
mcpserver/0.0.1-snapshot
```

If wanted, remove old versions
```
[user@host] conan remove mcpserver/0.0.1-snapshot
```

For local install, build new version inside source directory. 
Choose (checkout) a desired tag and use the makefile for installation.
```
[user@host:~/mcpserver] git checkout tags/0.0.2
[user@host:~/mcpserver] make conan-install-local
```

Check if the new version is correctly installed.
```
[user@host] conan search mcpserver
Existing package recipes:
mcpserver/0.0.2
```
