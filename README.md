# Introduction

Soon...

# Using the Library

Soon...

### Examples

Soon...

## Development HOWTO's

### Local Build 
```
# clone repository and build locally
[user@host] git clone https://github.com/andreclinio/mcpserver.git
[user@host] cd mcpserver
[user@host] make rebuild
...
yada yada yada
...

# list results
[user@host] ls include lib
include:
mcprequesttype.hpp      mcproute.hpp            mcprouteinfo.hpp        mcpserver.hpp           version.hpp

lib:
libmcpserver.a
``` 


### Version Release 
```
[user@host] git status 
Your branch is up to date with 'origin/master'.

[user@host] ./release.bash
<enter the data>
...
yada yada yada
...
```

### Install for Conan local repository
```
# check existing versions
[user@host] conan search mcpserver
Existing package recipes:
mcpserver/0.0.1-snapshot

# remove old versions (if desired)
[user@host] conan remove mcpserver/0.0.1-snapshot

# build new version locally
[user@host] git checkout tags/0.0.2
[user@host] make conan-install-local
...
yada yada yada
...

# check new version
[user@host] conan search mcpserver
Existing package recipes:
mcpserver/0.0.2

```