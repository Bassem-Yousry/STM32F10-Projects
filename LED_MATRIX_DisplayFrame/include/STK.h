#ifndef stk_H
#define stk_H

#define STK_CTRL 	*((u32*)0xE000E010)
#define STK_LOAD 	*((u32*)0xE000E014)
#define STK_VAL 	*((u32*)0xE000E018)


void STK_MS (u32 t);



#endif
