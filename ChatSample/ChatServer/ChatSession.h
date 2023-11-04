#pragma once

//ChatSession.h
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <map>
#include <vector>
#include <string>

//II bring in IDL - generated interface definitions
#include <COMChat.h>
// this class models a particular chat session
class ChatSession : public IChatSession {
	friend class StatementEnumerator;
	friend class ChatSessionClass;
	LONG m_cRef;
	CRITICAL_SECTION m_csStatementLock;
	CRITICAL_SECTION m_csAdviseLock;
	OLECHAR m_wszSessionName[1024];
	bool m_bIsDeleted;
	bool m_bAllowAnonymousAccess;
	std::vector<std::wstring> m_statements;
	// each session maintains a linked list of LISTENERs that
		// represent the connected clients
	struct LISTENER {
		LISTENER* pNext;
		LISTENER* pPrev;
		OLECHAR* pwszUser;
		IChatSessionEvents* pItf;
	};
	LISTENER* m_pHeadListeners;
	// concurrency management helpers
	void SLock(void);
	void SUnlock(void);
	void ALock(void);
	void AUnlock(void);
	// security helpers
	bool CheckAccess(const OLECHAR* pwszUser);
	// Event notification helpers
	void Fire_OnNewStatement(const OLECHAR* pwszUser, const OLECHAR* pwszStatement);
	void Fire_OnNewUser(const OLECHAR* pwszUser);
	void Fire_OnUserLeft(const OLECHAR* pwszUser);
	//I constructor / destructor
	ChatSession(const OLECHAR* pwszSessionName, bool bAllowAnonymousAccess);
	virtual ~ChatSession(void);
	void Disconnect(void);
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	STDMETHODIMP_(ULONG) AddRef(void) override;
	STDMETHODIMP_(ULONG) Release(void) override;
	// IChatSession methods
	STDMETHODIMP get_SessionName(OLECHAR** ppwsz) override;
	STDMETHODIMP Say(const OLECHAR* pwszStatement) override;
	STDMETHODIMP GetStatements(IEnumString** ppes) override;
	STDMETHODIMP Advise(IChatSessionEvents* pEventSink, DWORD* pdw) override;
	STDMETHODIMP Unadvise(DWORD dwReg) override;
};

// this class enumerates the statements of a session
class StatementEnumerator : public IEnumString {
	friend class ChatSession;
	LONG m_cRef;
	ChatSession* m_pThis;
	std::vector<std::wstring>::iterator m_cursor;
	CRITICAL_SECTION m_csLock;
	//concurrency helpers
	void Lock(void);
	void Unlock(void);
	// constructor/destructor
	StatementEnumerator(ChatSession* pThis);
	virtual ~StatementEnumerator(void);
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	STDMETHODIMP_(ULONG) AddRef(void) override;
	STDMETHODIMP_(ULONG) Release(void) override;
	//IEnumString methods
	STDMETHODIMP Next(ULONG cElems, OLECHAR** rgElems, ULONG* pce)  override;
	STDMETHODIMP Skip(ULONG cElems) override;
	STDMETHODIMP Reset(void) override;
	STDMETHODIMP Clone(IEnumString** ppes) override;
};

// this class models the management of chat sessions
// and acts as the class object for CLSID_ChatSession
class ChatSessionClass : public IChatSessionManager,
	public IExternalConnection {
	friend class SessionNamesEnumerator;
	typedef std::map<std::wstring, ChatSession*> SESSIONMAP;
	LONG m_cStrongLocks;
	SESSIONMAP m_sessions;
	CRITICAL_SECTION m_csSessionLock;
	// concurrency helpers
	void Lock(void);
	void Unlock(void);
	// security helpers
	bool CheckAccess(const OLECHAR* pwszUser);
	// constructor / destructor
public:
	ChatSessionClass(void);
	virtual ~ChatSessionClass(void);
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	STDMETHODIMP_(ULONG) AddRef(void) override;
	STDMETHODIMP_(ULONG) Release(void) override;
	// IExternalConnection methods
	STDMETHODIMP_(DWORD) AddConnection(DWORD extconn, DWORD) override;
	STDMETHODIMP_(DWORD) ReleaseConnection(DWORD, DWORD, BOOL) override;
	// IChatSessionManager methods
	STDMETHODIMP GetSessionNames(IEnumString** ppes) override;
	STDMETHODIMP FindSession(const OLECHAR* pwszSessionName,
		BOOL bDontCreate,
		BOOL bAllowAnonymousAccess,
		IChatSession** ppcs) override;
	STDMETHODIMP DeleteSession(const OLECHAR* pwszSessionName) override;
};
// this class enumerates the session names of a server
class SessionNamesEnumerator : public IEnumString
{
	friend class ChatSessionClass;
	LONG m_cRef;
	std::vector<std::wstring>* m_pStrings;
	SessionNamesEnumerator* m_pCloneSource;
	std::vector<std::wstring>::iterator m_cursor;
	CRITICAL_SECTION m_csLock;
	// accessor function to get string vector
	std::vector<std::wstring>& Strings(void);
	// concurrency helpers
	void Lock(void);
	void Unlock(void);
	// constructor / destructor
	SessionNamesEnumerator(ChatSessionClass* pSessionClass);
	SessionNamesEnumerator(SessionNamesEnumerator* pCloneSource);
	virtual ~SessionNamesEnumerator(void);
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	STDMETHODIMP_(ULONG) AddRef(void) override;
	STDMETHODIMP_(ULONG) Release(void) override;
	// IEnumString methods
	STDMETHODIMP Next(ULONG cElems, OLECHAR** rgElems, ULONG* pce) override;
	STDMETHODIMP Skip(ULONG cElems) override;
	STDMETHODIMP Reset(void) override;
	STDMETHODIMP Clone(IEnumString** ppes) override;
};