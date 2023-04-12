// Battlefield 4 External.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "includes.h"
#include "overlay.h"
HWND hwnd;
bool Run = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

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
	std::cout << "[+] Handle grabbed..\n";
	//En üstde saydam bir overlay oluþturuyoruz.
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, "MyOverlay", LoadIcon(nullptr, IDI_APPLICATION)};
	RegisterClassExA(&wc);
	hwnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, 0, NULL); // ToDo : Ayarlanabilir çözünürlük eklenicek.
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margin);
	std::cout << "[+] Overlay created..\n";
	// Direct3D device kurulumu gerçekleþtirdik.
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	// Overlayý gösteriyoruz
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Dear ImGui kuruyoruz
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;

	// Dear ImGui temasýný seçiyoruz
	ImGui::StyleColorsDark();

	// Render için directX backend seçiyoruz
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	std::cout << "[+] Imgui is ready..\n";
	ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.f);

	// Main loop
	while (Run)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				Run = false;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		// Rendering
		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClassA(wc.lpszClassName, wc.hInstance);
	return 1;
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
