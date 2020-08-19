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
   file="library/include/version.hpp"

   currVersion=`grep MCP_LIBRARY_VERSION $file | sed "s/.*\"\(.*\)\"/\1/g"`
   nextTag=`echo $currVersion | sed "s/-snapshot//1"`
   nextTag="$nextTag"

   echo "Versão corrente: $currVersion"
   echo "Tag a ser criada: $nextTag"

   get_next_version

   nextVersion=$nextVersion-snapshot
   echo "Próxima versão com sufixo: $nextVersion"

   sed -i '' "s/$currVersion/$nextTag/1" $file
   git add $file
   git commit -m "Commit da versão para a tag $nextTag"
   git push 
   git tag -a $nextTag -m 'Criação da tag $nextTag'
   git push origin $nextTag
   sed -i '' "s/$nextTag/$nextVersion/1" $file
   git add $file
   git commit -m "Segundo commit para a próxima versão $nextVersion"
   git push 
}

main
