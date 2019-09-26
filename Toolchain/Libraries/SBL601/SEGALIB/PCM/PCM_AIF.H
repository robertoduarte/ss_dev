/* 	
 *	Created: Monday, December 2, 1991 at 5:00 PM
 *	AIFF.h
 *	C Interface to the Macintosh Libraries
 * 	
 * 	Copyright Apple Computer, Inc. 1990-1991
 * 	All rights reserved
 *  
 */
/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:read AIFF and AIFC format
 * File		:pcm_aif.h
 * Date		:1994-09-29
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_AIF_H)
#define PCM_AIF_H


/*------------------------- 《マクロ定数》 -------------------------*/

/* Chank ID */
#if 1
	/* このような定義だと無駄があり、データがかさむ。 */
	#define PCM_ID_AIFF 				(*(unsigned long *)"AIFF")
	#define PCM_ID_AIFC 				(*(unsigned long *)"AIFC")
	#define PCM_ID_FORMAT_VERSION 		(*(unsigned long *)"FVER")
	#define PCM_ID_COMMON 				(*(unsigned long *)"COMM")
	#define PCM_ID_FORM 				(*(unsigned long *)"FORM")
	#define PCM_ID_SOUND_DATA 			(*(unsigned long *)"SSND")
	#define PCM_ID_ADPCM 				(*(unsigned long *)"APCM")
	#define PCM_ID_MARKER 				(*(unsigned long *)"MARK")
	#define PCM_ID_INS_TRUMENT 			(*(unsigned long *)"INST")
	#define PCM_ID_MIDI_DATA 			(*(unsigned long *)"MIDI")
	#define PCM_ID_AUDIO_RECORDING 		(*(unsigned long *)"AESD")
	#define PCM_ID_APPLICATION_SPECIFIC	(*(unsigned long *)"APPL")
	#define PCM_ID_COMMENT 				(*(unsigned long *)"COMT")
	#define PCM_ID_NAME 				(*(unsigned long *)"NAME")
	#define PCM_ID_AUTHOR 				(*(unsigned long *)"AUTH")
	#define PCM_ID_COPYRIGHT 			(*(unsigned long *)"(c) ")
	#define PCM_ID_ANNOTATION 			(*(unsigned long *)"ANNO")
#else
	/* このような定義を使いたいが、SCH Ver2.0 はワーニングを出す。	*/
	/* (正常に動作はする)											*/
	#define PCM_ID_AIFF 				('AIFF')
	#define PCM_ID_AIFC 				('AIFC')
	#define PCM_ID_FORMAT_VERSION 		('FVER')
	#define PCM_ID_COMMON 				('COMM')
	#define PCM_ID_FORM 				('FORM')
	#define PCM_ID_SOUND_DATA 			('SSND')
	#define PCM_ID_ADPCM 				('APCM')
	#define PCM_ID_MARKER 				('MARK')
	#define PCM_ID_INS_TRUMENT 			('INST')
	#define PCM_ID_MIDI_DATA 			('MIDI')
	#define PCM_ID_AUDIO_RECORDING 		('AESD')
	#define PCM_ID_APPLICATION_SPECIFIC	('APPL')
	#define PCM_ID_COMMENT 				('COMT')
	#define PCM_ID_NAME 				('NAME')
	#define PCM_ID_AUTHOR 				('AUTH')
	#define PCM_ID_COPYRIGHT 			('(c) ')
	#define PCM_ID_ANNOTATION 			('ANNO')
#endif

/*
enum LoopMode {
	NoLooping = 0,
	ForwardLooping = 1,
	ForwardBackwardLooping = 2,
	LoopModeEnd
};
*/

/* AIFF-C Versions */
#define	PCM_AIFC_VERSION1 0xA2805140

/* Compression Names */
#define PCM_NAME_NONE 				"\pnot compressed"
#define PCM_NAME_ACE2TO1 			"\pACE 2-to-1"
#define PCM_NAME_ACE8TO3 			"\pACE 8-to-3"
#define PCM_NAME_MACE3TO1 			"\pMACE 3-to-1"
#define PCM_NAME_MACE6TO1 			"\pMACE 6-to-1"

/* Compression Types */
#define PCM_TYPE_NONE 				(*(unsigned long *)"NONE")
#define PCM_TYPE_ACE2 				(*(unsigned long *)"ACE2")
#define PCM_TYPE_ACE8 				(*(unsigned long *)"ACE8")
#define PCM_TYPE_MACE3 				(*(unsigned long *)"MAC3")
#define PCM_TYPE_MACE6 				(*(unsigned long *)"MAC6")
#define PCM_TYPE_IMA4 				(*(unsigned long *)"ima4")



/*-------------------------- 《データ型》 --------------------------*/

typedef unsigned long PcmId;
typedef short PcmMarkerIdType;

typedef struct { short exp[1], man[4]; } PcmExtended80;

struct PcmChunkHeader {
	PcmId 			ck_id;
	long 			ck_size;
};

typedef struct PcmChunkHeader PcmChunkHeader;

struct PcmContainerChunk {
	PcmId 			ck_id;
	long 			ck_size;
	PcmId 			form_type;
};

typedef struct PcmContainerChunk PcmContainerChunk;

struct PcmFormatVersionChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned long 	timestamp;
};

typedef struct PcmFormatVersionChunk PcmFormatVersionChunk;
typedef PcmFormatVersionChunk *PcmFormatVersionChunkPtr;

struct PcmCommonChunk {
	PcmId 			ck_id;
	long 			ck_size;
	short 			num_channels;
/*	unsigned long 	num_sample_frames; */
	short 			num_sample_frames;
	short 			_num_sample_frames;
	short 			sample_size;
	PcmExtended80 	sample_rate;
};

typedef struct PcmCommonChunk PcmCommonChunk;
typedef PcmCommonChunk *PcmCommonChunkPtr;

struct PcmExtCommonChunk {
	PcmId 			ck_id;
	long 			ck_size;
	short 			num_channels;
/*	unsigned long 	num_sample_frames; */
	short 			num_sample_frames;
	short 			_num_sample_frames;
	short 			sample_size;
	PcmExtended80 	sample_rate;
/*	PcmId 			compression_type; */
	short 			compression_type;
	short 			_compression_type;
	char 			compression_name[1];
};

typedef struct PcmExtCommonChunk PcmExtCommonChunk;
typedef PcmExtCommonChunk *PcmExtCommonChunkPtr;

struct PcmSoundDataChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned long 	offset;
	unsigned long 	block_size;
};

typedef struct PcmSoundDataChunk PcmSoundDataChunk;
typedef PcmSoundDataChunk *PcmSoundDataChunkPtr;

/* SEGA ADPCM ENCODER ORIGINAL */
struct PcmAdpcmChunk {
	PcmId 			ck_id;			/* "APCM" */
	long 			ck_size;
	unsigned long 	reserved1;
	unsigned long 	reserved2;
/*	char 			pad[PCM_SIZE_SECTOR - (
											sizeof(PcmContainerChunk) + 
											sizeof(PcmCommonChunk) +
											4 * 4 +
											)];

*/

};

typedef struct PcmAdpcmChunk PcmAdpcmChunk;
typedef PcmAdpcmChunk *PcmAdpcmChunkPtr;

/* 1[sound group] = 128[byte] = 224[sample data] */
struct PcmSoundGroup {
	char			fr[4 * 4];		/* filter/range 	*/
	char			sd[4 * 28];		/* sound data 		*/
};

typedef struct PcmSoundGroup PcmSoundGroup;
typedef PcmSoundGroup *PcmSoundGroupPtr;

/* CD-ROM XA Audio sector 2324 [byte] = 18 [sound group] + 20 [byte] */
struct PcmAdpcmSectorChunk {
	PcmSoundGroup	sg[18];
	char			pad[20];
};

typedef struct PcmAdpcmSectorChunk PcmAdpcmSectorChunk;
typedef PcmAdpcmSectorChunk *PcmAdpcmSectorChunkPtr;

struct PcmMarker {
	PcmMarkerIdType id;
 	unsigned long 	position;
	unsigned char 	marker_name[256];
};

typedef struct PcmMarker PcmMarker;

struct PcmMarkerChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned short 	num_markers;
	PcmMarker 		markers[1];
};

typedef struct PcmMarkerChunk PcmMarkerChunk;
typedef PcmMarkerChunk *PcmMarkerChunkPtr;

struct PcmAiffLoop {
	short 			play_mode;
	PcmMarkerIdType begin_loop;
	PcmMarkerIdType end_loop;
};

typedef struct PcmAiffLoop PcmAiffLoop;

struct InstrumentChunk {
	PcmId 			ck_id;
	long 			ck_size;
	char 			base_frequency;
	char 			detune;
	char 			low_frequency;
	char 			high_frequency;
	char 			low_velocity;
	char 			high_velocity;
 	short 			gain;
	PcmAiffLoop 	sustain_loop;
	PcmAiffLoop 	release_loop;
};

typedef struct InstrumentChunk InstrumentChunk;
typedef InstrumentChunk *InstrumentChunkPtr;

struct PcmMidiDataChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned char 	midi_data[1];
};

typedef struct PcmMidiDataChunk PcmMidiDataChunk;
typedef PcmMidiDataChunk *PcmMidiDataChunkPtr;

struct PcmAudioRecordingChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned char 	aes_channel_status[24];
};

typedef struct PcmAudioRecordingChunk PcmAudioRecordingChunk;
typedef PcmAudioRecordingChunk *PcmAudioRecordingChunkPtr;

struct PcmApplicationSpecificChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned long 	application_signature;
	char 			data[1];
};

typedef struct PcmApplicationSpecificChunk PcmApplicationSpecificChunk;
typedef PcmApplicationSpecificChunk *PcmApplicationSpecificChunkPtr;

struct PcmComment {
	unsigned long 	timeStamp;
	PcmMarkerIdType marker;
	unsigned short 	count;
	char 			text[1];
};

typedef struct PcmComment PcmComment;

struct PcmCommentsChunk {
	PcmId 			ck_id;
	long 			ck_size;
	unsigned short 	numComments;
	PcmComment 		comments[1];
};

typedef struct PcmCommentsChunk PcmCommentsChunk;
typedef PcmCommentsChunk *PcmCommentsChunkPtr;

struct PcmTextChunk {
	PcmId 			ck_id;
	long 			ck_size;
	char 			text[1];
};

typedef struct PcmTextChunk PcmTextChunk;
typedef PcmTextChunk *PcmTextChunkPtr;

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ほかのファイルから参照されているので追加
	*/
	Sint32	pcm_AifHeaderProcess( PcmHn	hn );
#endif

#endif
