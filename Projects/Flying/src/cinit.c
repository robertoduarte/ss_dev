/* 
    C����ŕK�v�ȏ����������T���v�����[�`��(SGL�T���v���v���O�����p)
      1.BSS(���������̈�)�̃N���A
      2.ROM�̈悩��RAM�̈�ւ̃R�s�[(���݂��Ȃ��ꍇ�͂���Ȃ�)
      3.SGL�V�X�e���ϐ��̈�̃N���A
 */

#include "sgl.h"

#define  SystemWork  0x060ffc00  /* System Variable Address */
#define  SystemSize  (0x06100000-0x060ffc00)  /* System Variable Size */
/* sl.lnk �Ŏw�肵��.bss�Z�N�V�����̊J�n�A�I���V���{�� */
extern Uint32 _bstart, _bend;
/* */
extern void ss_main(void);

// GNUSH: void to int

int main(void)
{
    Uint8 *dst;
    Uint32 i;

    /* 1.Zero Set .bss Section */
    for (dst = (Uint8 *) & _bstart; dst < (Uint8 *) & _bend; dst++)
    {
        *dst = 0;
    }
    /* 2.ROM has data at end of text; copy it. */

    /* 3.SGL System Variable Clear */
    for (dst = (Uint8 *) SystemWork, i = 0; i < SystemSize; i++)
    {
        *dst = 0;
    }

    /* Application Call */
    ss_main();
}
