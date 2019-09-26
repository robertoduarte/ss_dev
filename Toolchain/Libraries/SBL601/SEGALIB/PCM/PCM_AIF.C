/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:read AIFF and AIFC format
 * File		:pcm_aif.c
 * Date		:1994-09-30
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include "pcm_msub.h"
#include "pcm_aif.h"

/*--------------------- 《グローバル関数ポインタ》 --------------------*/

/* １チャンク処理：Adpcm Chunk のADPCM非対応処理／対応処理
 *	次のいずれかが登録されている
 *		pcm_ChunkAdpcm
 *		pcm_ChunkAdpcmUnlink
 */
void (*pcm_chunk_adpcm_fp)(PcmHn hn, PcmAdpcmChunk *chunk);



/* １チャンク処理：AIFF の Common Chunk, AIFC の Common Chunk の前半 */
static void pcm_ChunkCommon(PcmHn hn, PcmCommonChunk *chunk)
{
	PcmWork		*work 	= *(PcmWork **)hn;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara		*para 	= &work->para;
#endif
	PcmStatus	*st 	= &work->status;

	if (st->ignore_header == ON) {
		return;
	}

	/* チャネル数 		*/
	st->info.channel = (Sint32)chunk->num_channels;

	/* サンプリングビット数 */
	st->info.sampling_bit = (Sint32)chunk->sample_size;

	/* サンプリングレート[Hz] */
	st->info.sampling_rate = (Sint32)chunk->sample_rate.man[0] & 0x0000ffff;
	st->info.sampling_rate >>= 0x400E - chunk->sample_rate.exp[0];

	/* ファイルの総サンプル数 */
	st->info.sample_file = VALUE_SINT32(&chunk->num_sample_frames);

	/* 簡単の為に、最後の端数は無いものとする。 */
	/* st->info.sample_file &= 0xfffffff8; */
	/* これはダメだよ、スムーズ連続の場合に、ごまかすわけには
	 * いかないんだ。
	 */

	st->info.data_type = PCM_DATA_TYPE_LRLRLR;
}

/* １チャンク処理：AIFC の Common Chunk */
static void pcm_ChunkExtCommon(PcmHn hn, PcmExtCommonChunk *chunk)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (st->ignore_header == ON) {
		return;
	}

	/* 前半は AIFF の Common Chunk と同じ */
	pcm_ChunkCommon(hn, (PcmCommonChunk *)chunk);

	/* 圧縮方式だけ記録する */
	st->info.compression_type = VALUE_SINT32(&chunk->compression_type);

	st->info.data_type = PCM_DATA_TYPE_ADPCM_SCT;

	/* 1 [sound group/1ch] = 32+2 [byte] = 64 [sample] */
	st->info.sample_file *= 64;
}

/* １チャンク処理：Sound Data Chunk */
static void pcm_ChunkSoundData(PcmHn hn, PcmSoundDataChunk *chunk)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;

	st->media_offset = (Sint32)chunk + 4*4 - (Sint32)para->ring_addr;
}

/*******************************************************************
【機　能】
	メモリ再生／１チャンク処理：Adpcm Chunk のADPCM非対応処理
【引　数】
	hn　	（入力）　ハンドル
【戻り値】
	なし
【備　考】
	関数ポインタ pcm_chunk_adpcm_fp に設定されてコールされる
*******************************************************************/
void pcm_ChunkAdpcmUnlink(PcmHn hn, PcmAdpcmChunk *chunk)
{
	/* ADPCM使用宣言がないのでAPCM形式ファイルを処理できない */
	PCM_MeSetErrCode(PCM_ERR_NOT_DECLARE_ADPCM);
}

/* AIFF または、AIFC のヘッダ解析処理 */
Sint32 pcm_AifHeaderProcess(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	PcmChunkHeader *local_chunk;
	PcmContainerChunk *form_chunk = (PcmContainerChunk *)para->ring_addr;
	Sint8		*read_addr = para->ring_addr;
	Sint32		read_size = 0;
	Sint32		flag_common = 0;/* コモンチャンク処理フラグ */
	Uint32		ch_id;

	read_addr += sizeof(PcmContainerChunk);
	read_size += sizeof(PcmContainerChunk);

	if (read_size + 32 > st->ring_write_offset) {
		return -1;/* データ供給不足 */
	}

	if (VALUE_UINT32(&form_chunk->ck_id) != PCM_ID_FORM) {
		PCM_MeSetErrCode(PCM_ERR_HEADER_DATA);
		return -1;
	}

	st->info.file_size = VALUE_SINT32(&form_chunk->ck_size) + 8;

	if (VALUE_UINT32(&form_chunk->form_type) == PCM_ID_AIFF) {
		st->info.file_type = PCM_FILE_TYPE_AIFF;
		for (;;) {
			local_chunk = (PcmChunkHeader *)read_addr;
			read_addr += VALUE_UINT32(&local_chunk->ck_size) + 8;
			read_size += VALUE_UINT32(&local_chunk->ck_size) + 8;
			ch_id = VALUE_UINT32(&local_chunk->ck_id);
			if (ch_id == PCM_ID_COMMON) {
				pcm_ChunkCommon(hn, (PcmCommonChunk *)local_chunk);
				flag_common = 1;
			} else if (ch_id == PCM_ID_SOUND_DATA) {
				pcm_ChunkSoundData(hn, (PcmSoundDataChunk *)local_chunk);
				if (flag_common == 0) {
					/* サウンドデータチャンクの前にコモンチャンクがない */
					PCM_MeSetErrCode(PCM_ERR_AFI_NO_COMMON);
					return -1;
				}
				return 0;
			} else if (ch_id == PCM_ID_ADPCM) {
				(*pcm_chunk_adpcm_fp)(hn, (PcmAdpcmChunk *)local_chunk);
				if (flag_common == 0) {
					/* サウンドデータチャンクの前にコモンチャンクがない */
					PCM_MeSetErrCode(PCM_ERR_AFI_NO_COMMON);
					return -1;
				}
				return 0;
			}
			if (read_size + 32 > st->ring_write_offset) {
				if (read_size > para->ring_size) {
					PCM_MeSetErrCode(PCM_ERR_TOO_LARGE_HEADER);
					return -1;
				}
				return -1;/* データ供給不足 */
			}
		}
	} else if (VALUE_UINT32(&form_chunk->form_type) == PCM_ID_AIFC) {
		st->info.file_type = PCM_FILE_TYPE_AIFC;
		for (;;) {
			local_chunk = (PcmChunkHeader *)read_addr;
			read_addr += VALUE_UINT32(&local_chunk->ck_size) + 8;
			ch_id = VALUE_UINT32(&local_chunk->ck_id);
			if (ch_id == PCM_ID_COMMON) {
				pcm_ChunkExtCommon(hn, (PcmExtCommonChunk *)local_chunk);
				flag_common = 1;
				if (st->info.compression_type != PCM_TYPE_NONE
				 && st->info.compression_type != PCM_TYPE_IMA4) {
					/* 未対応圧縮タイプ */
					PCM_MeSetErrCode(PCM_ERR_AFI_COMMPRESS);
					return -1;
				}
			} else if (ch_id == PCM_ID_SOUND_DATA) {
				pcm_ChunkSoundData(hn, (PcmSoundDataChunk *)local_chunk);
				if (flag_common == 0) {
					/* サウンドデータチャンクの前にコモンチャンクがない */
					PCM_MeSetErrCode(PCM_ERR_AFI_NO_COMMON);
					return -1;
				}
				return 0;
			}
			if (read_size + 32 > st->ring_write_offset) {
				if (read_size > para->ring_size) {
					PCM_MeSetErrCode(PCM_ERR_TOO_LARGE_HEADER);
					return -1;
				}
				return -1;/* データ供給不足 */
			}
		}
	} else {
		PCM_MeSetErrCode(PCM_ERR_HEADER_DATA);
		return -1;
	}
	return 0;
}
