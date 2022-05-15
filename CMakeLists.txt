cmake_minimum_required(VERSION 3.12)

project(Parallel_Huffman)

set(SOURCES 
	main.cpp
	Parallel_Huffman.cpp
	Archiever.cpp
	Decoder.cpp
	Utils.cpp
)

set(HEADERS
	Parallel_Huffman.h
	Archiever.h
	Decoder.h
	Utils.h
)

add_executable(version ${SOURCES} ${HEADERS})
set_target_properties(
	version PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)