cmake_minimum_required(VERSION 3.14)
project(RubricaTelefonica VERSION 1.0 LANGUAGES CXX)

# Imposta lo standard C++26 (per SQLiteCpp serve minimo C++17, ma altre funzioni nel codice richiedono versioni più recenti)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#---------------
# 1. Dipendenze
#---------------
# Trova la libreria SQLite3 instllata nel sistema
# Se non c'è, va prima installata col comando: sudo apt install libsqlite3-dev

find_package(SQLite3 REQUIRED)

# Poi scarica ed integra SQLiteCpp direttamente tramite FetchContent
include(FetchContent)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG 3.4.0
    )

# Disabilita la compilazione interna di SQLite3 in SQLiteCpp
set(SQLITECPP_INTERNAL_SQLITE OFF CACHE BOOL "" FORCE)

# Configura le opzioni di SQLiteCpp prima del caricamento
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SQLiteCpp)

#---------------
# 2. Target del progetto
#---------------
# Include i file sorgente del progetto
add_executable(Rubrica
    src/main.cpp
    src/Contact.cpp
    src/DatabaseManager.cpp
    )

# Aggiunge la cartella include/ per i file .h
target_include_directories(Rubrica PRIVATE include)

# Collega le librerie (SQLiteCpp e SQLite3) all'eseguitile
target_link_libraries(Rubrica PRIVATE
    SQLiteCpp
    ${SQLITE3_LIBRARIES}
    )
