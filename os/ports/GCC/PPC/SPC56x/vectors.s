/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file    PPC/SPC56x/vectors.s
 * @brief   SPC56x vectors table.
 *
 * @addtogroup PPC_CORE
 * @{
 */
/** @cond never */

        /*
         * BAM info, SWT off, WTE off, VLE off.
         */
        .section    .bam
        .long       0x005A0000
        .long       _boot_address

        /*
         * Software vectors table. The vectors are accessed from the IVOR4
         * handler only. In order to declare an interrupt handler just create
         * a function withe the same name of a vector, the symbol will
         * override the weak symbol declared here.
         */
        .section    .vectors
        .align		2
        .globl      _vectors
_vectors:
        .long       vector0
        .long       vector1
        .long       vector2
        .long       vector3
        .long       vector4
        .long       vector5
        .long       vector6
        .long       vector7
        .long       vector8
        .long       vector9
        .long       vector10
        .long       vector11
        .long       vector12
        .long       vector13
        .long       vector14
        .long       vector15
        .long       vector16
        .long       vector17
        .long       vector18
        .long       vector19
        .long       vector20
        .long       vector21
        .long       vector22
        .long       vector23
        .long       vector24
        .long       vector25
        .long       vector26
        .long       vector27
        .long       vector28
        .long       vector29
        .long       vector30
        .long       vector31
        .long       vector32
        .long       vector33
        .long       vector34
        .long       vector35
        .long       vector36
        .long       vector37
        .long       vector38
        .long       vector39
        .long       vector40
        .long       vector41
        .long       vector42
        .long       vector43
        .long       vector44
        .long       vector45
        .long       vector46
        .long       vector47
        .long       vector48
        .long       vector49
        .long       vector50
        .long       vector51
        .long       vector52
        .long       vector53
        .long       vector54
        .long       vector55
        .long       vector56
        .long       vector57
        .long       vector58
        .long       vector59
        .long       vector60
        .long       vector61
        .long       vector62
        .long       vector63
        .long       vector64
        .long       vector65
        .long       vector66
        .long       vector67
        .long       vector68
        .long       vector69
        .long       vector70
        .long       vector71
        .long       vector72
        .long       vector73
        .long       vector74
        .long       vector75
        .long       vector76
        .long       vector77
        .long       vector78
        .long       vector79
        .long       vector80
        .long       vector81
        .long       vector82
        .long       vector83
        .long       vector84
        .long       vector85
        .long       vector86
        .long       vector87
        .long       vector88
        .long       vector89
        .long       vector90
        .long       vector91
        .long       vector92
        .long       vector93
        .long       vector94
        .long       vector95
        .long       vector96
        .long       vector97
        .long       vector98
        .long       vector99
        .long       vector100
        .long       vector101
        .long       vector102
        .long       vector103
        .long       vector104
        .long       vector105
        .long       vector106
        .long       vector107
        .long       vector108
        .long       vector109
        .long       vector110
        .long       vector111
        .long       vector112
        .long       vector113
        .long       vector114
        .long       vector115
        .long       vector116
        .long       vector117
        .long       vector118
        .long       vector119
        .long       vector120
        .long       vector121
        .long       vector122
        .long       vector123
        .long       vector124
        .long       vector125
        .long       vector126
        .long       vector127
        .long       vector128
        .long       vector129
        .long       vector130
        .long       vector131
        .long       vector132
        .long       vector133
        .long       vector134
        .long       vector135
        .long       vector136
        .long       vector137
        .long       vector138
        .long       vector139
        .long       vector140
        .long       vector141
        .long       vector142
        .long       vector143
        .long       vector144
        .long       vector145
        .long       vector146
        .long       vector147
        .long       vector148
        .long       vector149
        .long       vector150
        .long       vector151
        .long       vector152
        .long       vector153
        .long       vector154
        .long       vector155
        .long       vector156
        .long       vector157
        .long       vector158
        .long       vector159
        .long       vector160
        .long       vector161
        .long       vector162
        .long       vector163
        .long       vector164
        .long       vector165
        .long       vector166
        .long       vector167
        .long       vector168
        .long       vector169
        .long       vector170
        .long       vector171
        .long       vector172
        .long       vector173
        .long       vector174
        .long       vector175
        .long       vector176
        .long       vector177
        .long       vector178
        .long       vector179
        .long       vector180
        .long       vector181
        .long       vector182
        .long       vector183
        .long       vector184
        .long       vector185
        .long       vector186
        .long       vector187
        .long       vector188
        .long       vector189
        .long       vector190
        .long       vector191
        .long       vector192
        .long       vector193
        .long       vector194
        .long       vector195
        .long       vector196
        .long       vector197
        .long       vector198
        .long       vector199
        .long       vector200
        .long       vector201
        .long       vector202
        .long       vector203
        .long       vector204
        .long       vector205
        .long       vector206
        .long       vector207
        .long       vector208
        .long       vector209
        .long       vector210
        .long       vector211
        .long       vector212
        .long       vector213
        .long       vector214
        .long       vector215
        .long       vector216
        .long       vector217
        .long       vector218
        .long       vector219
        .long       vector220
        .long       vector221
        .long       vector222
        .long       vector223
        .long       vector224
        .long       vector225
        .long       vector226
        .long       vector227
        .long       vector228
        .long       vector229
        .long       vector230
        .long       vector231
        .long       vector232
        .long       vector233
        .long       vector234
        .long       vector235
        .long       vector236
        .long       vector237
        .long       vector238
        .long       vector239
        .long       vector240
        .long       vector241
        .long       vector242
        .long       vector243
        .long       vector244
        .long       vector245
        .long       vector246
        .long       vector247
        .long       vector248
        .long       vector249
        .long       vector250
        .long       vector251
        .long       vector252
        .long       vector253
        .long       vector254
        .long       vector255
        .long       vector256
        .long       vector257
        .long       vector258
        .long       vector259
        .long       vector260
        .long       vector261
        .long       vector262
        .long       vector263
        .long       vector264
        .long       vector265
        .long       vector266
        .long       vector267
        .long       vector268
        .long       vector269
        .long       vector270
        .long       vector271
        .long       vector272
        .long       vector273
        .long       vector274
        .long       vector275
        .long       vector276
        .long       vector277
        .long       vector278
        .long       vector279
        .long       vector280
        .long       vector281
        .long       vector282
        .long       vector283
        .long       vector284
        .long       vector285
        .long       vector286
        .long       vector287
        .long       vector288
        .long       vector289
        .long       vector290
        .long       vector291
        .long       vector292
        .long       vector293
        .long       vector294
        .long       vector295
        .long       vector296
        .long       vector297
        .long       vector298
        .long       vector299
        .long       vector300
        .long       vector301
        .long       vector302
        .long       vector303
        .long       vector304
        .long       vector305
        .long       vector306
        .long       vector307
        .long       vector308
        .long       vector309
        .long       vector310
        .long       vector311
        .long       vector312
        .long       vector313
        .long       vector314
        .long       vector315
        .long       vector316
        .long       vector317
        .long       vector318
        .long       vector319
        .long       vector320
        .long       vector321
        .long       vector322
        .long       vector323
        .long       vector324
        .long       vector325
        .long       vector326
        .long       vector327
        .long       vector328
        .long       vector329
        .long       vector330
        .long       vector331
        .long       vector332
        .long       vector333
        .long       vector334
        .long       vector335
        .long       vector336
        .long       vector337
        .long       vector338
        .long       vector339
        .long       vector340
        .long       vector341
        .long       vector342
        .long       vector343
        .long       vector344
        .long       vector345
        .long       vector346
        .long       vector347
        .long       vector348
        .long       vector349
        .long       vector350
        .long       vector351
        .long       vector352
        .long       vector353
        .long       vector354
        .long       vector355
        .long       vector356
        .long       vector357
        .long       vector358
        .long       vector359

        .text
        .align      2
        .weak       vector0
vector0:

        .weak       vector1
vector1:

        .weak       vector2
vector2:

        .weak       vector3
vector3:

        .weak       vector4
vector4:

        .weak       vector5
vector5:

        .weak       vector6
vector6:

        .weak       vector7
vector7:

        .weak       vector8
vector8:

        .weak       vector9
vector9:

        .weak       vector10
vector10:

        .weak       vector11
vector11:

        .weak       vector12
vector12:

        .weak       vector13
vector13:

        .weak       vector14
vector14:

        .weak       vector15
vector15:

        .weak       vector16
vector16:

        .weak       vector17
vector17:

        .weak       vector18
vector18:

        .weak       vector19
vector19:

        .weak       vector20
vector20:

        .weak       vector21
vector21:

        .weak       vector22
vector22:

        .weak       vector23
vector23:

        .weak       vector24
vector24:

        .weak       vector25
vector25:

        .weak       vector26
vector26:

        .weak       vector27
vector27:

        .weak       vector28
vector28:

        .weak       vector29
vector29:

        .weak       vector30
vector30:

        .weak       vector31
vector31:

        .weak       vector32
vector32:

        .weak       vector33
vector33:

        .weak       vector34
vector34:

        .weak       vector35
vector35:

        .weak       vector36
vector36:

        .weak       vector37
vector37:

        .weak       vector38
vector38:

        .weak       vector39
vector39:

        .weak       vector40
vector40:

        .weak       vector41
vector41:

        .weak       vector42
vector42:

        .weak       vector43
vector43:

        .weak       vector44
vector44:

        .weak       vector45
vector45:

        .weak       vector46
vector46:

        .weak       vector47
vector47:

        .weak       vector48
vector48:

        .weak       vector49
vector49:

        .weak       vector50
vector50:

        .weak       vector51
vector51:

        .weak       vector52
vector52:

        .weak       vector53
vector53:

        .weak       vector54
vector54:

        .weak       vector55
vector55:

        .weak       vector56
vector56:

        .weak       vector57
vector57:

        .weak       vector58
vector58:

        .weak       vector59
vector59:

        .weak       vector60
vector60:

        .weak       vector61
vector61:

        .weak       vector62
vector62:

        .weak       vector63
vector63:

        .weak       vector64
vector64:

        .weak       vector65
vector65:

        .weak       vector66
vector66:

        .weak       vector67
vector67:

        .weak       vector68
vector68:

        .weak       vector69
vector69:

        .weak       vector70
vector70:

        .weak       vector71
vector71:

        .weak       vector72
vector72:

        .weak       vector73
vector73:

        .weak       vector74
vector74:

        .weak       vector75
vector75:

        .weak       vector76
vector76:

        .weak       vector77
vector77:

        .weak       vector78
vector78:

        .weak       vector79
vector79:

        .weak       vector80
vector80:

        .weak       vector81
vector81:

        .weak       vector82
vector82:

        .weak       vector83
vector83:

        .weak       vector84
vector84:

        .weak       vector85
vector85:

        .weak       vector86
vector86:

        .weak       vector87
vector87:

        .weak       vector88
vector88:

        .weak       vector89
vector89:

        .weak       vector90
vector90:

        .weak       vector91
vector91:

        .weak       vector92
vector92:

        .weak       vector93
vector93:

        .weak       vector94
vector94:

        .weak       vector95
vector95:

        .weak       vector96
vector96:

        .weak       vector97
vector97:

        .weak       vector98
vector98:

        .weak       vector99
vector99:

        .weak       vector100
vector100:

        .weak       vector101
vector101:

        .weak       vector102
vector102:

        .weak       vector103
vector103:

        .weak       vector104
vector104:

        .weak       vector105
vector105:

        .weak       vector106
vector106:

        .weak       vector107
vector107:

        .weak       vector108
vector108:

        .weak       vector109
vector109:

        .weak       vector110
vector110:

        .weak       vector111
vector111:

        .weak       vector112
vector112:

        .weak       vector113
vector113:

        .weak       vector114
vector114:

        .weak       vector115
vector115:

        .weak       vector116
vector116:

        .weak       vector117
vector117:

        .weak       vector118
vector118:

        .weak       vector119
vector119:

        .weak       vector120
vector120:

        .weak       vector121
vector121:

        .weak       vector122
vector122:

        .weak       vector123
vector123:

        .weak       vector124
vector124:

        .weak       vector125
vector125:

        .weak       vector126
vector126:

        .weak       vector127
vector127:

        .weak       vector128
vector128:

        .weak       vector129
vector129:

        .weak       vector130
vector130:

        .weak       vector131
vector131:

        .weak       vector132
vector132:

        .weak       vector133
vector133:

        .weak       vector134
vector134:

        .weak       vector135
vector135:

        .weak       vector136
vector136:

        .weak       vector137
vector137:

        .weak       vector138
vector138:

        .weak       vector139
vector139:

        .weak       vector140
vector140:

        .weak       vector141
vector141:

        .weak       vector142
vector142:

        .weak       vector143
vector143:

        .weak       vector144
vector144:

        .weak       vector145
vector145:

        .weak       vector146
vector146:

        .weak       vector147
vector147:

        .weak       vector148
vector148:

        .weak       vector149
vector149:

        .weak       vector150
vector150:

        .weak       vector151
vector151:

        .weak       vector152
vector152:

        .weak       vector153
vector153:

        .weak       vector154
vector154:

        .weak       vector155
vector155:

        .weak       vector156
vector156:

        .weak       vector157
vector157:

        .weak       vector158
vector158:

        .weak       vector159
vector159:

        .weak       vector160
vector160:

        .weak       vector161
vector161:

        .weak       vector162
vector162:

        .weak       vector163
vector163:

        .weak       vector164
vector164:

        .weak       vector165
vector165:

        .weak       vector166
vector166:

        .weak       vector167
vector167:

        .weak       vector168
vector168:

        .weak       vector169
vector169:

        .weak       vector170
vector170:

        .weak       vector171
vector171:

        .weak       vector172
vector172:

        .weak       vector173
vector173:

        .weak       vector174
vector174:

        .weak       vector175
vector175:

        .weak       vector176
vector176:

        .weak       vector177
vector177:

        .weak       vector178
vector178:

        .weak       vector179
vector179:

        .weak       vector180
vector180:

        .weak       vector181
vector181:

        .weak       vector182
vector182:

        .weak       vector183
vector183:

        .weak       vector184
vector184:

        .weak       vector185
vector185:

        .weak       vector186
vector186:

        .weak       vector187
vector187:

        .weak       vector188
vector188:

        .weak       vector189
vector189:

        .weak       vector190
vector190:

        .weak       vector191
vector191:

        .weak       vector192
vector192:

        .weak       vector193
vector193:

        .weak       vector194
vector194:

        .weak       vector195
vector195:

        .weak       vector196
vector196:

        .weak       vector197
vector197:

        .weak       vector198
vector198:

        .weak       vector199
vector199:

        .weak       vector200
vector200:

        .weak       vector201
vector201:

        .weak       vector202
vector202:

        .weak       vector203
vector203:

        .weak       vector204
vector204:

        .weak       vector205
vector205:

        .weak       vector206
vector206:

        .weak       vector207
vector207:

        .weak       vector208
vector208:

        .weak       vector209
vector209:

        .weak       vector210
vector210:

        .weak       vector211
vector211:

        .weak       vector212
vector212:

        .weak       vector213
vector213:

        .weak       vector214
vector214:

        .weak       vector215
vector215:

        .weak       vector216
vector216:

        .weak       vector217
vector217:

        .weak       vector218
vector218:

        .weak       vector219
vector219:

        .weak       vector220
vector220:

        .weak       vector221
vector221:

        .weak       vector222
vector222:

        .weak       vector223
vector223:

        .weak       vector224
vector224:

        .weak       vector225
vector225:

        .weak       vector226
vector226:

        .weak       vector227
vector227:

        .weak       vector228
vector228:

        .weak       vector229
vector229:

        .weak       vector230
vector230:

        .weak       vector231
vector231:

        .weak       vector232
vector232:

        .weak       vector233
vector233:

        .weak       vector234
vector234:

        .weak       vector235
vector235:

        .weak       vector236
vector236:

        .weak       vector237
vector237:

        .weak       vector238
vector238:

        .weak       vector239
vector239:

        .weak       vector240
vector240:

        .weak       vector241
vector241:

        .weak       vector242
vector242:

        .weak       vector243
vector243:

        .weak       vector244
vector244:

        .weak       vector245
vector245:

        .weak       vector246
vector246:

        .weak       vector247
vector247:

        .weak       vector248
vector248:

        .weak       vector249
vector249:

        .weak       vector250
vector250:

        .weak       vector251
vector251:

        .weak       vector252
vector252:

        .weak       vector253
vector253:

        .weak       vector254
vector254:

        .weak       vector255
vector255:

        .weak       vector256
vector256:

        .weak       vector257
vector257:

        .weak       vector258
vector258:

        .weak       vector259
vector259:

        .weak       vector260
vector260:

        .weak       vector261
vector261:

        .weak       vector262
vector262:

        .weak       vector263
vector263:

        .weak       vector264
vector264:

        .weak       vector265
vector265:

        .weak       vector266
vector266:

        .weak       vector267
vector267:

        .weak       vector268
vector268:

        .weak       vector269
vector269:

        .weak       vector270
vector270:

        .weak       vector271
vector271:

        .weak       vector272
vector272:

        .weak       vector273
vector273:

        .weak       vector274
vector274:

        .weak       vector275
vector275:

        .weak       vector276
vector276:

        .weak       vector277
vector277:

        .weak       vector278
vector278:

        .weak       vector279
vector279:

        .weak       vector280
vector280:

        .weak       vector281
vector281:

        .weak       vector282
vector282:

        .weak       vector283
vector283:

        .weak       vector284
vector284:

        .weak       vector285
vector285:

        .weak       vector286
vector286:

        .weak       vector287
vector287:

        .weak       vector288
vector288:

        .weak       vector289
vector289:

        .weak       vector290
vector290:

        .weak       vector291
vector291:

        .weak       vector292
vector292:

        .weak       vector293
vector293:

        .weak       vector294
vector294:

        .weak       vector295
vector295:

        .weak       vector296
vector296:

        .weak       vector297
vector297:

        .weak       vector298
vector298:

        .weak       vector299
vector299:

        .weak       vector300
vector300:

        .weak       vector301
vector301:

        .weak       vector302
vector302:

        .weak       vector303
vector303:

        .weak       vector304
vector304:

        .weak       vector305
vector305:

        .weak       vector306
vector306:

        .weak       vector307
vector307:

        .weak       vector308
vector308:

        .weak       vector309
vector309:

        .weak       vector310
vector310:

        .weak       vector311
vector311:

        .weak       vector312
vector312:

        .weak       vector313
vector313:

        .weak       vector314
vector314:

        .weak       vector315
vector315:

        .weak       vector316
vector316:

        .weak       vector317
vector317:

        .weak       vector318
vector318:

        .weak       vector319
vector319:

        .weak       vector320
vector320:

        .weak       vector321
vector321:

        .weak       vector322
vector322:

        .weak       vector323
vector323:

        .weak       vector324
vector324:

        .weak       vector325
vector325:

        .weak       vector326
vector326:

        .weak       vector327
vector327:

        .weak       vector328
vector328:

        .weak       vector329
vector329:

        .weak       vector330
vector330:

        .weak       vector331
vector331:

        .weak       vector332
vector332:

        .weak       vector333
vector333:

        .weak       vector334
vector334:

        .weak       vector335
vector335:

        .weak       vector336
vector336:

        .weak       vector337
vector337:

        .weak       vector338
vector338:

        .weak       vector339
vector339:

        .weak       vector340
vector340:

        .weak       vector341
vector341:

        .weak       vector342
vector342:

        .weak       vector343
vector343:

        .weak       vector344
vector344:

        .weak       vector345
vector345:

        .weak       vector346
vector346:

        .weak       vector347
vector347:

        .weak       vector348
vector348:

        .weak       vector349
vector349:

        .weak       vector350
vector350:

        .weak       vector351
vector351:

        .weak       vector352
vector352:

        .weak       vector353
vector353:

        .weak       vector354
vector354:

        .weak       vector355
vector355:

        .weak       vector356
vector356:

        .weak       vector357
vector357:

        .weak       vector358
vector358:

        .weak       vector359
vector359:

        .globl      _unhandled_irq
_unhandled_irq:
         b          _unhandled_irq

/** @endcond */
/** @} */
