#include "StateManager.h"

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> StateManager::m_StateData;

const std::string StateManager::m_ObjectEnd{ "<end>" };
const std::string StateManager::m_ObjectName{ ":" };
const std::string StateManager::m_EndLine{ ";" };
const std::string StateManager::m_VarLim{ "=" };

void StateManager::LoadState()
{
	std::ifstream file("src/system/state/save.atlas", std::ios::in);
	std::string fline;
    std::string contents;

    while (file >> fline) {
        contents += fline;
    }

    if (contents.empty())
    {
        ATLAS_TRACE("Save state file is empty!");
        file.close();
        return;
    }

	std::string Data = Decrypt(contents);

	size_t objectPos;
	size_t objectNamePos;
	size_t varLinePos;

    std::string obj;
    std::string objName;
    std::string line;
    std::string varName;
    std::string varValue;

    while ((objectPos = Data.find(m_ObjectEnd)) != std::string::npos)
    {
        obj = Data.substr(0, objectPos);
        obj.pop_back();
        objectNamePos = obj.find(m_ObjectName);
        objName = obj.substr(0, objectNamePos);
        obj.erase(0, objectNamePos + m_ObjectName.length() + 1);

        while ((varLinePos = obj.find(m_EndLine)) != std::string::npos)
        {
            line = obj.substr(0, varLinePos);
            varName = obj.substr(0, line.find(m_VarLim));
            varValue = line.erase(0, line.find(m_VarLim) + 1);
            obj.erase(0, varLinePos + m_EndLine.length() + 1);

            m_StateData[objName].emplace(varName, varValue);
        }

        Data.erase(0, objectPos + m_ObjectEnd.length() + 1);
    }

    file.close();

    ATLAS_INFO("State settings loaded!");
}

void StateManager::SaveState()
{
    ATLAS_INFO("Saving current state....");

    std::ofstream file("src/system/state/save.atlas", std::ios::trunc);

    std::string Data;

    for (auto& Obj : m_StateData)
    {
        Data += Obj.first + m_ObjectName + "\n";

        for (auto& Var : Obj.second)
        {
            Data += Var.first + m_VarLim + Var.second + m_EndLine + "\n";
        }
        Data += m_ObjectEnd + "\n";
    }

    if (Data.empty())
    {
        ATLAS_TRACE("Nothing to save!");
        file.close();
        return;
    }

    std::string Contents = Encrypt(Data);

    file << Contents;
    file.close();

    ATLAS_INFO("State saved!");
}

void StateManager::NewLog(LogData Data)
{
    std::chrono::year_month_day YMD{ std::chrono::floor<std::chrono::days>(Data.Time) };
    std::ofstream file("src/system/state/logs/" + std::format("{:%Y-%m-%d}", YMD) + ".atlas", std::ios::app);

    std::string log{ "[" + std::format("{:%T}", std::chrono::floor<std::chrono::seconds>(Data.Time)) + "]\n>> " + Data.Input + "\n<< " + Data.Output + "\n\n" };

    file << log;
    file.close();
}

void StateManager::QueueObjectItem(std::string& ObjName, std::string VarName, std::string VarValue)
{
    m_StateData[ObjName].emplace(VarName, VarValue);
}
