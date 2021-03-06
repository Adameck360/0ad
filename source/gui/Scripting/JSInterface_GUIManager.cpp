/* Copyright (C) 2020 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"

#include "JSInterface_GUIManager.h"

#include "gui/CGUI.h"
#include "gui/GUIManager.h"
#include "gui/ObjectBases/IGUIObject.h"
#include "ps/GameSetup/Config.h"
#include "scriptinterface/FunctionWrapper.h"
#include "scriptinterface/ScriptInterface.h"

namespace JSI_GUIManager
{
// Note that the initData argument may only contain clonable data.
// Functions aren't supported for example!
void PushGuiPage(const ScriptInterface& scriptInterface, const std::wstring& name, JS::HandleValue initData, JS::HandleValue callbackFunction)
{
	g_GUI->PushPage(name, scriptInterface.WriteStructuredClone(initData), callbackFunction);
}

void SwitchGuiPage(ScriptInterface::CmptPrivate* pCmptPrivate, const std::wstring& name, JS::HandleValue initData)
{
	g_GUI->SwitchPage(name, pCmptPrivate->pScriptInterface, initData);
}

void PopGuiPage(ScriptInterface::CmptPrivate* pCmptPrivate, JS::HandleValue args)
{
	if (g_GUI->GetPageCount() < 2)
	{
		ScriptRequest rq(pCmptPrivate->pScriptInterface);
		ScriptException::Raise(rq, "Can't pop GUI pages when less than two pages are opened!");
		return;
	}

	g_GUI->PopPage(pCmptPrivate->pScriptInterface->WriteStructuredClone(args));
}

std::wstring SetCursor(const std::wstring& name)
{
	std::wstring old = g_CursorName;
	g_CursorName = name;
	return old;
}

void ResetCursor()
{
	g_CursorName = g_DefaultCursor;
}

bool TemplateExists(const std::string& templateName)
{
	return g_GUI->TemplateExists(templateName);
}

CParamNode GetTemplate(const std::string& templateName)
{
	return g_GUI->GetTemplate(templateName);
}


void RegisterScriptFunctions(const ScriptRequest& rq)
{
	ScriptFunction::Register<&PushGuiPage>(rq, "PushGuiPage");
	ScriptFunction::Register<&SwitchGuiPage>(rq, "SwitchGuiPage");
	ScriptFunction::Register<&PopGuiPage>(rq, "PopGuiPage");
	ScriptFunction::Register<&SetCursor>(rq, "SetCursor");
	ScriptFunction::Register<&ResetCursor>(rq, "ResetCursor");
	ScriptFunction::Register<&TemplateExists>(rq, "TemplateExists");
	ScriptFunction::Register<&GetTemplate>(rq, "GetTemplate");

	ScriptFunction::Register<&CGUI::FindObjectByName, &ScriptFunction::ObjectFromCBData<CGUI>>(rq, "GetGUIObjectByName");
	ScriptFunction::Register<&CGUI::SetGlobalHotkey, &ScriptFunction::ObjectFromCBData<CGUI>>(rq, "SetGlobalHotkey");
	ScriptFunction::Register<&CGUI::UnsetGlobalHotkey, &ScriptFunction::ObjectFromCBData<CGUI>>(rq, "UnsetGlobalHotkey");
}
}
