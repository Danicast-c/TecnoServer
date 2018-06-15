# TecnoServer

Para utilizar esta comunicación de servidor de usa json-c el cual se puede encontrar en https://github.com/json-c/json-c

Una vez obtenido los archivos necesarios para json-c, se coloca en la carpeta donde está el main se agrega los siguientes comandos al CMakeLists.txt

  add_compile_options("-L/lib -ljson-c")
  
  link_libraries("-L/lib -ljson-c")
