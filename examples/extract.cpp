#include "../VDFS.hpp"

#include <iostream>
#include <fstream>

std::string getFilename(const char* name)
{
	int i;
	for (i = 0; *(name + i) != ' '; ++i);
	return std::string(name, i);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Need a filename and index as arguments\n";
		return 1;
	}
	
	vdfs::File file;
	const char* filename = argv[1];
	
	if (!file.openFromFile(filename))
	{
		return 1;
	}
	
	vdfs::word index = std::stoul(argv[2]);
	
	auto entry = file.getEntry(index);
	
	std::ifstream in(filename);
	in.seekg(entry.offset);
	char* buffer = new char[entry.size];
	in.read(buffer, entry.size);
	std::ofstream out(getFilename(entry.name));
	out.write(buffer, entry.size);
	
}
