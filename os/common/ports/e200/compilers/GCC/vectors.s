/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    vectors.s
 * @brief   SPC56x vectors table.
 *
 * @addtogroup PPC_CORE
 * @{
 */

#include "ppcparams.h"

#if !defined(__DOXYGEN__)

        /* Software vectors table. The vectors are accessed from the IVOR4
           handler only. In order to declare an interrupt handler just create
           a function withe the same name of a vector, the symbol will
           override the weak symbol declared here.*/
        .section    .vectors, "ax"
        .align		4
        .globl      _vectors
_vectors:
        .long       vector0,    vector1,    vector2,    vector3
#if PPC_VECTORS_NUMBER > 4
        .long       vector4,    vector5,    vector6,    vector7
#endif
#if PPC_VECTORS_NUMBER > 8
        .long       vector8,    vector9,    vector10,   vector11
#endif
#if PPC_VECTORS_NUMBER > 12
        .long       vector12,   vector13,   vector14,   vector15
#endif
#if PPC_VECTORS_NUMBER > 16
        .long       vector16,   vector17,   vector18,   vector19
#endif
#if PPC_VECTORS_NUMBER > 20
        .long       vector20,   vector21,   vector22,   vector23
#endif
#if PPC_VECTORS_NUMBER > 24
        .long       vector24,   vector25,   vector26,   vector27
#endif
#if PPC_VECTORS_NUMBER > 28
        .long       vector28,   vector29,   vector30,   vector31
#endif
#if PPC_VECTORS_NUMBER > 32
        .long       vector32,   vector33,   vector34,   vector35
#endif
#if PPC_VECTORS_NUMBER > 36
        .long       vector36,   vector37,   vector38,   vector39
#endif
#if PPC_VECTORS_NUMBER > 40
        .long       vector40,   vector41,   vector42,   vector43
#endif
#if PPC_VECTORS_NUMBER > 44
        .long       vector44,   vector45,   vector46,   vector47
#endif
#if PPC_VECTORS_NUMBER > 48
        .long       vector48,   vector49,   vector50,   vector51
#endif
#if PPC_VECTORS_NUMBER > 52
        .long       vector52,   vector53,   vector54,   vector55
#endif
#if PPC_VECTORS_NUMBER > 56
        .long       vector56,   vector57,   vector58,   vector59
#endif
#if PPC_VECTORS_NUMBER > 60
        .long       vector60,   vector61,   vector62,   vector63
#endif
#if PPC_VECTORS_NUMBER > 64
        .long       vector64,   vector65,   vector66,   vector67
#endif
#if PPC_VECTORS_NUMBER > 68
        .long       vector68,   vector69,   vector70,   vector71
#endif
#if PPC_VECTORS_NUMBER > 72
        .long       vector72,   vector73,   vector74,   vector75
#endif
#if PPC_VECTORS_NUMBER > 76
        .long       vector76,   vector77,   vector78,   vector79
#endif
#if PPC_VECTORS_NUMBER > 80
        .long       vector80,   vector81,   vector82,   vector83
#endif
#if PPC_VECTORS_NUMBER > 84
        .long       vector84,   vector85,   vector86,   vector87
#endif
#if PPC_VECTORS_NUMBER > 88
        .long       vector88,   vector89,   vector90,   vector91
#endif
#if PPC_VECTORS_NUMBER > 92
        .long       vector92,   vector93,   vector94,   vector95
#endif
#if PPC_VECTORS_NUMBER > 96
        .long       vector96,   vector97,   vector98,   vector99
#endif
#if PPC_VECTORS_NUMBER > 100
        .long       vector100,  vector101,  vector102,  vector103
#endif
#if PPC_VECTORS_NUMBER > 104
        .long       vector104,  vector105,  vector106,  vector107
#endif
#if PPC_VECTORS_NUMBER > 108
        .long       vector108,  vector109,  vector110,  vector111
#endif
#if PPC_VECTORS_NUMBER > 112
        .long       vector112,  vector113,  vector114,  vector115
#endif
#if PPC_VECTORS_NUMBER > 116
        .long       vector116,  vector117,  vector118,  vector119
#endif
#if PPC_VECTORS_NUMBER > 120
        .long       vector120,  vector121,  vector122,  vector123
#endif
#if PPC_VECTORS_NUMBER > 124
        .long       vector124,  vector125,  vector126,  vector127
#endif
#if PPC_VECTORS_NUMBER > 128
        .long       vector128,  vector129,  vector130,  vector131
#endif
#if PPC_VECTORS_NUMBER > 132
        .long       vector132,  vector133,  vector134,  vector135
#endif
#if PPC_VECTORS_NUMBER > 136
        .long       vector136,  vector137,  vector138,  vector139
#endif
#if PPC_VECTORS_NUMBER > 140
        .long       vector140,  vector141,  vector142,  vector143
#endif
#if PPC_VECTORS_NUMBER > 144
        .long       vector144,  vector145,  vector146,  vector147
#endif
#if PPC_VECTORS_NUMBER > 148
        .long       vector148,  vector149,  vector150,  vector151
#endif
#if PPC_VECTORS_NUMBER > 152
        .long       vector152,  vector153,  vector154,  vector155
#endif
#if PPC_VECTORS_NUMBER > 156
        .long       vector156,  vector157,  vector158,  vector159
#endif
#if PPC_VECTORS_NUMBER > 160
        .long       vector160,  vector161,  vector162,  vector163
#endif
#if PPC_VECTORS_NUMBER > 164
        .long       vector164,  vector165,  vector166,  vector167
#endif
#if PPC_VECTORS_NUMBER > 168
        .long       vector168,  vector169,  vector170,  vector171
#endif
#if PPC_VECTORS_NUMBER > 172
        .long       vector172,  vector173,  vector174,  vector175
#endif
#if PPC_VECTORS_NUMBER > 176
        .long       vector176,  vector177,  vector178,  vector179
#endif
#if PPC_VECTORS_NUMBER > 180
        .long       vector180,  vector181,  vector182,  vector183
#endif
#if PPC_VECTORS_NUMBER > 184
        .long       vector184,  vector185,  vector186,  vector187
#endif
#if PPC_VECTORS_NUMBER > 188
        .long       vector188,  vector189,  vector190,  vector191
#endif
#if PPC_VECTORS_NUMBER > 192
        .long       vector192,  vector193,  vector194,  vector195
#endif
#if PPC_VECTORS_NUMBER > 196
        .long       vector196,  vector197,  vector198,  vector199
#endif
#if PPC_VECTORS_NUMBER > 200
        .long       vector200,  vector201,  vector202,  vector203
#endif
#if PPC_VECTORS_NUMBER > 204
        .long       vector204,  vector205,  vector206,  vector207
#endif
#if PPC_VECTORS_NUMBER > 208
        .long       vector208,  vector209,  vector210,  vector211
#endif
#if PPC_VECTORS_NUMBER > 212
        .long       vector212,  vector213,  vector214,  vector215
#endif
#if PPC_VECTORS_NUMBER > 216
        .long       vector216,  vector217,  vector218,  vector219
#endif
#if PPC_VECTORS_NUMBER > 220
        .long       vector220,  vector221,  vector222,  vector223
#endif
#if PPC_VECTORS_NUMBER > 224
        .long       vector224,  vector225,  vector226,  vector227
#endif
#if PPC_VECTORS_NUMBER > 228
        .long       vector228,  vector229,  vector230,  vector231
#endif
#if PPC_VECTORS_NUMBER > 232
        .long       vector232,  vector233,  vector234,  vector235
#endif
#if PPC_VECTORS_NUMBER > 236
        .long       vector236,  vector237,  vector238,  vector239
#endif
#if PPC_VECTORS_NUMBER > 240
        .long       vector240,  vector241,  vector242,  vector243
#endif
#if PPC_VECTORS_NUMBER > 244
        .long       vector244,  vector245,  vector246,  vector247
#endif
#if PPC_VECTORS_NUMBER > 248
        .long       vector248,  vector249,  vector250,  vector251
#endif
#if PPC_VECTORS_NUMBER > 252
        .long       vector252,  vector253,  vector254,  vector255
#endif
#if PPC_VECTORS_NUMBER > 256
        .long       vector256,  vector257,  vector258,  vector259
#endif
#if PPC_VECTORS_NUMBER > 260
        .long       vector260,  vector261,  vector262,  vector263
#endif
#if PPC_VECTORS_NUMBER > 264
        .long       vector264,  vector265,  vector266,  vector267
#endif
#if PPC_VECTORS_NUMBER > 268
        .long       vector268,  vector269,  vector270,  vector271
#endif
#if PPC_VECTORS_NUMBER > 272
        .long       vector272,  vector273,  vector274,  vector275
#endif
#if PPC_VECTORS_NUMBER > 276
        .long       vector276,  vector277,  vector278,  vector279
#endif
#if PPC_VECTORS_NUMBER > 280
        .long       vector280,  vector281,  vector282,  vector283
#endif
#if PPC_VECTORS_NUMBER > 284
        .long       vector284,  vector285,  vector286,  vector287
#endif
#if PPC_VECTORS_NUMBER > 288
        .long       vector288,  vector289,  vector290,  vector291
#endif
#if PPC_VECTORS_NUMBER > 292
        .long       vector292,  vector293,  vector294,  vector295
#endif
#if PPC_VECTORS_NUMBER > 296
        .long       vector296,  vector297,  vector298,  vector299
#endif
#if PPC_VECTORS_NUMBER > 300
        .long       vector300,  vector301,  vector302,  vector303
#endif
#if PPC_VECTORS_NUMBER > 304
        .long       vector304,  vector305,  vector306,  vector307
#endif
#if PPC_VECTORS_NUMBER > 308
        .long       vector308,  vector309,  vector310,  vector311
#endif
#if PPC_VECTORS_NUMBER > 312
        .long       vector312,  vector313,  vector314,  vector315
#endif
#if PPC_VECTORS_NUMBER > 316
        .long       vector316,  vector317,  vector318,  vector319
#endif
#if PPC_VECTORS_NUMBER > 320
        .long       vector320,  vector321,  vector322,  vector323
#endif
#if PPC_VECTORS_NUMBER > 324
        .long       vector324,  vector325,  vector326,  vector327
#endif
#if PPC_VECTORS_NUMBER > 328
        .long       vector328,  vector329,  vector330,  vector331
#endif
#if PPC_VECTORS_NUMBER > 332
        .long       vector332,  vector333,  vector334,  vector335
#endif
#if PPC_VECTORS_NUMBER > 336
        .long       vector336,  vector337,  vector338,  vector339
#endif
#if PPC_VECTORS_NUMBER > 340
        .long       vector340,  vector341,  vector342,  vector343
#endif
#if PPC_VECTORS_NUMBER > 344
        .long       vector344,  vector345,  vector346,  vector347
#endif
#if PPC_VECTORS_NUMBER > 348
        .long       vector348,  vector349,  vector350,  vector351
#endif
#if PPC_VECTORS_NUMBER > 352
        .long       vector352,  vector353,  vector354,  vector355
#endif
#if PPC_VECTORS_NUMBER > 356
        .long       vector356,  vector357,  vector358,  vector359
#endif
#if PPC_VECTORS_NUMBER > 360
        .long       vector360,  vector361,  vector362,  vector363
#endif
#if PPC_VECTORS_NUMBER > 364
        .long       vector364,  vector365,  vector366,  vector367
#endif
#if PPC_VECTORS_NUMBER > 368
        .long       vector368,  vector369,  vector370,  vector371
#endif
#if PPC_VECTORS_NUMBER > 372
        .long       vector372,  vector373,  vector374,  vector375
#endif
#if PPC_VECTORS_NUMBER > 376
        .long       vector376,  vector377,  vector378,  vector379
#endif
#if PPC_VECTORS_NUMBER > 380
        .long       vector380,  vector381,  vector382,  vector383
#endif
#if PPC_VECTORS_NUMBER > 384
        .long       vector384,  vector385,  vector386,  vector387
#endif
#if PPC_VECTORS_NUMBER > 388
        .long       vector388,  vector389,  vector390,  vector391
#endif
#if PPC_VECTORS_NUMBER > 392
        .long       vector392,  vector393,  vector394,  vector395
#endif
#if PPC_VECTORS_NUMBER > 396
        .long       vector396,  vector397,  vector398,  vector399
#endif
#if PPC_VECTORS_NUMBER > 400
        .long       vector400,  vector401,  vector402,  vector403
#endif
#if PPC_VECTORS_NUMBER > 404
        .long       vector404,  vector405,  vector406,  vector407
#endif
#if PPC_VECTORS_NUMBER > 408
        .long       vector408,  vector409,  vector410,  vector411
#endif
#if PPC_VECTORS_NUMBER > 412
        .long       vector412,  vector413,  vector414,  vector415
#endif
#if PPC_VECTORS_NUMBER > 416
        .long       vector416,  vector417,  vector418,  vector419
#endif
#if PPC_VECTORS_NUMBER > 420
        .long       vector420,  vector421,  vector422,  vector423
#endif
#if PPC_VECTORS_NUMBER > 424
        .long       vector424,  vector425,  vector426,  vector427
#endif
#if PPC_VECTORS_NUMBER > 428
        .long       vector428,  vector429,  vector430,  vector431
#endif
#if PPC_VECTORS_NUMBER > 432
        .long       vector432,  vector433,  vector434,  vector435
#endif
#if PPC_VECTORS_NUMBER > 436
        .long       vector436,  vector437,  vector438,  vector439
#endif
#if PPC_VECTORS_NUMBER > 440
        .long       vector440,  vector441,  vector442,  vector443
#endif
#if PPC_VECTORS_NUMBER > 444
        .long       vector444,  vector445,  vector446,  vector447
#endif
#if PPC_VECTORS_NUMBER > 448
        .long       vector448,  vector449,  vector450,  vector451
#endif
#if PPC_VECTORS_NUMBER > 452
        .long       vector452,  vector453,  vector454,  vector455
#endif
#if PPC_VECTORS_NUMBER > 456
        .long       vector456,  vector457,  vector458,  vector459
#endif
#if PPC_VECTORS_NUMBER > 460
        .long       vector460,  vector461,  vector462,  vector463
#endif
#if PPC_VECTORS_NUMBER > 464
        .long       vector464,  vector465,  vector466,  vector467
#endif
#if PPC_VECTORS_NUMBER > 468
        .long       vector468,  vector469,  vector470,  vector471
#endif
#if PPC_VECTORS_NUMBER > 472
        .long       vector472,  vector473,  vector474,  vector475
#endif
#if PPC_VECTORS_NUMBER > 476
        .long       vector476,  vector477,  vector478,  vector479
#endif
#if PPC_VECTORS_NUMBER > 480
        .long       vector480,  vector481,  vector482,  vector483
#endif
#if PPC_VECTORS_NUMBER > 484
        .long       vector484,  vector485,  vector486,  vector487
#endif
#if PPC_VECTORS_NUMBER > 488
        .long       vector488,  vector489,  vector490,  vector491
#endif
#if PPC_VECTORS_NUMBER > 492
        .long       vector492,  vector493,  vector494,  vector495
#endif
#if PPC_VECTORS_NUMBER > 496
        .long       vector496,  vector497,  vector498,  vector499
#endif
#if PPC_VECTORS_NUMBER > 500
        .long       vector500,  vector501,  vector502,  vector503
#endif
#if PPC_VECTORS_NUMBER > 504
        .long       vector504,  vector505,  vector506,  vector507
#endif
#if PPC_VECTORS_NUMBER > 508
        .long       vector508,  vector509,  vector510,  vector511
#endif

        .text
        .align      2

        .weak       vector0,    vector1,    vector2,    vector3
        .weak       vector4,    vector5,    vector6,    vector7
        .weak       vector8,    vector9,    vector10,   vector11
        .weak       vector12,   vector13,   vector14,   vector15
        .weak       vector16,   vector17,   vector18,   vector19
        .weak       vector20,   vector21,   vector22,   vector23
        .weak       vector24,   vector25,   vector26,   vector27
        .weak       vector28,   vector29,   vector30,   vector31
        .weak       vector32,   vector33,   vector34,   vector35
        .weak       vector36,   vector37,   vector38,   vector39
        .weak       vector40,   vector41,   vector42,   vector43
        .weak       vector44,   vector45,   vector46,   vector47
        .weak       vector48,   vector49,   vector50,   vector51
        .weak       vector52,   vector53,   vector54,   vector55
        .weak       vector56,   vector57,   vector58,   vector59
        .weak       vector60,   vector61,   vector62,   vector63
        .weak       vector64,   vector65,   vector66,   vector67
        .weak       vector68,   vector69,   vector70,   vector71
        .weak       vector72,   vector73,   vector74,   vector75
        .weak       vector76,   vector77,   vector78,   vector79
        .weak       vector80,   vector81,   vector82,   vector83
        .weak       vector84,   vector85,   vector86,   vector87
        .weak       vector88,   vector89,   vector90,   vector91
        .weak       vector92,   vector93,   vector94,   vector95
        .weak       vector96,   vector97,   vector98,   vector99
        .weak       vector100,  vector101,  vector102,  vector103
        .weak       vector104,  vector105,  vector106,  vector107
        .weak       vector108,  vector109,  vector110,  vector111
        .weak       vector112,  vector113,  vector114,  vector115
        .weak       vector116,  vector117,  vector118,  vector119
        .weak       vector120,  vector121,  vector122,  vector123
        .weak       vector124,  vector125,  vector126,  vector127
        .weak       vector128,  vector129,  vector130,  vector131
        .weak       vector132,  vector133,  vector134,  vector135
        .weak       vector136,  vector137,  vector138,  vector139
        .weak       vector140,  vector141,  vector142,  vector143
        .weak       vector144,  vector145,  vector146,  vector147
        .weak       vector148,  vector149,  vector150,  vector151
        .weak       vector152,  vector153,  vector154,  vector155
        .weak       vector156,  vector157,  vector158,  vector159
        .weak       vector160,  vector161,  vector162,  vector163
        .weak       vector164,  vector165,  vector166,  vector167
        .weak       vector168,  vector169,  vector170,  vector171
        .weak       vector172,  vector173,  vector174,  vector175
        .weak       vector176,  vector177,  vector178,  vector179
        .weak       vector180,  vector181,  vector182,  vector183
        .weak       vector184,  vector185,  vector186,  vector187
        .weak       vector188,  vector189,  vector190,  vector191
        .weak       vector192,  vector193,  vector194,  vector195
        .weak       vector196,  vector197,  vector198,  vector199
        .weak       vector200,  vector201,  vector202,  vector203
        .weak       vector204,  vector205,  vector206,  vector207
        .weak       vector208,  vector209,  vector210,  vector211
        .weak       vector212,  vector213,  vector214,  vector215
        .weak       vector216,  vector217,  vector218,  vector219
        .weak       vector220,  vector221,  vector222,  vector223
        .weak       vector224,  vector225,  vector226,  vector227
        .weak       vector228,  vector229,  vector230,  vector231
        .weak       vector232,  vector233,  vector234,  vector235
        .weak       vector236,  vector237,  vector238,  vector239
        .weak       vector240,  vector241,  vector242,  vector243
        .weak       vector244,  vector245,  vector246,  vector247
        .weak       vector248,  vector249,  vector250,  vector251
        .weak       vector252,  vector253,  vector254,  vector255
        .weak       vector256,  vector257,  vector258,  vector259
        .weak       vector260,  vector261,  vector262,  vector263
        .weak       vector264,  vector265,  vector266,  vector267
        .weak       vector268,  vector269,  vector270,  vector271
        .weak       vector272,  vector273,  vector274,  vector275
        .weak       vector276,  vector277,  vector278,  vector279
        .weak       vector280,  vector281,  vector282,  vector283
        .weak       vector284,  vector285,  vector286,  vector287
        .weak       vector288,  vector289,  vector290,  vector291
        .weak       vector292,  vector293,  vector294,  vector295
        .weak       vector296,  vector297,  vector298,  vector299
        .weak       vector300,  vector301,  vector302,  vector303
        .weak       vector304,  vector305,  vector306,  vector307
        .weak       vector308,  vector309,  vector310,  vector311
        .weak       vector312,  vector313,  vector314,  vector315
        .weak       vector316,  vector317,  vector318,  vector319
        .weak       vector320,  vector321,  vector322,  vector323
        .weak       vector324,  vector325,  vector326,  vector327
        .weak       vector328,  vector329,  vector330,  vector331
        .weak       vector332,  vector333,  vector334,  vector335
        .weak       vector336,  vector337,  vector338,  vector339
        .weak       vector340,  vector341,  vector342,  vector343
        .weak       vector344,  vector345,  vector346,  vector347
        .weak       vector348,  vector349,  vector350,  vector351
        .weak       vector352,  vector353,  vector354,  vector355
        .weak       vector356,  vector357,  vector358,  vector359
        .weak       vector360,  vector361,  vector362,  vector363
        .weak       vector364,  vector365,  vector366,  vector367
        .weak       vector368,  vector369,  vector370,  vector371
        .weak       vector372,  vector373,  vector374,  vector375
        .weak       vector376,  vector377,  vector378,  vector379
        .weak       vector380,  vector381,  vector382,  vector383
        .weak       vector384,  vector385,  vector386,  vector387
        .weak       vector388,  vector389,  vector390,  vector391
        .weak       vector392,  vector393,  vector394,  vector395
        .weak       vector396,  vector397,  vector398,  vector399
        .weak       vector400,  vector401,  vector402,  vector403
        .weak       vector404,  vector405,  vector406,  vector407
        .weak       vector408,  vector409,  vector410,  vector411
        .weak       vector412,  vector413,  vector414,  vector415
        .weak       vector416,  vector417,  vector418,  vector419
        .weak       vector420,  vector421,  vector422,  vector423
        .weak       vector424,  vector425,  vector426,  vector427
        .weak       vector428,  vector429,  vector430,  vector431
        .weak       vector432,  vector433,  vector434,  vector435
        .weak       vector436,  vector437,  vector438,  vector439
        .weak       vector440,  vector441,  vector442,  vector443
        .weak       vector444,  vector445,  vector446,  vector447
        .weak       vector448,  vector449,  vector450,  vector451
        .weak       vector452,  vector453,  vector454,  vector455
        .weak       vector456,  vector457,  vector458,  vector459
        .weak       vector460,  vector461,  vector462,  vector463
        .weak       vector464,  vector465,  vector466,  vector467
        .weak       vector468,  vector469,  vector470,  vector471
        .weak       vector472,  vector473,  vector474,  vector475
        .weak       vector476,  vector477,  vector478,  vector479
        .weak       vector480,  vector481,  vector482,  vector483
        .weak       vector484,  vector485,  vector486,  vector487
        .weak       vector488,  vector489,  vector490,  vector491
        .weak       vector492,  vector493,  vector494,  vector495
        .weak       vector496,  vector497,  vector498,  vector499
        .weak       vector500,  vector501,  vector502,  vector503
        .weak       vector504,  vector505,  vector506,  vector507
        .weak       vector508,  vector509,  vector510,  vector511

vector0:
vector1:
vector2:
vector3:
vector4:
vector5:
vector6:
vector7:
vector8:
vector9:
vector10:
vector11:
vector12:
vector13:
vector14:
vector15:
vector16:
vector17:
vector18:
vector19:
vector20:
vector21:
vector22:
vector23:
vector24:
vector25:
vector26:
vector27:
vector28:
vector29:
vector30:
vector31:
vector32:
vector33:
vector34:
vector35:
vector36:
vector37:
vector38:
vector39:
vector40:
vector41:
vector42:
vector43:
vector44:
vector45:
vector46:
vector47:
vector48:
vector49:
vector50:
vector51:
vector52:
vector53:
vector54:
vector55:
vector56:
vector57:
vector58:
vector59:
vector60:
vector61:
vector62:
vector63:
vector64:
vector65:
vector66:
vector67:
vector68:
vector69:
vector70:
vector71:
vector72:
vector73:
vector74:
vector75:
vector76:
vector77:
vector78:
vector79:
vector80:
vector81:
vector82:
vector83:
vector84:
vector85:
vector86:
vector87:
vector88:
vector89:
vector90:
vector91:
vector92:
vector93:
vector94:
vector95:
vector96:
vector97:
vector98:
vector99:
vector100:
vector101:
vector102:
vector103:
vector104:
vector105:
vector106:
vector107:
vector108:
vector109:
vector110:
vector111:
vector112:
vector113:
vector114:
vector115:
vector116:
vector117:
vector118:
vector119:
vector120:
vector121:
vector122:
vector123:
vector124:
vector125:
vector126:
vector127:
vector128:
vector129:
vector130:
vector131:
vector132:
vector133:
vector134:
vector135:
vector136:
vector137:
vector138:
vector139:
vector140:
vector141:
vector142:
vector143:
vector144:
vector145:
vector146:
vector147:
vector148:
vector149:
vector150:
vector151:
vector152:
vector153:
vector154:
vector155:
vector156:
vector157:
vector158:
vector159:
vector160:
vector161:
vector162:
vector163:
vector164:
vector165:
vector166:
vector167:
vector168:
vector169:
vector170:
vector171:
vector172:
vector173:
vector174:
vector175:
vector176:
vector177:
vector178:
vector179:
vector180:
vector181:
vector182:
vector183:
vector184:
vector185:
vector186:
vector187:
vector188:
vector189:
vector190:
vector191:
vector192:
vector193:
vector194:
vector195:
vector196:
vector197:
vector198:
vector199:
vector200:
vector201:
vector202:
vector203:
vector204:
vector205:
vector206:
vector207:
vector208:
vector209:
vector210:
vector211:
vector212:
vector213:
vector214:
vector215:
vector216:
vector217:
vector218:
vector219:
vector220:
vector221:
vector222:
vector223:
vector224:
vector225:
vector226:
vector227:
vector228:
vector229:
vector230:
vector231:
vector232:
vector233:
vector234:
vector235:
vector236:
vector237:
vector238:
vector239:
vector240:
vector241:
vector242:
vector243:
vector244:
vector245:
vector246:
vector247:
vector248:
vector249:
vector250:
vector251:
vector252:
vector253:
vector254:
vector255:
vector256:
vector257:
vector258:
vector259:
vector260:
vector261:
vector262:
vector263:
vector264:
vector265:
vector266:
vector267:
vector268:
vector269:
vector270:
vector271:
vector272:
vector273:
vector274:
vector275:
vector276:
vector277:
vector278:
vector279:
vector280:
vector281:
vector282:
vector283:
vector284:
vector285:
vector286:
vector287:
vector288:
vector289:
vector290:
vector291:
vector292:
vector293:
vector294:
vector295:
vector296:
vector297:
vector298:
vector299:
vector300:
vector301:
vector302:
vector303:
vector304:
vector305:
vector306:
vector307:
vector308:
vector309:
vector310:
vector311:
vector312:
vector313:
vector314:
vector315:
vector316:
vector317:
vector318:
vector319:
vector320:
vector321:
vector322:
vector323:
vector324:
vector325:
vector326:
vector327:
vector328:
vector329:
vector330:
vector331:
vector332:
vector333:
vector334:
vector335:
vector336:
vector337:
vector338:
vector339:
vector340:
vector341:
vector342:
vector343:
vector344:
vector345:
vector346:
vector347:
vector348:
vector349:
vector350:
vector351:
vector352:
vector353:
vector354:
vector355:
vector356:
vector357:
vector358:
vector359:
vector360:
vector361:
vector362:
vector363:
vector364:
vector365:
vector366:
vector367:
vector368:
vector369:
vector370:
vector371:
vector372:
vector373:
vector374:
vector375:
vector376:
vector377:
vector378:
vector379:
vector380:
vector381:
vector382:
vector383:
vector384:
vector385:
vector386:
vector387:
vector388:
vector389:
vector390:
vector391:
vector392:
vector393:
vector394:
vector395:
vector396:
vector397:
vector398:
vector399:
vector400:
vector401:
vector402:
vector403:
vector404:
vector405:
vector406:
vector407:
vector408:
vector409:
vector410:
vector411:
vector412:
vector413:
vector414:
vector415:
vector416:
vector417:
vector418:
vector419:
vector420:
vector421:
vector422:
vector423:
vector424:
vector425:
vector426:
vector427:
vector428:
vector429:
vector430:
vector431:
vector432:
vector433:
vector434:
vector435:
vector436:
vector437:
vector438:
vector439:
vector440:
vector441:
vector442:
vector443:
vector444:
vector445:
vector446:
vector447:
vector448:
vector449:
vector450:
vector451:
vector452:
vector453:
vector454:
vector455:
vector456:
vector457:
vector458:
vector459:
vector460:
vector461:
vector462:
vector463:
vector464:
vector465:
vector466:
vector467:
vector468:
vector469:
vector470:
vector471:
vector472:
vector473:
vector474:
vector475:
vector476:
vector477:
vector478:
vector479:
vector480:
vector481:
vector482:
vector483:
vector484:
vector485:
vector486:
vector487:
vector488:
vector489:
vector490:
vector491:
vector492:
vector493:
vector494:
vector495:
vector496:
vector497:
vector498:
vector499:
vector500:
vector501:
vector502:
vector503:
vector504:
vector505:
vector506:
vector507:
vector508:
vector509:
vector510:
vector511:

        .weak       _unhandled_irq
        .type       _unhandled_irq, @function
_unhandled_irq:
         b          _unhandled_irq

#endif /* !defined(__DOXYGEN__) */

/** @} */
