#include        "avrx.inc"
/*
        Copyright 1999-2000, Larry Barello
        larry@barello.net

*/
;
; AvrX Time Queue Manager
;
        _MODULE(avrx_canceltimer)
        _EXTERN(_TimerQueue)
        _EXTERN(_RemoveObject)
        _EXTERN(_Epilog)
        _EXTERN(IntProlog)
        _EXTERN(AvrXIntSetObjectSemaphore)

        _CODESECTION
/*+
; -----------------------------------------------
; pTimerControlBlock
; AvrXCancelTimer(pTimerControlBlock)
;
; Passed:       R25:R24 = TCB
; Returns:      Pointer to removed timer, or 0 if failure
; Uses:
; Stack:        
; Note: walk list looking for TCB.  If found, and not at
;       end of list, then add count to Next tcb.
;
-*/
        _FUNCTION(AvrXCancelTimer)
        
AvrXCancelTimer:
	push	p1l
	push	p1h
        mov     p2l, p1l
        mov     p2h, p1h
        ldi     Zl, lo8(_TimerQueue)
        ldi     Zh, hi8(_TimerQueue)

        BeginCritical
        rcall   _RemoveObject   ; Z = next item, R23:R22 = obj.
        subi    tmp0, lo8(0)
        sbci    tmp1, hi8(0)    ; Test if in timer queue
        breq    act00           ; No, just return

        mov     Yl, p2l         ; Y is restored in _Epilog
        mov     Yh, p2h
        ldd     p2l, Y+TcbCount+NextL
        ldd     p2h, Y+TcbCount+NextH
        ldd     r1h, Z+TcbCount+NextH ; Next
        ldd     r1l, Z+TcbCount+NextL
        add     r1l, p2l
        adc     r1h, p2h
        std     Z+TcbCount+NextL, r1l
        std     Z+TcbCount+NextH, r1h
        EndCritical
        mov	p1l, p2l
        mov	p1h, p2h
        rcall	AvrXSetObjectSemaphore
        pop	p1h
        pop	p1l
        ret
act00:
	clr	r1l
	clr	r1h		; return 0, failure
	EndCriticalReturn        
        _ENDFUNC
        _END

