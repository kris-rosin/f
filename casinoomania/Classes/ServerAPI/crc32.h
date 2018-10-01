
class crc32
{
protected:
	unsigned table[256];
public:
	unsigned m_crc32;
	crc32();
	void ProcessCRC(void * pData, int nLen);
    void ProcessFileCRC(const char * path);
};
