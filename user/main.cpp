#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "GWState.hpp"
#include "GameUtility.hpp"
#include "D3D11Hooking.hpp"
#include "detours.h"
#include "GWGUI.hpp"

using namespace app;

extern const LPCWSTR LOG_FILE = L"GhostWareAU-log.txt";

IDXGISwapChain* SwapChain;
ID3D11Device* Device;
ID3D11DeviceContext* Ctx;
ID3D11RenderTargetView* RenderTargetView;

D3D_PRESENT_FUNCTION OriginalD3DFunction;
WNDPROC OriginalWndProcFunction;
void (*OriginalKeyboardJoystick_Update)(KeyboardJoystick*, MethodInfo*);
void (*OriginalMeetingHud_Update)(MeetingHud*, MethodInfo*);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(GWState::Window, &mPos);

	ImGui::GetIO().MousePos.x = mPos.x;
	ImGui::GetIO().MousePos.y = mPos.y;

	if (uMsg == WM_KEYUP && wParam == VK_DELETE)
		GWState::ShowMenu = !GWState::ShowMenu;

	if (GWState::ShowMenu)
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProcW(OriginalWndProcFunction, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall D3D_FUNCTION_HOOK(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags)
{
	if (!GWState::ImGuiInitialized)
	{
		SwapChain = pThis;
		pThis->GetDevice(__uuidof(ID3D11Device), (void**)&Device);
		Device->GetImmediateContext(&Ctx);

		DXGI_SWAP_CHAIN_DESC desc;

		pThis->GetDesc(&desc);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		GWState::Window = desc.OutputWindow;

		ImGui_ImplWin32_Init(GWState::Window);
		ImGui_ImplDX11_Init(Device, Ctx);

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		OriginalWndProcFunction = (WNDPROC)SetWindowLongW(GWState::Window, GWLP_WNDPROC, (LONG)WndProcHook);

		ImGui::GetIO().ImeWindowHandle = GWState::Window;

		ID3D11Texture2D* pBackBuffer;
		pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
		pBackBuffer->Release();

		ImGui::GetStyle().WindowRounding = 0.0F;
		ImGui::GetStyle().ChildRounding = 0.0F;
		ImGui::GetStyle().FrameRounding = 0.0F;
		ImGui::GetStyle().GrabRounding = 0.0F;
		ImGui::GetStyle().PopupRounding = 0.0F;
		ImGui::GetStyle().ScrollbarRounding = 0.0F;

		GWState::ImGuiInitialized = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GWState::ShowMenu)
		GWGUI::RenderMenu(&GWState::ShowMenu);

	ImGui::EndFrame();
	ImGui::Render();

	Ctx->OMSetRenderTargets(1, &RenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return OriginalD3DFunction(pThis, SyncInterval, Flags);
}

void DetouredKeyboardJoystick_Update(KeyboardJoystick* __this, MethodInfo* method)
{
	if (GetGameState() != 2) {
		GWState::MurderTarget = std::nullopt;
		GWState::VoteTarget = std::nullopt;
		GWState::KickTarget = std::nullopt;
	}
	else {
		if (GWState::KickTarget.has_value()) {
			VoteBanSystem* vbSystem = (*VoteBanSystem__TypeInfo).static_fields->Instance;
			InnerNetClient* net = (InnerNetClient*)(*AmongUsClient__TypeInfo).static_fields->Instance;

			int32_t targetId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)GWState::KickTarget.value(), NULL);
			int32_t selfId = net->fields.ClientId;

			for (auto player : GetAllPlayers()) {
				net->fields.ClientId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)player, NULL);
				VoteBanSystem_CmdAddVote(vbSystem, targetId, NULL);
			}

			net->fields.ClientId = selfId;
			GWState::KickTarget = std::nullopt;
		}

		if (GWState::MurderTarget.has_value()) {
			for (auto player : GetAllPlayers()) {
				if (GetPlayerData(player)->fields.LODLBBJNGKB && !GetPlayerData(player)->fields.DMFDFKEJHLH) {
					PlayerControl_RpcMurderPlayer(player, GWState::MurderTarget.value(), NULL);
					break;
				}
			}
			GWState::MurderTarget = std::nullopt;
		}
	}

	OriginalKeyboardJoystick_Update(__this, method);
}

void DetouredMeetingHud_Update(MeetingHud* __this, MethodInfo* method)
{
	MeetingHud_IONNOOOEADE__Enum state = (MeetingHud_IONNOOOEADE__Enum)((intptr_t)__this->fields.VoteEndingSound);

	if (GWState::VoteTarget.has_value() && (state == 1 || state == 2))
	{
		auto targetData = GetPlayerData(GWState::VoteTarget.value());
		auto targetName = GetUTF8StringFromNETString(targetData->fields.EKHEPECKPKK);

		for (auto player : GetAllPlayers()) {
			auto playerData = GetPlayerData(player);
			auto playerName = GetUTF8StringFromNETString(playerData->fields.EKHEPECKPKK);

			if (!MeetingHud_DidVote(__this, playerData->fields.FIOIBHIDDOC, NULL) && !playerData->fields.DMFDFKEJHLH) {
				MeetingHud_CmdCastVote(__this, playerData->fields.FIOIBHIDDOC, targetData->fields.FIOIBHIDDOC, NULL);
				break;
			}
		}

		if (MeetingHud_GetVotesRemaining(__this, NULL) == 0)
			GWState::VoteTarget = std::nullopt;
	}

	OriginalMeetingHud_Update(__this, method);
}

bool HookFunction(PVOID* original, PVOID detour, const char* funcName)
{
	LONG err = DetourAttach(original, detour);
	if (err != 0)
	{
		std::cout << "Failed to hook a function: " << funcName << "\n";
		return false;
	}
	else
	{
		std::cout << "Successfully hooked a function: " << funcName << "\n";
		return true;
	}
}

void Run()
{
	NewConsole();

	std::cout << "Initializing..." << std::endl;

	OriginalKeyboardJoystick_Update = KeyboardJoystick_Update;
	OriginalD3DFunction = GetD3D11PresentFunction();
	OriginalMeetingHud_Update = MeetingHud_Update;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (!HookFunction(&(PVOID&)OriginalKeyboardJoystick_Update, DetouredKeyboardJoystick_Update, "KeyboardJoystick_Update"))
		return;

	if (!HookFunction(&(PVOID&)OriginalMeetingHud_Update, DetouredMeetingHud_Update, "MeetingHud_Update"))
		return;

	if (OriginalD3DFunction == NULL || !HookFunction(&(PVOID&)OriginalD3DFunction, D3D_FUNCTION_HOOK, "D3D11Present"))
		return;

	DetourTransactionCommit();

	std::cout << "Initialization Complete" << std::endl;
}