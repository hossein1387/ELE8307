/* i      r8    */
/* x      r9    */
/* y      r10   */
/* m      r11   */
/* status r12   */
/* j      r13   */
/* x0     r14   */
/* y0     r15   */
/* radius r16   */
/* color  r17   */


.section .text
.global _planete_drawball_opt
_planete_drawball_opt :
	add  r18,  r0,  ra
/* store passed by value variables to general registers*/
	add  r14,  r0,  r4
	add  r15,  r0,  r5
	add  r16,  r0,  r6
	add  r17,  r0,  r7

	sub  r9 ,  r9,  r9    /* x = 0*/
	add  r10,  r0,  r16   /* y = radius*/
	sub  r11,  r11, r11   /* m = 0*/
	slli r11,  r16, 2     /* m = radius*4*/
	sub  r11,  r0,  r11   /* m = -m*/
	addi r11,  r11, 5     /* m = m + 5 */
	WHILE_BEGIN:
			ble r9,  r10,  NEW_LOOP  /* x<=y*/
			br  END_OF_CALL
		NEW_LOOP:
			sub r8,  r14,  r9   /* i = x0 - x*/
		LOOP_1:
			add r12,  r14,  r9
			blt r8,   r12, BEGIN_LOOP_1 /* i < x + x0 */
			br  END_OF_LOOP_1
			BEGIN_LOOP_1:
				addi   r8,  r8,  1               /* i++   */
				blt    r8,  r0,  LOOP_1          /* i<0   */
				cmpgei r12, r8,  641             /* i>641 */
				bne    r12, r0,  LOOP_1          /**/
				add    r4,  r0,  r8              /* pass i        */
				add    r5,  r10, r15             /* pass y+y0     */
				add    r6,  r0,  r17             /* pass color    */
				call   ecran2d_setPixel          /* call function */
				add    r4,  r0,  r8              /* pass i        */
				sub    r5,  r15, r10             /* pass y-y0     */
				add    r6,  r0,  r17             /* pass color    */
				call   ecran2d_setPixel          /* call function */
				br     LOOP_1
		END_OF_LOOP_1:

			sub r13,  r14,  r10                  /* j = x0 - y    */
		LOOP_2:                                  /*               */
			add r12,  r14,  r10                  /*               */
			blt r13,  r12, BEGIN_LOOP_2          /* j < x0 + y    */
			br  END_OF_LOOP_2                    /*               */
			BEGIN_LOOP_2:                        /*               */
				addi   r13,  r13,  1             /* j++           */
				blt    r13,  r0,   LOOP_2        /* j<0           */
				cmpgei r12,  r13,  641           /* j>641         */
				bne    r12,  r0,   LOOP_2        /*               */
				add    r4,   r0,   r13           /* pass j        */
				add    r5,   r9,   r15           /* pass x+y0     */
				add    r6,   r0,   r17           /* pass color    */
				call   ecran2d_setPixel          /* call function */
				add    r4,   r0,   r13           /* pass j        */
				sub    r5,   r15,  r9            /* pass y0-x     */
				add    r6,   r0,   r17           /* pass color    */
				call   ecran2d_setPixel          /* call function */
				br     LOOP_2                    /*               */
			END_OF_LOOP_2:
			blt r0,  r11,  CALC_Y_M              /* 0<m           */
			br END_OF_CAL_Y_M
		CALC_Y_M:
			subi  r10,  r10,  1                 /* y=y-1          */
			slli  r12,  r10,  3                 /*                */
			sub   r11,  r11,  r12               /* m=m-8*y        */
			br    END_OF_CAL_Y_M
		END_OF_CAL_Y_M:
			addi  r9,  r9,  1                   /* x++            */
			slli  r12, r9,  3                   /*                */
			addi  r12, r12, 4                   /*                */
			add   r11, r11, r12                 /* m=m+x*8+4      */
			br    WHILE_BEGIN
	END_OF_CALL:
		sub  ra,   ra,  ra
		add  ra,   r18, r0
		ret
.end
