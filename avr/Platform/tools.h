/*
 * tools.h
 * Полезняшки. 
 *
 * Author: Погребняк Дмитрий
 * 
 * Помещённый здесь код является свободным. Т.е. допускается его свободное использование для любых целей, включая коммерческие, при условии указания ссылки на автора (Погребняк Дмитрий, http://aterlux.ru/).
 * Code included here are free to use. It can be used for free for any purpose, including commercial use, as long as link to author (Pogrebnyak Dmitry, http://aterlux.ru/) is included.
 */ 


#ifndef TOOLS_H_
#define TOOLS_H_


#define shr(a,b)          \
({                            \
  uint8_t t = a; \
  __asm__ __volatile__ (       \
  "sbrs %1, 2 \n\t"         \
  "rjmp b2c_%=  \n\t"	         \
  "swap %0 \n\t"  \
  "andi %0, 0x0F \n\t" \
  "b2c_%=:" "sbrs %1, 1 \n\t" \
  "rjmp b1c_%= \n\t" \
  "lsr %0 \n\t" \
  "lsr %0 \n\t" \
  "b1c_%=:" "sbrc %1, 0\n\t" \
  "lsr %0 \n\t" \
  : "+d" (t)               \
  : "r" (b)     \
  );                          \
  t;                       \
})


#define shl(a,b)          \
({                            \
  uint8_t t = a; \
  __asm__ __volatile__ (       \
  "sbrs %1, 2 \n\t"         \
  "rjmp b2c_%=  \n\t"	         \
  "swap %0 \n\t"  \
  "andi %0, 0xF0 \n\t" \
  "b2c_%=:" "sbrs %1, 1 \n\t" \
  "rjmp b1c_%= \n\t" \
  "lsl %0 \n\t" \
  "lsl %0 \n\t" \
  "b1c_%=:" "sbrc %1, 0\n\t" \
  "lsl %0 \n\t" \
  : "+d" (t)               \
  : "r" (b)     \
  );                          \
  t;                       \
})

#define reversebits(a) \
({ \
  uint8_t b = a; \
  uint8_t t = a; \
  __asm__ __volatile__ ( \
    "ror %0 \n\t" \
    "rol %1 \n\t" \
    "ror %0 \n\t" \
    "rol %1 \n\t" \
    "ror %0 \n\t" \
    "rol %1 \n\t" \
    "ror %0 \n\t" \
    "rol %1 \n\t" \
    "ror %0 \n\t" \
    "andi %0, 0xF0 \n\t" \
    "andi %1, 0x0F \n\t" \
    "or %0, %1 \n\t" \
    "swap %0 \n\t" \
    : "+d" (b), "+d" (t) \
  ); \
  b; \
})

#define arraysize(array) (sizeof(array) / sizeof(array[0]))

#endif /* TOOLS_H_ */
