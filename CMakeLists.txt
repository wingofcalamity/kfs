cmake_minimum_required(VERSION 3.15)

project(KFS VERSION 0.2
	DESCRIPTION "kfs archive system"
	LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 90)

add_executable(kfs 
	src/main.c 
	src/kfs2.c
)

if(WIN32)
	add_executable(win WIN32
		src/winmain.c 
		src/kfs2.c
	)
ENDIF()