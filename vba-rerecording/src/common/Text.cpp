/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Ben Parnell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Code originally from fceu/drawing.h file, adapted by Forgotten
 */
#include "System.h"

bool outlinedText = true, transparentText = false;
// textMethod:
// 0 - "in-game"; 1 - "on-game"; 2 - "on-screen"
int  textColor = 0, textMethod = 1;

extern u32 RGB_LOW_BITS_MASK;

static const u8 fontdata2[2048] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x81, 0xa5, 0x81, 0xbd, 0x99, 0x81, 0x7e, 0x7e, 0xff,  0xdb, 0xff,
	0xc3, 0xe7, 0xff, 0x7e, 0x36, 0x7f, 0x7f, 0x7f, 0x3e, 0x1c, 0x08, 0x00, 0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x08, 0x00,
	0x1c,
	0x3e, 0x1c, 0x7f, 0x7f, 0x3e, 0x1c, 0x3e, 0x08, 0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x3e, 0x00, 0x00, 0x18, 0x3c, 0x3c,
	0x18,
	0x00, 0x00, 0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff, 0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00,  0xff, 0xc3,
	0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff, 0xf0, 0xe0, 0xf0, 0xbe, 0x33, 0x33, 0x33, 0x1e, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x18,
	0x7e,
	0x18, 0xfc, 0xcc, 0xfc, 0x0c, 0x0c, 0x0e, 0x0f, 0x07, 0xfe, 0xc6, 0xfe, 0xc6, 0xc6, 0xe6, 0x67, 0x03, 0x99, 0x5a, 0x3c,
	0xe7,
	0xe7, 0x3c, 0x5a, 0x99, 0x01, 0x07, 0x1f, 0x7f, 0x1f, 0x07, 0x01, 0x00, 0x40, 0x70, 0x7c, 0x7f, 0x7c, 0x70, 0x40, 0x00,
	0x18,
	0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 0xfe, 0xdb, 0xdb, 0xde, 0xd8,
	0xd8,
	0xd8, 0x00, 0x7c, 0xc6, 0x1c, 0x36, 0x36, 0x1c, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x00, 0x18, 0x3c,
	0x7e,
	0x18, 0x7e, 0x3c, 0x18, 0xff, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18,
	0x00,
	0x00, 0x18, 0x30, 0x7f, 0x30, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x06, 0x7f, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
	0x03,
	0x7f, 0x00, 0x00, 0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00, 0x00,
	0xff,
	0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1e, 0x1e, 0x0c, 0x0c, 0x00, 0x0c, 0x00, 0x36, 0x36, 0x36, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x7f, 0x36, 0x7f, 0x36, 0x36, 0x00, 0x0c, 0x3e, 0x03, 0x1e, 0x30, 0x1f, 0x0c, 0x00,
	0x00,
	0x63, 0x33, 0x18, 0x0c, 0x66, 0x63, 0x00, 0x1c, 0x36, 0x1c, 0x6e, 0x3b, 0x33, 0x6e, 0x00, 0x06, 0x06, 0x03, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x18, 0x0c, 0x06, 0x06, 0x06, 0x0c, 0x18, 0x00, 0x06, 0x0c, 0x18, 0x18, 0x18, 0x0c, 0x06, 0x00, 0x00, 0x66,
	0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
	0x0c,
	0x06, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x60, 0x30, 0x18,
	0x0c,
	0x06, 0x03, 0x01, 0x00, 0x3e, 0x63, 0x73, 0x7b, 0x6f, 0x67, 0x3e, 0x00, 0x0c, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x00,
	0x1e,
	0x33, 0x30, 0x1c, 0x06, 0x33, 0x3f, 0x00, 0x1e, 0x33, 0x30, 0x1c, 0x30, 0x33, 0x1e, 0x00, 0x38, 0x3c, 0x36, 0x33, 0x7f,
	0x30,
	0x78, 0x00, 0x3f, 0x03, 0x1f, 0x30, 0x30, 0x33, 0x1e, 0x00, 0x1c, 0x06, 0x03, 0x1f, 0x33, 0x33, 0x1e, 0x00, 0x3f, 0x33,
	0x30,
	0x18, 0x0c, 0x0c, 0x0c, 0x00, 0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x1e, 0x00, 0x1e, 0x33, 0x33, 0x3e, 0x30, 0x18, 0x0e,
	0x00,
	0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x06, 0x18, 0x0c, 0x06, 0x03,
	0x06,
	0x0c, 0x18, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00, 0x1e,
	0x33,
	0x30, 0x18, 0x0c, 0x00, 0x0c, 0x00,
	0x3e, 0x63, 0x7b, 0x7b, 0x7b, 0x03, 0x1e, 0x00, 0x0c, 0x1e, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x00, 0x3f, 0x66, 0x66, 0x3e,
	0x66, 0x66, 0x3f, 0x00, 0x3c, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3c, 0x00, 0x1f, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1f, 0x00,
	0x7f,
	0x46, 0x16, 0x1e, 0x16, 0x46, 0x7f, 0x00, 0x7f, 0x46, 0x16, 0x1e, 0x16, 0x06, 0x0f, 0x00, 0x3c, 0x66, 0x03, 0x03, 0x73,
	0x66,
	0x7c, 0x00, 0x33, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x33, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x78, 0x30,
	0x30, 0x30, 0x33, 0x33, 0x1e, 0x00, 0x67, 0x66, 0x36, 0x1e, 0x36, 0x66, 0x67, 0x00, 0x0f, 0x06, 0x06, 0x06, 0x46, 0x66,
	0x7f,
	0x00, 0x63, 0x77, 0x7f, 0x7f, 0x6b, 0x63, 0x63, 0x00, 0x63, 0x67, 0x6f, 0x7b, 0x73, 0x63, 0x63, 0x00, 0x1c, 0x36, 0x63,
	0x63,
	0x63, 0x36, 0x1c, 0x00, 0x3f, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x0f, 0x00, 0x1e, 0x33, 0x33, 0x33, 0x3b, 0x1e, 0x38, 0x00,
	0x3f,
	0x66, 0x66, 0x3e, 0x36, 0x66, 0x67, 0x00, 0x1e, 0x33, 0x07, 0x0e, 0x38, 0x33, 0x1e, 0x00, 0x3f, 0x2d, 0x0c, 0x0c, 0x0c,
	0x0c,
	0x1e, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x00, 0x63, 0x63,
	0x63,
	0x6b, 0x7f, 0x77, 0x63, 0x00, 0x63, 0x63, 0x36, 0x1c, 0x1c, 0x36, 0x63, 0x00, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x0c, 0x1e,
	0x00,
	0x7f, 0x63, 0x31, 0x18, 0x4c, 0x66, 0x7f, 0x00, 0x1e, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1e, 0x00, 0x03, 0x06, 0x0c, 0x18,
	0x30,
	0x60, 0x40, 0x00, 0x1e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1e, 0x00, 0x08, 0x1c, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x0c, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x30, 0x3e, 0x33, 0x6e, 0x00, 0x07, 0x06, 0x06, 0x3e,
	0x66, 0x66, 0x3b, 0x00, 0x00, 0x00, 0x1e, 0x33, 0x03, 0x33, 0x1e, 0x00, 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6e, 0x00,
	0x00,
	0x00, 0x1e, 0x33, 0x3f, 0x03, 0x1e, 0x00, 0x1c, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0f, 0x00, 0x00, 0x00, 0x6e, 0x33, 0x33,
	0x3e,
	0x30, 0x1f, 0x07, 0x06, 0x36, 0x6e, 0x66, 0x66, 0x67, 0x00, 0x0c, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x30, 0x00,
	0x30, 0x30, 0x30, 0x33, 0x33, 0x1e, 0x07, 0x06, 0x66, 0x36, 0x1e, 0x36, 0x67, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x1e,
	0x00, 0x00, 0x00, 0x33, 0x7f, 0x7f, 0x6b, 0x63, 0x00, 0x00, 0x00, 0x1f, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x1e,
	0x33,
	0x33, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x3b, 0x66, 0x66, 0x3e, 0x06, 0x0f, 0x00, 0x00, 0x6e, 0x33, 0x33, 0x3e, 0x30, 0x78,
	0x00,
	0x00, 0x3b, 0x6e, 0x66, 0x06, 0x0f, 0x00, 0x00, 0x00, 0x3e, 0x03, 0x1e, 0x30, 0x1f, 0x00, 0x08, 0x0c, 0x3e, 0x0c, 0x0c,
	0x2c,
	0x18, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6e, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x00, 0x00, 0x00,
	0x63,
	0x6b, 0x7f, 0x7f, 0x36, 0x00, 0x00, 0x00, 0x63, 0x36, 0x1c, 0x36, 0x63, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x3e, 0x30,
	0x1f,
	0x00, 0x00, 0x3f, 0x19, 0x0c, 0x26, 0x3f, 0x00, 0x38, 0x0c, 0x0c, 0x07, 0x0c, 0x0c, 0x38, 0x00, 0x18, 0x18, 0x18, 0x00,
	0x18,
	0x18, 0x18, 0x00, 0x07, 0x0c, 0x0c, 0x38, 0x0c, 0x0c, 0x07, 0x00, 0x6e, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08,
	0x1c, 0x36, 0x63, 0x63, 0x7f, 0x00,
	0x1e, 0x33, 0x03, 0x33, 0x1e, 0x18, 0x30, 0x1e, 0x00, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7e, 0x00, 0x38, 0x00, 0x1e, 0x33,
	0x3f, 0x03, 0x1e, 0x00, 0x7e, 0xc3, 0x3c, 0x60, 0x7c, 0x66, 0xfc, 0x00, 0x33, 0x00, 0x1e, 0x30, 0x3e, 0x33, 0x7e, 0x00,
	0x07,
	0x00, 0x1e, 0x30, 0x3e, 0x33, 0x7e, 0x00, 0x0c, 0x0c, 0x1e, 0x30, 0x3e, 0x33, 0x7e, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x03,
	0x1e,
	0x30, 0x1c, 0x7e, 0xc3, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00, 0x33, 0x00, 0x1e, 0x33, 0x3f, 0x03, 0x1e, 0x00, 0x07, 0x00,
	0x1e, 0x33, 0x3f, 0x03, 0x1e, 0x00, 0x33, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x3e, 0x63, 0x1c, 0x18, 0x18, 0x18,
	0x3c,
	0x00, 0x07, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x63, 0x1c, 0x36, 0x63, 0x7f, 0x63, 0x63, 0x00, 0x0c, 0x0c, 0x00,
	0x1e,
	0x33, 0x3f, 0x33, 0x00, 0x38, 0x00, 0x3f, 0x06, 0x1e, 0x06, 0x3f, 0x00, 0x00, 0x00, 0xfe, 0x30, 0xfe, 0x33, 0xfe, 0x00,
	0x7c,
	0x36, 0x33, 0x7f, 0x33, 0x33, 0x73, 0x00, 0x1e, 0x33, 0x00, 0x1e, 0x33, 0x33, 0x1e, 0x00, 0x00, 0x33, 0x00, 0x1e, 0x33,
	0x33,
	0x1e, 0x00, 0x00, 0x07, 0x00, 0x1e, 0x33, 0x33, 0x1e, 0x00, 0x1e, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7e, 0x00, 0x00, 0x07,
	0x00,
	0x33, 0x33, 0x33, 0x7e, 0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x3e, 0x30, 0x1f, 0xc3, 0x18, 0x3c, 0x66, 0x66, 0x3c, 0x18,
	0x00,
	0x33, 0x00, 0x33, 0x33, 0x33, 0x33, 0x1e, 0x00, 0x18, 0x18, 0x7e, 0x03, 0x03, 0x7e, 0x18, 0x18, 0x1c, 0x36, 0x26, 0x0f,
	0x06,
	0x67, 0x3f, 0x00, 0x33, 0x33, 0x1e, 0x3f, 0x0c, 0x3f, 0x0c, 0x0c, 0x1f, 0x33, 0x33, 0x5f, 0x63, 0xf3, 0x63, 0xe3, 0x70,
	0xd8,
	0x18, 0x3c, 0x18, 0x18, 0x1b, 0x0e,
	0x38, 0x00, 0x1e, 0x30, 0x3e, 0x33, 0x7e, 0x00, 0x1c, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x00, 0x38, 0x00, 0x1e,
	0x33, 0x33, 0x1e, 0x00, 0x00, 0x38, 0x00, 0x33, 0x33, 0x33, 0x7e, 0x00, 0x00, 0x1f, 0x00, 0x1f, 0x33, 0x33, 0x33, 0x00,
	0x3f,
	0x00, 0x33, 0x37, 0x3f, 0x3b, 0x33, 0x00, 0x3c, 0x36, 0x36, 0x7c, 0x00, 0x7e, 0x00, 0x00, 0x1c, 0x36, 0x36, 0x1c, 0x00,
	0x3e,
	0x00, 0x00, 0x0c, 0x00, 0x0c, 0x06, 0x03, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x30, 0x30, 0x00, 0x00, 0xc3, 0x63, 0x33, 0x7b, 0xcc, 0x66, 0x33, 0xf0, 0xc3, 0x63, 0x33, 0xdb, 0xec, 0xf6,
	0xf3,
	0xc0, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0xcc, 0x66, 0x33, 0x66, 0xcc, 0x00, 0x00, 0x00, 0x33, 0x66,
	0xcc,
	0x66, 0x33, 0x00, 0x00, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	0xdb,
	0xee, 0xdb, 0x77, 0xdb, 0xee, 0xdb, 0x77, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f,
	0x18,
	0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18, 0x6c, 0x6c, 0x6c, 0x6c, 0x6f, 0x6c, 0x6c, 0x6c, 0x00, 0x00,
	0x00,
	0x00, 0x7f, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18, 0x6c, 0x6c, 0x6f, 0x60, 0x6f, 0x6c, 0x6c,
	0x6c,
	0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x7f, 0x60, 0x6f, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6f, 0x60,
	0x7f,
	0x00, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0x7f, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1f, 0x18, 0x1f, 0x00, 0x00, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x1f, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
	0x18,
	0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18, 0x6c, 0x6c, 0x6c, 0x6c, 0xec,
	0x6c,
	0x6c, 0x6c, 0x6c, 0x6c, 0xec, 0x0c, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0c, 0xec, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c,
	0xef, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xef, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0xec, 0x0c, 0xec, 0x6c,
	0x6c,
	0x6c, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x6c, 0x6c, 0xef, 0x00, 0xef, 0x6c, 0x6c, 0x6c, 0x18, 0x18, 0xff,
	0x00,
	0xff, 0x00, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x18, 0x18, 0x18,
	0x00,
	0x00, 0x00, 0x00, 0xff, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0xfc, 0x00, 0x00, 0x00, 0x18, 0x18, 0xf8, 0x18, 0xf8,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c,
	0x6c,
	0x6c, 0xff, 0x6c, 0x6c, 0x6c, 0x18, 0x18, 0xff, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0x18, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0xff,
	0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff,
	0xff,
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x6e, 0x3b, 0x13, 0x3b, 0x6e, 0x00, 0x00, 0x1e, 0x33, 0x1f, 0x33, 0x1f, 0x03, 0x03, 0x00, 0x3f, 0x33, 0x03,
	0x03, 0x03, 0x03, 0x00, 0x00, 0x7f, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x3f, 0x33, 0x06, 0x0c, 0x06, 0x33, 0x3f, 0x00,
	0x00,
	0x00, 0x7e, 0x1b, 0x1b, 0x1b, 0x0e, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x06, 0x03, 0x00, 0x6e, 0x3b, 0x18, 0x18,
	0x18,
	0x18, 0x00, 0x3f, 0x0c, 0x1e, 0x33, 0x33, 0x1e, 0x0c, 0x3f, 0x1c, 0x36, 0x63, 0x7f, 0x63, 0x36, 0x1c, 0x00, 0x1c, 0x36,
	0x63, 0x63, 0x36, 0x36, 0x77, 0x00, 0x38, 0x0c, 0x18, 0x3e, 0x33, 0x33, 0x1e, 0x00, 0x00, 0x00, 0x7e, 0xdb, 0xdb, 0x7e,
	0x00,
	0x00, 0x60, 0x30, 0x7e, 0xdb, 0xdb, 0x7e, 0x06, 0x03, 0x1c, 0x06, 0x03, 0x1f, 0x03, 0x06, 0x1c, 0x00, 0x1e, 0x33, 0x33,
	0x33,
	0x33, 0x33, 0x33, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x00, 0x3f, 0x00,
	0x06,
	0x0c, 0x18, 0x0c, 0x06, 0x00, 0x3f, 0x00, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x00, 0x3f, 0x00, 0x70, 0xd8, 0xd8, 0x18, 0x18,
	0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1b, 0x1b, 0x0e, 0x0c, 0x0c, 0x00, 0x3f, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x6e,
	0x3b,
	0x00, 0x6e, 0x3b, 0x00, 0x00, 0x1c, 0x36, 0x36, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xf0, 0x30, 0x30, 0x30, 0x37, 0x36, 0x3c, 0x38, 0x1e, 0x36, 0x36, 0x36,
	0x36,
	0x00, 0x00, 0x00, 0x0e, 0x18, 0x0c, 0x06, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static void calcColors(const int colorNum, int & lo, int & hi, int & out)
{
	int redLo, redHi, greenLo, greenHi, blueLo, blueHi;

	if (colorNum == 0 || colorNum == 1 || colorNum == 2 || colorNum == 6) // white, red, yellow, or magenta
		redLo = (0xf) << systemRedShift, redHi = (0x1f) << systemRedShift;
	else
		redLo = redHi = 0;

	if (colorNum == 0 || colorNum == 2 || colorNum == 3 || colorNum == 4) // white, yellow, green, or cyan
		greenLo = (0xf) << systemGreenShift, greenHi = (0x1f) << systemGreenShift;
	else
		greenLo = greenHi = 0;

	if (colorNum == 0 || colorNum == 4 || colorNum == 5 || colorNum == 6) // white, cyan, blue, or magenta
		blueLo = (0xf) << systemBlueShift, blueHi = (0x1f) << systemBlueShift;
	else
		blueLo = blueHi = 0;

	lo = redLo + greenLo + blueLo;
	hi = redHi + greenHi + blueHi;

	if (colorNum == 7) // black
		out = 0xffffffff; // white border
	else
		out = 0;  // black border
}

int lastColID = 0;
static void progressColorList(const char *& colorList, int & lo, int & hi, int & out)
{
	if (*colorList)
	{
		if (*colorList != lastColID)
		{
			calcColors((int)(*colorList)-1, lo, hi, out);
			lastColID = *colorList;
		}
		colorList++;
	}
	else
	{
		colorList = NULL;
	}
}

static void drawTextInternal(u8 *screen, int pitch, int x, int y,
                             const char *string, bool trans, const char *colorList = NULL)
{
	if (colorList && !*colorList)
		colorList = NULL;

	int loCol, hiCol, outCol;
	calcColors(textColor, loCol, hiCol, outCol);

	lastColID = 0;

	const static int xd [8] = {-1, 0, 1, 1, 1, 0, -1, -1};
	const static int yd [8] = {-1, -1, -1, 0, 1, 1, 1, 0};

	screen += y*pitch;
	int inc = 2;
	switch (systemColorDepth)
	{
	case 24:
		inc = 3;
		break;
	case 32:
		inc = 4;
		break;
	}
	screen += x*inc;

	int xpos = x;
	switch (systemColorDepth)
	{
	case 16:
	{
		while (*string)
		{
			char c   = *string++;
			u8 * scr = screen;

			if (colorList)
				progressColorList(colorList, loCol, hiCol, outCol);

			u16  mask = u16(~RGB_LOW_BITS_MASK);
			u16 *s = (u16 *)scr;
			for (int h = 0-1; h < 8+1; h++)
			{
				for (int w = 0-1; w < 8+1; w++, s++)
				{
					int on = (h < 0 || w < 0 || h >= 8 || w >= 8) ? 0 : (fontdata2[(c<<3)+h]>>w)&1;

					int border = 0;
					if (outlinedText)
						for (int i = 0; i < 8; i++)
						{
							int h2 = h+yd[i], w2 = w+xd[i];
							border = (h2 < 0 || w2 < 0 || h2 >= 8 || w2 >= 8) ? 0 : (fontdata2[(c<<3)+h2]>>w2)&1;
							if (border)
								break;
						}

					if (trans)
					{
						if (on)
							*s = loCol +
							     ((*s & mask) >>1);
						else if (border)
						{
							*s = outCol +
							     ((*s & mask) >>1);
						}
					}
					else
					{
						if (on)
							*s = hiCol;
						else if (border)
							*s = outCol;
					}
				}
				scr += pitch;
				s    = (u16 *)scr;
			}
			screen += inc*8;

			xpos += 8;
			if (xpos+8 > pitch>>1) // cut off text at right boundary
				break;
		}
		break;
	}
	case 24: // TODO: verify this code's correctness
	{
		while (*string)
		{
			char c   = *string++;
			u8 * scr = screen;

			if (colorList)
				progressColorList(colorList, loCol, hiCol, outCol);

			int h, w;
			u8 *s = (u8 *)scr;
			for (h = 0-1; h < 8+1; h++)
			{
				for (w = 0-1; w < 8+1; w++, s++)
				{
					int on = (h < 0 || w < 0 || h >= 8 || w >= 8) ? 0 : (fontdata2[(c<<3)+h]>>w)&1;

					int border = 0;
					if (outlinedText)
						for (int i = 0; i < 8; i++)
						{
							int h2 = h+yd[i], w2 = w+xd[i];
							border = (h2 < 0 || w2 < 0 || h2 >= 8 || w2 >= 8) ? 0 : (fontdata2[(c<<3)+h2]>>w2)&1;
							if (border)
								break;
						}

					if (trans)
					{
						if (on)
						{
							u32 color = hiCol;
							*s     = ((color & 255)>>1)+(*s>>1);
							*(s+1) = (((color >> 8) & 255)>>1)+(*(s+1)>>1);
							*(s+2) = (((color >> 16) & 255)>>1)+(*(s+2)>>1);
						}
						else if (border)
						{
							u32 color = outCol;
							*s     = ((color & 255)>>1)+(*s>>1);
							*(s+1) = (((color >> 8) & 255)>>1)+(*(s+1)>>1);
							*(s+2) = (((color >> 16) & 255)>>1)+(*(s+2)>>1);
						}
					}
					else
					{
						if (on)
						{
							u32 color = hiCol;
							*s     = (color & 255);
							*(s+1) = (color >> 8) & 255;
							*(s+2) = (color >> 16) & 255;
						}
						else if (border)
						{
							u32 color = outCol;
							*s     = (color & 255);
							*(s+1) = (color >> 8) & 255;
							*(s+2) = (color >> 16) & 255;
						}
					}
				}
				scr += pitch;
				s    = (u8 *)scr;
			}
			screen += inc*8;

			xpos += 8;
			if (xpos+8 > pitch/3) // cut off text at right boundary
				break;
		}
		break;
	}
	case 32:
	{
		while (*string)
		{
			char c   = *string++;
			u8 * scr = screen;

			if (colorList)
				progressColorList(colorList, loCol, hiCol, outCol);

			int  h, w;
			u32  mask = 0xfefefe;
			u32 *s    = (u32 *)scr;
			for (h = 0-1; h < 8+1; h++)
			{
				for (w = 0-1; w < 8+1; w++, s++)
				{
					int on = (h < 0 || w < 0 || h >= 8 || w >= 8) ? 0 : (fontdata2[(c<<3)+h]>>w)&1;

					int border = 0;
					if (outlinedText)
						for (int i = 0; i < 8; i++)
						{
							int h2 = h+yd[i], w2 = w+xd[i];
							border = (h2 < 0 || w2 < 0 || h2 >= 8 || w2 >= 8) ? 0 : (fontdata2[(c<<3)+h2]>>w2)&1;
							if (border)
								break;
						}

					if (trans)
					{
						if (on)
							*s = loCol +
							     ((*s & mask)>>1);
						else if (border)
						{
							*s = outCol +
							     ((*s & mask)>>1);
						}
					}
					else
					{
						if (on)
							*s = hiCol;
						else if (border)
							*s = outCol;
					}
				}
				scr += pitch;
				s    = (u32 *)scr;
			}
			screen += inc*8;

			xpos += 8;
			if (xpos+8 > pitch>>2) // cut off text at right boundary
				break;
		}
		break;
	}
	}
}

void drawText(u8 *screen, int pitch, int x, int y, const char *string, const char *colorList)
{
	drawTextInternal(screen, pitch, x, y, string, transparentText, colorList);
}

void drawTextTransp(u8 *screen, int pitch, int x, int y, const char *string, const char *colorList)
{
	drawTextInternal(screen, pitch, x, y, string, true, colorList);
}

