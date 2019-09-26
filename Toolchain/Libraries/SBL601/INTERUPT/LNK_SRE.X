OUTPUT_FORMAT("srec")
OUTPUT_ARCH(sh)

SECTIONS
{
	.text	0x06010000:
	{
		*(.text)
		*(.strings)
	}

	.tors  ALIGN(0x10) :
	{
		___ctors = . ;
		*(.ctors)
		___ctors_end = . ;
		___dtors = . ;
		*(.dtors)
		___dtors_end = . ;
	}

	.data ALIGN(0x10):
	{
		STARTOF_R = .;
		*(.data)
		SIZEOF_R = . - STARTOF_R;
		ENDOF_R = .;
	}
	.bss ALIGN(0x10) (NOLOAD):
	{
		STARTOF_B = .;
		*(.bss)
		*( COMMON )
		SIZEOF_B = . - STARTOF_B;
		ENDOF_B = .;
		_end = .;
 	}
}


