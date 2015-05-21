//-------------------------PlcBinaryFile.h-------------------------------------
#ifndef PLC_BINARY_FILE_H
#define PLC_BINARY_FILE_H

#include <fstream>
using namespace std;

#pragma warning(disable: 4244)

template <typename Record>
class PlcBinaryFile
{
public:
	PlcBinaryFile(ifstream &bin) : file(bin), offset(0)
	{
		bin.seekg(0, ios::end);
		size = bin.tellg();
		bin.seekg(0);
	}

	PlcBinaryFile(ifstream& bin, bool isBegin) : file(bin), offset(0)
	{
		bin.seekg(0, ios::end);
		size = bin.tellg();
		bin.seekg(0);

		if (!isBegin)
			offset = size;
	}

	PlcBinaryFile operator = (const PlcBinaryFile& other)
	{
		file.copyfmt(other.file);
		file.clear(other.file.rdstate());
		file.basic_ios<char>::rdbuf(other.file.rdbuf());
		offset = other.offset;
		size = other.size;
		currentRecord = other.currentRecord;
		return *this;
	}

	PlcBinaryFile& operator ++()
	{
		offset += sizeof(Record);
		return (*this);
	}

	PlcBinaryFile operator ++(int)
	{
		PlcBinaryFile plc = *this;
		++(*this);
		return plc;
	}

	PlcBinaryFile& operator --()
	{
		offset -= sizeof(Record);
		return (*this);
	}

	PlcBinaryFile operator --(int)
	{
		PlcBinaryFile plc = *this;
		--(*this);
		return plc;
	}

	Record const& operator * ()
	{
		if (offset >= 0)
		{
			file.seekg(offset);
			file.read((char*)&currentRecord, sizeof(currentRecord));
			return currentRecord;
		}
	}

	bool operator == (PlcBinaryFile const& it) const
	{
		return (&file == &(it.file) && offset == it.offset);
	}

	bool operator != (PlcBinaryFile const& it) const
	{
		return (&file != &(it.file) || offset != it.offset);
	}

private:
	ifstream &file;
	Record currentRecord;
	int offset;
	int size;
};

#endif