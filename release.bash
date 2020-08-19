# /bin/bash

function get_next_version() {
   local x
   until [ "$x" != "" ]; do
     echo "Digite a próxima versão (sem sufixo snapshot):"
     read x
   done
   export nextVersion="$x"
   echo "Proxíma versão: $nextVersion"
}

function main() {
   include_file="include/version.hpp"
   conan_file="conanfile.py"

   currVersion=`grep MCP_LIBRARY_VERSION $include_file | sed "s/.*\"\(.*\)\"/\1/g"`
   nextTag=`echo $currVersion | sed "s/-snapshot//1"`
   nextTag="$nextTag"

   echo "Versão corrente no fonte: $currVersion"
   echo "Tag a ser criada: $nextTag"

   get_next_version

   nextVersion=$nextVersion-snapshot
   echo "Próxima versão com sufixo: $nextVersion"

   sed -i "s/version = \"\(.*\)\"/version = \"$nextTag\"/1" $conan_file
   sed -i "s/$currVersion/$nextTag/1" $include_file
   #git add $include_file
   #git add $conan_file
   #git commit -m "Commit da versão para a tag $nextTag"
   #git push 
   #git tag -a $nextTag -m 'Criação da tag $nextTag'
   #git push origin $nextTag
   sed -i "s/$nextTag/$nextVersion/1" $include_file
   sed -i "s/$nextTag/$nextVersion/1" $conan_file
   #git add $include_file
   #git add $conan_file
   #git commit -m "Segundo commit para a próxima versão $nextVersion"
   #git push 
}

main
