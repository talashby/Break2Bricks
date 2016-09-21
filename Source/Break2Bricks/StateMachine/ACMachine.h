// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <list>
#include <set>
#include <vector>
/**
 * 
 */

class CACMachine;
typedef FString(CACMachine::*fnACStateHandler)(int);

class CACMachine
{
public:
    enum TICK
    {
        TICK_StateStarted = 1,
        TICK_StateNormal,
        TICK_StateFinished
    };

    CACMachine(const FString &sName);
    virtual ~CACMachine() {}

    void TickPublic(); // (not virtual, do not overload this function) call it to do state machine work
    bool IsInNoneState() const;
    bool IsInStartState() const;
    bool IsInErrorState() const;
    void WriteStatesArchiveToErrorLog() const;
    FString GetLogPrefix() const;
    FString GetName() const;

protected:
    enum EXTERNAL_SIGNAL
    {
        EXTERNAL_SIGNAL_None = 0,
        EXTERNAL_SIGNAL_1 = 1
    };

    virtual void Tick() {}
    virtual void TickHierarchical() {}
    virtual FString TickStateStart(int iTickType) = 0;
    virtual FString TickStateError(int iTickType) { return ""; }
    virtual void BeforeTickStateStarted() {} // used to clear state variables e.g. m_eStateExternalSignal
    virtual void BeforeTickStateStartedHierarchical(); // used to clear state variables e.g. m_eStateExternalSignal

    void Restart();
    void OperateStates(); // (not virtual, do not overload this function) it calls from TickPublic.

    bool IsCurrentState(const FString &sStateName) const;
    bool IsLastState(const FString &sStateName) const;
    bool IsNextState(const FString &sStateName) const;
    bool RegisterState(const FString& sStateName, fnACStateHandler pStateHandler);
    bool UnregisterState(const FString& sStateName);
    //bool IsStateRegistered(const hashstring& sStateName);
    FString GetStateStartName() const;
    FString ErrorState(const FString& sMessage = ""); // write log message and returns error state
    FString GetErrorMessage() const { return m_sErrorMessage; }

    void SetName(const FString &sName);
    void SetStateExternalSignal(int iExternalSignal); // used to switch states by external call. Automatically clear between states
    bool IsStateExternalSignal(int iExternalSignal) const;  //Check signal. Use in state function to switch state. How to use: call SetStateExternalSignal then call OperateStates

    void SetLogSeverity(int iLogSeverityLevel);

    void WriteLogMessage(int iLevel, const char *sFormat, ...) const;

private:
    bool SetNextState(const FString& sStateTo);
    void SetNextErrorState(const FString& sMessage = "");
    FString TickStateNone(int iTickType);
    //bool ExecuteState(int iTickType);

    typedef std::map<FString, fnACStateHandler> tStateMap;
    tStateMap m_aStateMap;

    std::list<FString> m_aStatesArchive;

    FString m_sState;
    FString m_sNextState;
    FString m_sLastState;

    fnACStateHandler m_fnCurrentState;
    fnACStateHandler m_fnNextState;
    FString m_sNameMachine;
    FString m_sErrorMessage;
    int m_iStateExternalSignal;
    int m_iLogSeverityLevel;
    bool m_bRestart;
};

#define REGISTER_ACSTATE(ClassName, StateName) { bool bRegistered = RegisterState(#StateName, (fnACStateHandler)&ClassName::TickState##StateName); (void)bRegistered; check(bRegistered); }


class ACMachineElement; typedef TSharedPtr<ACMachineElement> SP_ACMachineElement;
class CACMachine; typedef TSharedPtr<CACMachine> SP_ACMachine;

typedef std::set<ACMachineElement*> ElementsContainer;
typedef std::set<CACMachine*> MachinesContainer;
class ElementManagerBase; typedef std::set<ElementManagerBase*> ElementsRDContainer;
typedef std::vector<bool> BoolVector;

