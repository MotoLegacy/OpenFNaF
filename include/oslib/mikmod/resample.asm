.386p
.model flat
.data ; self-modifying code... keep in data segment

        NAME    resample
        EXTRN   _rvoltab :DWORD
        EXTRN   _lvoltab :DWORD
        EXTRN   _bitshift :BYTE

        PUBLIC  _AsmStereoNormal
        PUBLIC  _AsmStereoSurround
        PUBLIC  _AsmMonoNormal
IFDEF __ASMSTUFF__
        PUBLIC  _AsmMix32To16_Normal
        PUBLIC  _AsmMix32To8_Normal
ENDIF


STUBSTART macro
  push eax
  push ebx
  push ecx
  push edx
  push esi
  push edi
  push ebp
  endm


STUBEND macro
  pop ebp
  pop edi
  pop esi
  pop edx
  pop ecx
  pop ebx
  pop eax
  endm


SS2F MACRO index,lab1,lab2,lab3,lab4 ; 486+
 mov al,[edx]
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*8)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 mov ebp,(index*8+4)[edi]
 add esi,ecx
 db 08bh,0ch,085h
lab4 dd 0 ; mov ecx,[eax*4+table2]
 mov (index*8)[edi],esi
 add ebp,ecx
 ENDM

SS2M MACRO index,lab1,lab2,lab3,lab4 ; 486+
 mov al,[edx]
 mov (index*8-4)[edi],ebp
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*8)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 mov ebp,(index*8+4)[edi]
 add esi,ecx
 db 08bh,0ch,085h
lab4 dd 0 ; mov ecx,[eax*4+table2]
 mov (index*8)[edi],esi
 add ebp,ecx
 ENDM

SS3F MACRO index,lab1,lab2,lab3 ; 486+
 mov al,[edx]
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*8)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 mov ebp,(index*8+4)[edi]
 add esi,ecx
 sub ebp,ecx
 mov (index*8)[edi],esi
 ENDM

SS3M MACRO index,lab1,lab2,lab3 ; 486+
 mov al,[edx]
 mov (index*8-4)[edi],ebp
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*8)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 mov ebp,(index*8+4)[edi]
 add esi,ecx
 sub ebp,ecx
 mov (index*8)[edi],esi
 ENDM

SS2L MACRO index ; 486+
 mov (index*8-4)[edi],ebp
 ENDM

SM2F MACRO index,lab1,lab2,lab3 ; 486+
 mov al,[edx] ; AGI-3
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*4)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 ENDM

SM2M MACRO index,lab1,lab2,lab3 ; 486+
 mov al,[edx] ; AGI-3
 add esi,ecx
 db 081h,0c3h
lab1 dd 0 ; add ebx,lo
 mov (index*4-4)[edi],esi
 db 081h,0d2h
lab2 dd 0 ; adc edx,hi
 mov esi,(index*4)[edi]
 db 08bh,0ch,085h
lab3 dd 0 ; mov ecx,[eax*4+table1]
 ENDM

SM2L MACRO index ; 486+
 add esi,ecx
 mov (index*4-4)[edi],esi
 ENDM

;SS2 ; 386
;mov al,[edx+source]
;add ebx,lo
;adc edx,hi
;mov ecx,[eax*4+table1]
;mov ebp,[eax*4+table2]
;add [edi],ecx
;add [edi+4],ebp
;ENDM

;SS2M ; 486+
;mov al,[edx+source]
;mov [edi-8+4],ebp
;add ebx,lo
;adc edx,hi
;mov esi,[edi]
;mov ecx,[eax*4+table1]
;mov ebp,[edi+4]
;add esi,ecx
;mov ecx,[eax*4+table2]
;mov [edi],esi
;add ebp,ecx
;ENDM

;SM2M ; 386
;add ebx,lo
;adc edx,hi
;mov ecx,[eax*4+table1]
;mov al,[edx+source]
;add [edi],ecx
;ENDM

;SM2M ; 486+
;add ebx,lo
;mov [edi-4],esi
;adc edx,hi
;mov esi,[edi]
;mov ecx,[eax*4+table1]
;mov al,[edx+source] ; AGI-3
;add esi,ecx
;ENDM


_AsmStereoNormal:
        STUBSTART
        mov    esi,[esp+32] ; get src
        mov    edi,[esp+36] ; get dst
        mov    ebx,[esp+40] ; get index
        mov    ecx,[esp+44] ; get increment
        mov    ebp,[esp+48] ; get todo
        mov    eax,ecx
        mov    edx,ebx
        sar    eax,31
        sar    edx,31
        shld   eax,ecx,21 ; convert to 32:32
        shl    ecx,21
        shld   edx,ebx,21 ; convert to 32:32
        shl    ebx,21
        add    edx,esi
        mov    shi1,eax
        mov    shi2,eax
        mov    shi3,eax
        mov    shi4,eax
        mov    shi5,eax
        mov    eax,_lvoltab
        mov    slo1,ecx
        mov    slo2,ecx
        mov    slo3,ecx
        mov    slo4,ecx
        mov    slo5,ecx
        mov    sltab1,eax
        mov    sltab2,eax
        mov    sltab3,eax
        mov    sltab4,eax
        mov    sltab5,eax
        mov    eax,_rvoltab
        push   ebp
        mov    srtab1,eax
        mov    srtab2,eax
        mov    srtab3,eax
        mov    srtab4,eax
        mov    srtab5,eax
        xor    eax,eax
        jmp    s1 ; flush code cache
s1:
        shr    ebp,2
        jz     sskip16
        push   ebp
sagain16:
        SS2F   0,slo1,shi1,sltab1,srtab1
        SS2M   1,slo2,shi2,sltab2,srtab2
        SS2M   2,slo3,shi3,sltab3,srtab3
        SS2M   3,slo4,shi4,sltab4,srtab4
        SS2L   4
        add    edi,(4*8)
        dec    dword ptr [esp]
        jnz    sagain16
        pop    ebp
sskip16:
        pop    ebp
        and    ebp,3
        jz     sskip1
        push   ebp
sagain1:
        SS2F   0,slo5,shi5,sltab5,srtab5
        SS2L   1
        add    edi,8
        dec    dword ptr [esp]
        jnz    sagain1
        pop    ebp
sskip1:
        STUBEND
        ret


_AsmStereoSurround:
        STUBSTART
        mov    esi,[esp+32] ; get src
        mov    edi,[esp+36] ; get dst
        mov    ebx,[esp+40] ; get index
        mov    ecx,[esp+44] ; get increment
        mov    ebp,[esp+48] ; get todo
        mov    eax,ecx
        mov    edx,ebx
        sar    eax,31
        sar    edx,31
        shld   eax,ecx,21 ; convert to 32:32
        shl    ecx,21
        shld   edx,ebx,21 ; convert to 32:32
        shl    ebx,21
        add    edx,esi
        mov    s2hi1,eax
        mov    s2hi2,eax
        mov    s2hi3,eax
        mov    s2hi4,eax
        mov    s2hi5,eax
        mov    eax,_lvoltab
        mov    s2lo1,ecx
        mov    s2lo2,ecx
        mov    s2lo3,ecx
        mov    s2lo4,ecx
        mov    s2lo5,ecx
        mov    s2ltab1,eax
        mov    s2ltab2,eax
        mov    s2ltab3,eax
        mov    s2ltab4,eax
        mov    s2ltab5,eax
        ;mov    eax,_rvoltab
        push   ebp
        ;mov    s2rtab1,eax
        ;mov    s2rtab2,eax
        ;mov    s2rtab3,eax
        ;mov    s2rtab4,eax
        ;mov    s2rtab5,eax
        xor    eax,eax
        jmp    s3 ; flush code cache
s3:
        shr    ebp,2
        jz     s2skip16
        push   ebp
s2again16:
        SS3F   0,s2lo1,s2hi1,s2ltab1  ;,s2rtab1
        SS3M   1,s2lo2,s2hi2,s2ltab2  ;,s2rtab2
        SS3M   2,s2lo3,s2hi3,s2ltab3  ;,s2rtab3
        SS3M   3,s2lo4,s2hi4,s2ltab4  ;,s2rtab4
        SS2L   4
        add    edi,(4*8)
        dec    dword ptr [esp]
        jnz    s2again16
        pop    ebp
s2skip16:
        pop    ebp
        and    ebp,3
        jz     s2skip1
        push   ebp
s2again1:
        SS3F   0,s2lo5,s2hi5,s2ltab5  ;,s2rtab5
        SS2L   1
        add    edi,8
        dec    dword ptr [esp]
        jnz    s2again1
        pop    ebp
s2skip1:
        STUBEND
        ret


_AsmMonoNormal:
        STUBSTART
        mov    esi,[esp+32] ; get src
        mov    edi,[esp+36] ; get dst
        mov    ebx,[esp+40] ; get index
        mov    ecx,[esp+44] ; get increment
        mov    ebp,[esp+48] ; get todo
        mov    eax,ecx
        mov    edx,ebx
        sar    eax,31
        sar    edx,31
        shld   eax,ecx,21 ; convert to 32:32
        shl    ecx,21
        shld   edx,ebx,21 ; convert to 32:32
        shl    ebx,21
        add    edx,esi
        mov    mhi1,eax
        mov    mhi2,eax
        mov    mhi3,eax
        mov    mhi4,eax
        mov    mhi5,eax
        mov    eax,_lvoltab
        mov    mlo1,ecx
        mov    mlo2,ecx
        mov    mlo3,ecx
        mov    mlo4,ecx
        mov    mlo5,ecx
        mov    mltab1,eax
        mov    mltab2,eax
        mov    mltab3,eax
        mov    mltab4,eax
        mov    mltab5,eax
        xor    eax,eax

        jmp    m1 ; flush code cache
m1:
        push   ebp
        shr    ebp,2
        jz     mskip16
magain16:
        SM2F   0,mlo1,mhi1,mltab1
        SM2M   1,mlo2,mhi2,mltab2
        SM2M   2,mlo3,mhi3,mltab3
        SM2M   3,mlo4,mhi4,mltab4
        SM2L   4
        add    edi,(4*4)
        dec    ebp
        jnz    magain16
mskip16:
        pop    ebp
        and    ebp,3
        jz     mskip1
magain1:
        SM2F   0,mlo5,mhi5,mltab5
        SM2L   1
        add    edi,4
        dec    ebp
        jnz    magain1
mskip1:
        STUBEND
        ret



IFDEF __ASMSTUFF__
_AsmMix32To16_Normal:
        STUBSTART
        mov   edi,[esp+32] ; get dest
        mov   esi,[esp+36] ; get src
        mov   edx,[esp+40] ; get count
        mov    cl,[_bitshift]
again:
        mov   eax,[esi]
        sar   eax,cl
        cmp   eax,32767
        jg    toobig
        cmp   eax,-32768
        jl    toosmall
write:
        mov   [edi],ax
        add   esi,4
        add   edi,2
        dec   edx
        jnz   again
        jmp   ready
toobig:
        mov   eax,32767
        jmp   write
toosmall:
        mov   eax,-32768
        jmp   write
ready:
        STUBEND
        ret


_AsmMix32To8_Normal:
        STUBSTART
        mov   edi,[esp+32] ; get dest
        mov   esi,[esp+36] ; get src
        mov   edx,[esp+40] ; get count
        mov    cl,[_bitshift]
cagain:
        mov   eax,[esi]
        sar   eax,cl
        cmp   eax,127
        jg    ctoobig
        cmp   eax,-128
        jl    ctoosmall
cwrite:
        add   al,080h
        add   esi,4
        mov   [edi],al
        inc   edi
        dec   edx
        jnz   cagain
        jmp   cready
ctoobig:
        mov   eax,127
        jmp   cwrite
ctoosmall:
        mov   eax,-128
        jmp   cwrite
cready:
        STUBEND
        ret
ENDIF

        END
