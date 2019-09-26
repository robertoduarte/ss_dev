typedef struct {
    VECTOR	 norm ;			/* 法線ベクトル */
	Uint16	att;			/*属性*/
	Uint16	gru;			/*グループ*/
	FIXED	cen_x;			/*中心*/
	FIXED	cen_y;			/*中心*/
	FIXED	cen_z;			/*中心*/
} COLLISON;

typedef struct {
	COLLISON	*cotbl;		/* ポリゴン定義テーブル */
	Uint32		 nbCo;		/* ポリゴンの数 */
}CDATA;
