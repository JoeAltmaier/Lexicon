/* TTimer_T.h -- Interval Timer with class specific notification or callback on the Windows thread
 *
 * Copyright (c) Tom Nelson D.B.A. OpenVista Software, 2020
 *
 * This material is a confidential trade secret and proprietary information of
 * OpenVista Software, which may not be reproduced, used, sold or transferred to
 * any third party without the prior written consent of OpenVista Software. This 
 * material is also copyrighted as an unpublished work under sections 104 and 408
 * of Title 17 of the United States Code. Law prohibits unauthorized use, copying
 * or reproduction.
 *
 * Revision History:
 *     4-20-20  TRN  Created
 *
**/

#pragma once

#ifndef __TTimer_T_h
#define __TTimer_T_h

#include "ATimer.h"
#include "AHashTable_T.h"

#include "TCallbackService.h"	// TMFC.lib


// TTimerCallbackMethod_T --  ATimer with direct callback to method on the Windows thread.
//
// Example:
//		class MyClass {
//			typedef TTimerCallbackMethod_T<MyClass> MyTimer;
//
//			MyClass() : timer(this,&MyClass::OnTimerExpired) { }
//
//			void OnTimerExpired(MyTimer *_pTimer);
//
//			MyTimer timer;
//		};
// 
// The virtual ATimer::OnTimerExpired() notifications cause a message to be posted to the Windows thread
// via the TCallbackService which in turn makes a static callback on the windows thread. 
//
template <class InstanceT>
class TTimerCallbackMethod_T : public ATimer {
public:
	typedef TTimerCallbackMethod_T<InstanceT> SelfT;
	typedef void (InstanceT::*MethodT)(SelfT* _pTimer);
	typedef AHashTableAddress_T<SelfT*, SelfT*> Table;

public:
	TTimerCallbackMethod_T(InstanceT* _pInstance, MethodT _pMethod) : pInstance(_pInstance), pMethod(_pMethod) { tableCallback.Insert(this, this); }
	virtual ~TTimerCallbackMethod_T() { tableCallback.Delete(this); }

	// Callback on the TimerService thread -- be quick about it!
	virtual void OnTimerExpired() override {
		TCallbackService::PostCallback(&SelfT::OnWinCallbackStatic, this);
	}

private:
	// Method callback handler (GUI thread)
	void OnWinCallbackMethod() {
		ASSERT(pInstance != nullptr && pMethod != nullptr);

		if (pInstance && pMethod)
			(pInstance->*pMethod)(this);
	}

	// static callback handler from TCallbackService (GUI thread)
	static LRESULT OnWinCallbackStatic(void* _pParm) {
		SelfT*pThis = (SelfT*)_pParm;

		//*FIX* Could be a better way supported directly by TCallbackServer
		if (tableCallback.Find(pThis, pThis))
			pThis->OnWinCallbackMethod();

		return OK;
	}

private:
	InstanceT* pInstance;
	MethodT    pMethod;

	static Table tableCallback;
};

// .tableCallback -- HashTable used to verify instance still exists when called back
//
template <class InstanceT>
typename TTimerCallbackMethod_T<InstanceT>::Table TTimerCallbackMethod_T<InstanceT>::tableCallback(64);


#endif	// __TTimer_T_h

