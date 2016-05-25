
procedure.o:     формат файла elf64-x86-64

Разделы:
Инд Имя           Размер    VMA               LMA               Файл      Вырав
  0 .group        00000008  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, READONLY, EXCLUDE, GROUP, LINK_ONCE_DISCARD
  1 .group        00000008  0000000000000000  0000000000000000  00000048  2**2
                  CONTENTS, READONLY, EXCLUDE, GROUP, LINK_ONCE_DISCARD
  2 .text         0000000b  0000000000000000  0000000000000000  00000050  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  3 .data         00000000  0000000000000000  0000000000000000  0000005b  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  4 .bss          00000000  0000000000000000  0000000000000000  0000005b  2**0
                  ALLOC
  5 .debug_info   000000a2  0000000000000000  0000000000000000  0000005b  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  6 .debug_abbrev 0000008d  0000000000000000  0000000000000000  000000fd  2**0
                  CONTENTS, READONLY, DEBUGGING
  7 .debug_aranges 00000030  0000000000000000  0000000000000000  0000018a  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  8 .debug_macro  00000024  0000000000000000  0000000000000000  000001ba  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  9 .debug_macro  0000061c  0000000000000000  0000000000000000  000001de  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 10 .debug_macro  00000022  0000000000000000  0000000000000000  000007fa  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 11 .debug_line   00000079  0000000000000000  0000000000000000  0000081c  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 12 .debug_str    00001b88  0000000000000000  0000000000000000  00000895  2**0
                  CONTENTS, READONLY, DEBUGGING
 13 .comment      0000002e  0000000000000000  0000000000000000  0000241d  2**0
                  CONTENTS, READONLY
 14 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000244b  2**0
                  CONTENTS, READONLY
 15 .eh_frame     00000038  0000000000000000  0000000000000000  00002450  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA

Дизассемблирование раздела .text:

0000000000000000 <_ZN9procedureC1Ev>:
#include "procedure.h"

procedure::procedure()
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
{

}
   8:	90                   	nop
   9:	5d                   	pop    %rbp
   a:	c3                   	retq   
