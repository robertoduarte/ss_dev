/*	machine.h	*/

#ifndef __MACHINE_H
#define __MACHINE_H

#define sleep()                     _builtin_sleep()
#define tas(addr)                   _builtin_tas(addr)
#define get_imask()                 _builtin_get_imask()
#define set_imask(mask)             _builtin_set_imask(mask)
#define set_vbr(base)               _builtin_set_vbr(base)
#define get_vbr()                   _builtin_get_vbr()
#define set_gbr(base)               _builtin_set_gbr(base)
#define get_gbr()                   _builtin_get_gbr()
#endif	/* __MACHINE_H	*/

#define set_cr(cr)                  _builtin_set_cr(cr)
#define get_cr()                    _builtin_get_cr()
#define gbr_read_byte(offset)       _builtin_gbr_read_byte(offset)
#define gbr_read_word(offset)       _builtin_gbr_read_word(offset)
#define gbr_read_long(offset)       _builtin_gbr_read_long(offset)
#define gbr_write_byte(offset,data) _builtin_gbr_write_byte(offset,data)
#define gbr_write_word(offset,data) _builtin_gbr_write_word(offset,data)
#define gbr_write_long(offset,data) _builtin_gbr_write_long(offset,data)
#define gbr_and_byte(offset,mask)   _builtin_gbr_and_byte(offset,mask)
#define gbr_or_byte(offset,mask)    _builtin_gbr_or_byte(offset,mask)
#define gbr_xor_byte(offset,mask)   _builtin_gbr_xor_byte(offset,mask)
#define gbr_tst_byte(offset,mask)   _builtin_gbr_tst_byte(offset,mask)
#define trapa(trap_no)              _builtin_trapa(trap_no)
#define macw(ptr1, ptr2, count)        _builtin_macw(ptr1, ptr2, count)
#define macwl(ptr1, ptr2, count, mask) _builtin_macwl(ptr1, ptr2, count, mask)
#define macl(ptr1, ptr2, count)        _builtin_macl(ptr1, ptr2, count)
#define macll(ptr1, ptr2, count, mask) _builtin_macll(ptr1, ptr2, count, mask)
#define trapa_svc                      _builtin_trapa_svc
