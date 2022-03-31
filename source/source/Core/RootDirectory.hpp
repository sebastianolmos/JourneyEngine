/*
	This field was automatically created with CMake please don't modify it
*/
#pragma once
#ifndef ROOTDIRECTORY_HPP
#define ROOTDIRECTORY_HPP
#include <filesystem>

namespace Journey {
	static const char * const source_directory = "D:/9no/motores/repos/JourneyEngine/source/";
	static std::filesystem::path SourcePath(const std::string &relative_path){
		return source_directory + relative_path;
	}
	
}


#endif
