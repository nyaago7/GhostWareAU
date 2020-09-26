#pragma once

#include "pch-il2cpp.h"

#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "il2cpp-appdata.h"
#include "GameUtility.hpp"
#include "StringUtility.hpp"
#include "GWState.hpp"

namespace GWGUI
{
	void RenderMenu(bool* state);
}