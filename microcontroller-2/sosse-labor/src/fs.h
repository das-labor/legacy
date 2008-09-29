/*
	Simple Operating System for Smartcard Education
	Copyright (C) 2002  Matthias Bruestle <m@mbsks.franken.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
	\brief File system declarations.

	SOSSE implements a very simple file system. Each file starts with
	following header:

	- 2 Byte: Content size
	- 2 Byte: ID
	- 1 Byte: Type
	- 1 Byte: Access conditions

	Directly after that the content area is located. If the file is a
	DF, this content area is directly used to add header/content
	sequences.

	After the content area of a file, a content size of 0 must be
	writen as an end of data marker. This means the data capacity
	of a MF or DF is the total content size of it minus 2.

	Currently the file system support only one level of DFs, i.e.
	the maximum depth is MF:DF:EF.

	There are probably a lot optimization possibilities in this
	file system, but I think this is not bad for my first designed and
	implemented file system.

	\todo
	Here are certainly some optimizations possible to reduce code size.

	$Id: fs.h,v 1.13 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_FS_H
#define SOSSE_FS_H

#include <types.h>
#include <fstream.h>

/*! \brief File path specifier.

	\todo
	There is probably a potential for simplification, when the file
	type (DF/EF) is not handled so rigid.
*/
typedef struct s_fpath {
	uint16_t df;	//!< ID of one DF level. Set to FFFF if unused.
	uint16_t ef;	//!< IF of the EF. Set to FFFF if unused.
} S_FPATH;

/*! \brief File info struct. This is directly written as a file header in the
	file system.
*/
typedef struct s_finfo {
	uint16_t size;	//!< Size of the file content.
	uint16_t fid;		//!< ID of the file.
	uint8_t type;		//!< Type of the file. (FS_TYPE_*)
	uint8_t ac[1];	//!< Access conditions of the file. (See CREATE FILE dox.)
} S_FINFO;

//! File type DF
#define FS_TYPE_DF	0x38
//! File type EF
#define FS_TYPE_EF	0x00

//! Size of the file header, which is the same as S_FINFO.
#define FS_HEADER_SIZE	sizeof(S_FINFO)

/*! \brief Path specified of the currently selected file.

	\todo
	Saving here a S_FINFO and/or S_FSTREAM of the selected file might
	reduce the code size.
*/
extern S_FPATH selected;

/*! \brief Initialize file system variables.

	This looks e.g. for the start of the file system and it's size.
	It must be called after each reset.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_init( void );
/*! \brief Look for a file in a stream.

    \param fid File ID to look for.
    \param fs Pointer to S_FSTREAM, where the FID is searched for. It returns
		on success a stream on the file found.
	\param fi Pointer to S_FINFO, which will be filled with data on success.
	\param type Type of file wanted.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.

	\todo
	There is probably a potential for simplification, when the file
	type is not handled so rigid.
*/
bool fs_seek( uint16_t fid, S_FSTREAM *fs, S_FINFO *fi, uint8_t type );
/*! \brief Look for the end of files in the stream.

    \param fs Pointer to S_FSTREAM, which supplies a directory stream and
		which will be used to return the end position on success.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_seekEnd( S_FSTREAM *fs );
/*! \brief Get stream and info for a file specified as a path.

    \param fp Pointer to S_FPATH, which is used to supply the information
		about the file wanted.
	\param fs Pointer to S_FSTREAM, which will be filled with data on success.
	\param fi Pointer to S_FINFO, which will be filled with data on success.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_getData( S_FPATH *fp, S_FSTREAM *fs, S_FINFO *fi );
/*! \brief Get path, stream and info for a file specified as a FID.

	The file is searched for in the order described in the SELECT FILE
	documentation on the main page.

    \param fid File ID to look for.
	\param fp Pointer to S_FPATH, which will be filled with data on success.
	\param fs Pointer to S_FSTREAM, which will be filled with data on success.
		This can be NULL when no data is wanted.
	\param fi Pointer to S_FINFO, which will be filled with data on success.
		This can be NULL when no data is wanted.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_getFPath( uint16_t fid, S_FPATH *fp, S_FSTREAM *fs, S_FINFO *fi );
/*! \brief Delete file specified as fid.

	The function looks for the fid in the current file, which must be
	a MF or DF. The file, which should be deleted must be the last one
	in the current MF or DF.

    \param fid ID of the file to delete.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_delete( uint16_t fid );
/*! \brief The function creates a file.

	The functions creates a file with the data in <EM>fi</EM>. The
	current file must be a MF or DF. In case the current file is a
	DF, the file, which should be created, can't be a DF.

    \param fi Pointer to S_FINFO, which contains the data of the file to
		create.

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fs_create( S_FINFO *fi );

#endif /* SOSSE_FS_H */

