#include "NativeFile.h"

using namespace VFS;
using namespace VFS::Files;

NativeFile::NativeFile(FILE* p_File) :
	m_File(p_File)
{
}

NativeFile::~NativeFile()
{
	if (m_File)
		fclose(m_File);

	m_File = nullptr;
}

size_t NativeFile::Write(const std::string& p_Data, int64_t p_Length /* = -1 */)
{
	if (!m_File)
		return 0;

	return fwrite(p_Data.data(), (size_t) (p_Length == -1 ? p_Data.size() : p_Length), 1, m_File);
}

std::string NativeFile::Read(size_t p_Length)
{
	if (!m_File)
		return nullptr;

	char* s_Buffer = new char[p_Length];
	size_t s_ReadSize = fread(s_Buffer, p_Length, 1, m_File);

	std::string s_StringBuffer(s_Buffer, s_ReadSize);

	delete [] s_Buffer;

	return s_StringBuffer;
}

int NativeFile::Scan(const char* p_Format, ...)
{
	if (!m_File)
		return 0;

	va_list s_Arguments;
	va_start(s_Arguments, p_Format);
	int s_Return = vfscanf(m_File, p_Format, s_Arguments);
	va_end(s_Arguments);

	return s_Return;
}

void NativeFile::Seek(int64_t p_Offset, int p_Origin)
{
	if (!m_File)
		return;

	fseek(m_File, (long) p_Offset, p_Origin);
}

void NativeFile::Close()
{
	if (!m_File)
		return;

	fclose(m_File);
	m_File = nullptr;
}

void NativeFile::Flush()
{
	if (!m_File)
		return;

	fflush(m_File);
}