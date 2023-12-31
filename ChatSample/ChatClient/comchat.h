

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 05:14:07 2038
 */
/* Compiler settings for comchat.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __comchat_h__
#define __comchat_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IChatSession_FWD_DEFINED__
#define __IChatSession_FWD_DEFINED__
typedef interface IChatSession IChatSession;

#endif 	/* __IChatSession_FWD_DEFINED__ */


#ifndef __IChatSessionEvents_FWD_DEFINED__
#define __IChatSessionEvents_FWD_DEFINED__
typedef interface IChatSessionEvents IChatSessionEvents;

#endif 	/* __IChatSessionEvents_FWD_DEFINED__ */


#ifndef __IChatSessionManager_FWD_DEFINED__
#define __IChatSessionManager_FWD_DEFINED__
typedef interface IChatSessionManager IChatSessionManager;

#endif 	/* __IChatSessionManager_FWD_DEFINED__ */


/* header files for imported files */
#include "objidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_comchat_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_comchat_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_comchat_0000_0000_v0_0_s_ifspec;

#ifndef __IChatSession_INTERFACE_DEFINED__
#define __IChatSession_INTERFACE_DEFINED__

/* interface IChatSession */
/* [object][uuid] */ 


EXTERN_C const IID IID_IChatSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5223A050-2441-11d1-AF4F-0060976AA886")
    IChatSession : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SessionName( 
            /* [string][out] */ OLECHAR **ppwsz) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Say( 
            /* [string][in] */ const OLECHAR *pwszStatement) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatements( 
            /* [out] */ IEnumString **ppes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ IChatSessionEvents *pEventSink,
            /* [out] */ DWORD *pdwReg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ DWORD dwReg) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IChatSessionVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatSession * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatSession * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatSession * This);
        
        DECLSPEC_XFGVIRT(IChatSession, get_SessionName)
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SessionName )( 
            IChatSession * This,
            /* [string][out] */ OLECHAR **ppwsz);
        
        DECLSPEC_XFGVIRT(IChatSession, Say)
        HRESULT ( STDMETHODCALLTYPE *Say )( 
            IChatSession * This,
            /* [string][in] */ const OLECHAR *pwszStatement);
        
        DECLSPEC_XFGVIRT(IChatSession, GetStatements)
        HRESULT ( STDMETHODCALLTYPE *GetStatements )( 
            IChatSession * This,
            /* [out] */ IEnumString **ppes);
        
        DECLSPEC_XFGVIRT(IChatSession, Advise)
        HRESULT ( STDMETHODCALLTYPE *Advise )( 
            IChatSession * This,
            /* [in] */ IChatSessionEvents *pEventSink,
            /* [out] */ DWORD *pdwReg);
        
        DECLSPEC_XFGVIRT(IChatSession, Unadvise)
        HRESULT ( STDMETHODCALLTYPE *Unadvise )( 
            IChatSession * This,
            /* [in] */ DWORD dwReg);
        
        END_INTERFACE
    } IChatSessionVtbl;

    interface IChatSession
    {
        CONST_VTBL struct IChatSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatSession_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatSession_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatSession_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatSession_get_SessionName(This,ppwsz)	\
    ( (This)->lpVtbl -> get_SessionName(This,ppwsz) ) 

#define IChatSession_Say(This,pwszStatement)	\
    ( (This)->lpVtbl -> Say(This,pwszStatement) ) 

#define IChatSession_GetStatements(This,ppes)	\
    ( (This)->lpVtbl -> GetStatements(This,ppes) ) 

#define IChatSession_Advise(This,pEventSink,pdwReg)	\
    ( (This)->lpVtbl -> Advise(This,pEventSink,pdwReg) ) 

#define IChatSession_Unadvise(This,dwReg)	\
    ( (This)->lpVtbl -> Unadvise(This,dwReg) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatSession_INTERFACE_DEFINED__ */


#ifndef __IChatSessionEvents_INTERFACE_DEFINED__
#define __IChatSessionEvents_INTERFACE_DEFINED__

/* interface IChatSessionEvents */
/* [object][uuid] */ 


EXTERN_C const IID IID_IChatSessionEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5223A051-2441-11d1-AF4F-0060976AA886")
    IChatSessionEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnNewUser( 
            /* [string][in] */ const OLECHAR *pwszUser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnUserLeft( 
            /* [string][in] */ const OLECHAR *pwszUser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnNewStatement( 
            /* [string][in] */ const OLECHAR *pwszUser,
            /* [string][in] */ const OLECHAR *pwszStmnt) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IChatSessionEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatSessionEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatSessionEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatSessionEvents * This);
        
        DECLSPEC_XFGVIRT(IChatSessionEvents, OnNewUser)
        HRESULT ( STDMETHODCALLTYPE *OnNewUser )( 
            IChatSessionEvents * This,
            /* [string][in] */ const OLECHAR *pwszUser);
        
        DECLSPEC_XFGVIRT(IChatSessionEvents, OnUserLeft)
        HRESULT ( STDMETHODCALLTYPE *OnUserLeft )( 
            IChatSessionEvents * This,
            /* [string][in] */ const OLECHAR *pwszUser);
        
        DECLSPEC_XFGVIRT(IChatSessionEvents, OnNewStatement)
        HRESULT ( STDMETHODCALLTYPE *OnNewStatement )( 
            IChatSessionEvents * This,
            /* [string][in] */ const OLECHAR *pwszUser,
            /* [string][in] */ const OLECHAR *pwszStmnt);
        
        END_INTERFACE
    } IChatSessionEventsVtbl;

    interface IChatSessionEvents
    {
        CONST_VTBL struct IChatSessionEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatSessionEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatSessionEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatSessionEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatSessionEvents_OnNewUser(This,pwszUser)	\
    ( (This)->lpVtbl -> OnNewUser(This,pwszUser) ) 

#define IChatSessionEvents_OnUserLeft(This,pwszUser)	\
    ( (This)->lpVtbl -> OnUserLeft(This,pwszUser) ) 

#define IChatSessionEvents_OnNewStatement(This,pwszUser,pwszStmnt)	\
    ( (This)->lpVtbl -> OnNewStatement(This,pwszUser,pwszStmnt) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatSessionEvents_INTERFACE_DEFINED__ */


#ifndef __IChatSessionManager_INTERFACE_DEFINED__
#define __IChatSessionManager_INTERFACE_DEFINED__

/* interface IChatSessionManager */
/* [object][uuid] */ 


EXTERN_C const IID IID_IChatSessionManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5223A052-2441-11d1-AF4F-0060976AA886")
    IChatSessionManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSessionNames( 
            /* [out] */ IEnumString **ppes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindSession( 
            /* [string][in] */ const OLECHAR *pwszName,
            /* [in] */ BOOL bDontCreate,
            /* [in] */ BOOL bAllowAnonymousAccess,
            /* [out] */ IChatSession **ppcs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteSession( 
            /* [string][in] */ const OLECHAR *pwszName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IChatSessionManagerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatSessionManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatSessionManager * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatSessionManager * This);
        
        DECLSPEC_XFGVIRT(IChatSessionManager, GetSessionNames)
        HRESULT ( STDMETHODCALLTYPE *GetSessionNames )( 
            IChatSessionManager * This,
            /* [out] */ IEnumString **ppes);
        
        DECLSPEC_XFGVIRT(IChatSessionManager, FindSession)
        HRESULT ( STDMETHODCALLTYPE *FindSession )( 
            IChatSessionManager * This,
            /* [string][in] */ const OLECHAR *pwszName,
            /* [in] */ BOOL bDontCreate,
            /* [in] */ BOOL bAllowAnonymousAccess,
            /* [out] */ IChatSession **ppcs);
        
        DECLSPEC_XFGVIRT(IChatSessionManager, DeleteSession)
        HRESULT ( STDMETHODCALLTYPE *DeleteSession )( 
            IChatSessionManager * This,
            /* [string][in] */ const OLECHAR *pwszName);
        
        END_INTERFACE
    } IChatSessionManagerVtbl;

    interface IChatSessionManager
    {
        CONST_VTBL struct IChatSessionManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatSessionManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatSessionManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatSessionManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatSessionManager_GetSessionNames(This,ppes)	\
    ( (This)->lpVtbl -> GetSessionNames(This,ppes) ) 

#define IChatSessionManager_FindSession(This,pwszName,bDontCreate,bAllowAnonymousAccess,ppcs)	\
    ( (This)->lpVtbl -> FindSession(This,pwszName,bDontCreate,bAllowAnonymousAccess,ppcs) ) 

#define IChatSessionManager_DeleteSession(This,pwszName)	\
    ( (This)->lpVtbl -> DeleteSession(This,pwszName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatSessionManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_comchat_0000_0003 */
/* [local] */ 

DEFINE_GUID(CLSID_ChatSession,0x5223a053,0x2441,
0x11d1,0xaf,0x4f,0x0,0x60,0x97,0x6a,0xa8,0x86);


extern RPC_IF_HANDLE __MIDL_itf_comchat_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_comchat_0000_0003_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


