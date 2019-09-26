/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#ifdef __cplusplus

class FLASH
{

private:
	int togval;
public:
	FLASH();
	FLASH(int);
	FLASH(int,int);
	void fDelay(unsigned long ul = 50000l);
	void red(int val=1);
	void green(int val=1);
	void tog();

};

#endif // #ifdef __cplusplus
