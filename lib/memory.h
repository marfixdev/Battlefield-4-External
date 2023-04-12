#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
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
}m;