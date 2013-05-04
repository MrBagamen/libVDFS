#ifndef LIBVDFS_VDFS_HPP
#define LIBVDFS_VDFS_HPP

#include <cstdint>
#include <string>

namespace vdfs {

typedef uint_least32_t word;

struct Header {
	char comment[256];
	char versionString[12];
	char versionBytes[4];
	word entryCount;
	word fileCount;
	word timeStamp;
	word dataSize;
	word tableOffset;
	word entrySize;
};

struct Entry {
	char name[64];
	word offset;
	word size;
	word flags;
	word attrib;
};

class File {
public:
	bool openFromFile(const std::string& filename);
	const Entry& getEntry(word index);
	void* readDataOf(const Entry& entry);
	Header header;
private:
	Entry* table;
};

}

#endif // LIBVDFS_VDFS_HPP
