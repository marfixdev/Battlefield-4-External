// Battlefield 4 External.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Includes.h"
HWND hwnd;
int main()
{
	DWORD PID = 0;
	HWND Ghwnd = FindWindowA(NULL, "Battlefield 4");

	if (!Ghwnd) //Oyun açýk mý diye kontrol edelim.
	{
		MessageBox(NULL, TEXT("Battlefield 4 bulunamadý! Lütfen oyunu açýp tekrar deneyinz."), NULL, MB_ICONWARNING | MB_OK);
		std::cout << "[-]Process not found ! \n";
		return 0;
	}
	GetWindowThreadProcessId(Ghwnd, &PID); // Oyunun processid alalým.
	std::cout << "[+]Process found : " << std::dec << PID << "\n";

	m.pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (!m.pHandle) // Handle alabildik mi kontrol ediyoruz.
	{
		MessageBox(NULL, TEXT("Handle alýnamadý!"), NULL, MB_ICONWARNING | MB_OK);
		return 0;
	}

	//En üstde saydam bir overlay oluþturuyoruz.
	WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), 0, WndProc, 0, 0, NULL, NULL, NULL, NULL, "GG", "WP", NULL };
	RegisterClassExA(&wc);
	hwnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL); // ToDo : Ayarlanabilir çözünürlük eklenicek.
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margin);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	std::cout << "[+] Handle grabbed..\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
