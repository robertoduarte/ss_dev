
#ifndef	__ACT_H__
#define	__ACT_H__

	#include	"sgl.h"

	#define	_SHOTINTERVAL	8
	#define	_NODAMAGE		10
	#define	_BULLETS		3
	#define	_BULLETSPEED	toFIXED(6.0)
	#define	_BULLETROT		DEGtoANG(20.0)

	struct	BASICOBJECT;
	typedef	struct BASICOBJECT	BASICOBJECT;

	struct	BULLET;
	typedef	struct BULLET	BULLET;

	struct	ONEPLANE;
	typedef	struct ONEPLANE	ONEPLANE;

	struct	FIGHTER;
	typedef	struct FIGHTER	FIGHTER;

	struct	CAMERA;
	typedef	struct CAMERA	CAMERA;


	/**	基本オブジェクト	**/
	struct	BASICOBJECT{
		FIXED	pos[XYZ];
		PDATA	*pd;
		ANGLE	ang[XYZ];
	};


	/**	弾丸オブジェクト	**/
	struct	BULLET{
		FIXED	pos[XYZ];
		PDATA	*pd;
		ANGLE	ang[XYZ];
		short	id;
		FIXED	spd[XYZ];
		ANGLE	rot[XYZ];
		short	busy;
		FIGHTER	*target;
	};


	/**	破片オブジェクト	**/
	struct	ONEPLANE{
		FIXED	pos[XYZ];
		PDATA	*pd;
		ANGLE	ang[XYZ];
		short	id;
		short	spd[XYZ];	/* -1/0/1 or  2 */
		ANGLE	rot[XYZ];
	};

	/**	戦闘機オブジェクト	**/
	struct	FIGHTER{
		FIXED	pos[XYZ];		/*	 4 x  3 = 12	*/
		PDATA	*pd;			/*	 4 x  1 =  4	*/
		ANGLE	ang[XYZ];		/*	 2 x  3 =  6	*/
		short	id;				/*	 2 x  1 =  2	*/
		short	hitpoint;		/*	 2 x  1 =  2	*/
		short	hit;			/*	 2 x  1 =  2	*/
		char	shotwait;		/*	 1 x  1 =  1	*/
		char	collision;		/*	 1 x  1 =  1	*/
		BULLET	*myb[_BULLETS];	/*	 4 x  3 = 12	*/
	};							/*	total   = 42 	*/

	/** カメラオブジェクト	**/
	struct	CAMERA{
		FIXED	pos[XYZ];
		FIXED	target[XYZ];
		FIXED 	posZ;
		ANGLE	angZ;
		ANGLE 	rotY;
	};

#endif
