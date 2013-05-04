#include "VDFS.hpp"

#include <fstream>
#include <iostream>

namespace vdfs {

template <typename T>
std::ifstream& rawRead(std::ifstream& stream, T& dest)
{
	stream.read(reinterpret_cast<char*>(&dest), sizeof(T));
	return stream;
}

bool File::openFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	
	if (!file)
	{
		std::cerr << "Failed to open " << filename << " for reading.\n";
		return false;
	}
	
	if (!rawRead(file, header))
	{
		std::cerr << "Failed reading header file.\n";
		return false;
	}
	
	if (header.entrySize != 80)
	{
		std::cerr << "Entry size is assumed to be 80.\n";
		return false;
	}
	
	table = new Entry[header.entryCount];
	file.seekg(header.tableOffset);
	if (!file.read(reinterpret_cast<char*>(table), sizeof(Entry) * header.entryCount))
	{
		std::cerr << "Failed reading entry table.\n";
		delete[] table;
		return false;
	}
	
	return true;
}

const Entry& File::getEntry(word index)
{
	return *(table + index);
}

}
