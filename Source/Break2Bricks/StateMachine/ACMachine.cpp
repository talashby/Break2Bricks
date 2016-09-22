// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMachine.h"

static const FName g_ssStateNone("None");
static const FName g_ssStateStart("Start");
static const FName g_ssStateError("Error");

CACMachine::CACMachine(const FString &sName)
{
    //UE_LOG(YourLog, Warning, TEXT("This is a message to yourself during runtime!"));
    m_iLogSeverityLevel = ELogVerbosity::Verbose;
    m_sNameMachine = sName;
    REGISTER_ACSTATE(CACMachine, Start);
    REGISTER_ACSTATE(CACMachine, Error);

    m_sState = g_ssStateNone;
    m_fnCurrentState = (fnACStateHandler)&CACMachine::TickStateNone;
    m_bRestart = false;
    //SetNextState(g_ssStateStart);
}

void CACMachine::TickPublic()
{
    OperateStates();
    Tick();
    TickHierarchical();
}

bool CACMachine::IsInNoneState() const
{
    return IsCurrentState(g_ssStateNone);
}

bool CACMachine::IsInStartState() const
{
    return IsCurrentState(g_ssStateStart);
}

bool CACMachine::IsInErrorState() const
{
    return g_ssStateError == m_sState;
}

void CACMachine::OperateStates()
{
    FName sNextState;
    if (m_bRestart)
    {
        sNextState = g_ssStateStart;
        m_bRestart = false;
    }
    else
    {
        sNextState = (this->*(m_fnCurrentState))(TICK_StateNormal);
    }
    bool bStateChanged = SetNextState(sNextState);
    while (bStateChanged)
    {
        (this->*(m_fnCurrentState))(TICK_StateFinished);
        m_aStatesArchive.push_back(m_sNextState);
        if (m_aStatesArchive.size() > 20)
        {
            m_aStatesArchive.pop_front();
        }
        if (LogHelper::CheckLogLevel(m_iLogSeverityLevel))
        {
            FString str = "Finished state: '" + m_sState.ToString() + "'. Start state: '" + m_sNextState.ToString() + "'";
            WriteLogMessage(m_iLogSeverityLevel, TCHAR_TO_ANSI(*str));
        }
        m_sLastState = m_sState;
        m_sState = m_sNextState;
        m_fnCurrentState = m_fnNextState;
        BeforeTickStateStartedHierarchical();
        BeforeTickStateStarted();
        FName sNextState2 = (this->*(m_fnCurrentState))(TICK_StateStarted);
        bStateChanged = SetNextState(sNextState2);
    }
}

bool CACMachine::IsCurrentState(const FName &sStateName) const
{
    return m_sState == sStateName;
}

bool CACMachine::IsLastState(const FName &sStateName) const
{
    return m_sLastState == sStateName;
}

bool CACMachine::IsNextState(const FName &sStateName) const
{
    return m_sNextState == sStateName;
}

void CACMachine::SetNextErrorState(const FString& sMessage/* = ""*/)
{
    WriteLogMessage(ELogVerbosity::Error, "Error. Switching to Error state at state \"%s\" with message \"%s\"", TCHAR_TO_ANSI(*m_sState.ToString()), TCHAR_TO_ANSI(*sMessage));
    SetNextState(g_ssStateError);
}

FName CACMachine::TickStateNone(int iTickType)
{
    return g_ssStateStart;
}

FName CACMachine::GetStateStartName() const
{
    return g_ssStateStart;
}

FName CACMachine::ErrorState(const FString &sMessage)
{
    WriteLogMessage(ELogVerbosity::Error, "Error. Switching to Error state at state \"%s\" with message \"%s\"", TCHAR_TO_ANSI(*m_sState.ToString()), TCHAR_TO_ANSI(*sMessage));
    WriteStatesArchiveToErrorLog();
    m_sErrorMessage = sMessage;
    return g_ssStateError;
}

void CACMachine::WriteStatesArchiveToErrorLog() const
{
    FString str = "States archive: ";
    std::list<FName>::const_iterator it = m_aStatesArchive.begin();
    while (it != m_aStatesArchive.end())
    {
        str += it->ToString();
        ++it;
        if (it != m_aStatesArchive.end())
        {
            str += " -> ";
        }
    }
    WriteLogMessage(ELogVerbosity::Error, TCHAR_TO_ANSI(*str));
}

FString CACMachine::GetLogPrefix() const
{
    FString prefix = "ACMachine : " + m_sNameMachine + "(" + m_sState.ToString() + ") : ";
    return prefix;
}

FString CACMachine::GetName() const
{
    return m_sNameMachine;
}

void CACMachine::SetName(const FString &sName)
{
    m_sNameMachine = sName;
}

void CACMachine::SetStateExternalSignal(int iExternalSignal)
{
    WriteLogMessage(m_iLogSeverityLevel, "External signal was set : %d", iExternalSignal);
    m_iStateExternalSignal = iExternalSignal;
}

bool CACMachine::IsStateExternalSignal(int iExternalSignal) const
{
    return iExternalSignal == m_iStateExternalSignal;
}

void CACMachine::SetLogSeverity(int iLogSeverityLevel)
{
    m_iLogSeverityLevel = iLogSeverityLevel;
}

bool CACMachine::RegisterState(const FName& sStateName, fnACStateHandler pStateHandler)
{
    return m_aStateMap.insert(tStateMap::value_type(sStateName, pStateHandler)).second;
}

bool CACMachine::UnregisterState(const FName& sStateName)
{
    return m_aStateMap.erase(sStateName) == 1;
}

bool CACMachine::SetNextState(const FName& sStateTo)
{
    if (!sStateTo.IsNone())
    {
        tStateMap::const_iterator itStateMap = m_aStateMap.find(sStateTo);
        if (itStateMap != m_aStateMap.end())
        {
            m_fnNextState = itStateMap->second;
            m_sNextState = sStateTo;
        }
        else
        {
            SetNextErrorState(FString::Printf(TEXT("Failed to switch to state \"%s\""), *sStateTo.ToString()));
            m_sNextState = g_ssStateError;
        }
        return true;
    }
    return false;
}

void CACMachine::WriteLogMessage(int iLevel, const char *sFormat, ...) const
{
    va_list args;
    va_start(args, sFormat);

    char sCommunicatorMessage[2048];
    vsnprintf(sCommunicatorMessage, 2048, sFormat, args);

    va_end(args);

    //WRITE_LOG_MESSAGE(iLevel, 0, "%s%s", GetLogPrefix().c_str(), sCommunicatorMessage);
}

void CACMachine::BeforeTickStateStartedHierarchical()
{
    m_iStateExternalSignal = EXTERNAL_SIGNAL_None;
}

void CACMachine::Restart()
{
    m_bRestart = true;
    OperateStates();
}
