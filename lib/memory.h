#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
class Memory
{
public:
	HANDLE pHandle;

	template <typename T>
	constexpr const T RPM(const uintptr_t& adder) const noexcept
	{
		T v = { };
		ReadProcessMemory(pHandle, reinterpret_cast<const void*>(adder), &v, sizeof(T), NULL);
		return v;
	}

	template <typename T>
	constexpr void WPM(const uintptr_t& adder, const T& v) const noexcept
	{
		WriteProcessMemory(pHandle, reinterpret_cast<void*>(adder), &v, sizeof(T), NULL);
	}
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
			addr += offsets[i];
		}
		return addr;
	}
	HMODULE GetModuleBaseAddress(const char* toFind, DWORD pid)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		HMODULE ret = nullptr;
		if (hSnapshot)
		{
			MODULEENTRY32 me32{};
			me32.dwSize = sizeof(me32);

			if (Module32First(hSnapshot, &me32))
			{
				do
				{
					// Debug.
					//printf("%s\n", me32.szModule);

					if (!strcmp(me32.szModule, toFind))
					{
						ret = me32.hModule;
						break;
					}

				} while (Module32Next(hSnapshot, &me32));
			}

			CloseHandle(hSnapshot);
		}

		return ret;
	}
}m;