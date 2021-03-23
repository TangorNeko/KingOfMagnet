#include "stdafx.h"
#include "tkmFileManager.h"
#include "tkFile/TkmFile.h"

tkmFileManager* tkmFileManager::m_instance = nullptr;

TkmFile* tkmFileManager::GetTkmFile(const char* filepath)
{
	auto it = m_tkmMap.find(filepath);

	if (it != m_tkmMap.end())
	{
		return it->second;
	}
	else
	{
		TkmFile* newtkm = new TkmFile;
		
		newtkm->Load(filepath);

		auto it = m_tkmMap.insert(std::make_pair(filepath, newtkm));
		return it.first->second;
	}
}