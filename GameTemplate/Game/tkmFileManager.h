#pragma once
class TkmFile;

class tkmFileManager
{
private:
	tkmFileManager() = default;
	~tkmFileManager() = default;

	static tkmFileManager* m_instance;

	std::map<const char*, TkmFile*> m_tkmMap;

public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new tkmFileManager;
		}
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static tkmFileManager* GetInstance()
	{
		return m_instance;
	}

	TkmFile* GetTkmFile(const char* filepath);
};

