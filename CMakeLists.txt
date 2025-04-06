cmake_minimum_required(VERSION 3.15)

project(KFS VERSION 0.2
	DESCRIPTION "kfs archive system"
	LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 90) # ANSI C MY BELOVED

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(kfs 
	src/main.c 
	src/kfs2.c
)

add_executable(kfs3 
	src/main3.c 
	src/kfs3.c
)

add_executable(kfs4
	src/kfs4.c
)

if(WIN32)
	add_executable(win WIN32
		src/winmain.c 
		src/kfs2.c
	)
ENDIF()