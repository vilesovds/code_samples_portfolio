#define LEDTXT_COLS 5

static char letters[256][5] = {
   0x00, 0x00, 0x00, 0x00, 0x00,    // 0 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 1 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 2 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 3 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 4 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 5 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 6 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 7 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 8 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 9 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 10 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 11 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 12 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 13 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 14 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 15 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 16 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 17 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 18 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 19 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 20 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 21 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 22 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 23 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 24 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 25 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 26 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 27 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 28 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 29 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 30 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 31 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 32 ( )
   0x00, 0x00, 0x5F, 0x00, 0x00,    // 33 (!)
   0x00, 0x03, 0x00, 0x03, 0x00,    // 34 (")
   0x14, 0x3E, 0x14, 0x3E, 0x14,    // 35 (#)
   0x26, 0x49, 0x7F, 0x49, 0x32,    // 36 ($)
   0x4C, 0x2C, 0x10, 0x68, 0x64,    // 37 (%)
   0x40, 0x40, 0x40, 0x40, 0x40,    // 38 (&)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 39 (')
   0x00, 0x00, 0x3E, 0x41, 0x00,    // 40 (()
   0x00, 0x41, 0x3E, 0x00, 0x00,    // 41 ())
   0x2A, 0x1C, 0x3E, 0x1C, 0x2A,    // 42 (*)
   0x08, 0x08, 0x3E, 0x08, 0x08,    // 43 (+)
   0x00, 0x40, 0x30, 0x00, 0x00,    // 44 (,)
   0x00, 0x10, 0x10, 0x10, 0x00,    // 45 (-)
   0x00, 0x00, 0x40, 0x00, 0x00,    // 46 (.)
   0x40, 0x30, 0x08, 0x06, 0x01,    // 47 (/)
   0x3C, 0x52, 0x4A, 0x3C, 0x00,    // 48 (0)
   0x08, 0x04, 0x7E, 0x00, 0x00,    // 49 (1)
   0x44, 0x62, 0x52, 0x4C, 0x00,    // 50 (2)
   0x22, 0x4A, 0x4A, 0x36, 0x00,    // 51 (3)
   0x0E, 0x08, 0x08, 0x7E, 0x00,    // 52 (4)
   0x4E, 0x4A, 0x4A, 0x32, 0x00,    // 53 (5)
   0x3C, 0x4A, 0x4A, 0x32, 0x00,    // 54 (6)
   0x02, 0x72, 0x0A, 0x06, 0x00,    // 55 (7)
   0x34, 0x4A, 0x4A, 0x34, 0x00,    // 56 (8)
   0x4C, 0x52, 0x52, 0x3C, 0x00,    // 57 (9)
   0x00, 0x00, 0x14, 0x00, 0x00,    // 58 (:)
   0x00, 0x40, 0x32, 0x00, 0x00,    // 59 (;)
   0x00, 0x08, 0x14, 0x22, 0x00,    // 60 (<)
   0x00, 0x14, 0x14, 0x14, 0x00,    // 61 (=)
   0x00, 0x22, 0x14, 0x08, 0x00,    // 62 (>)
   0x02, 0x01, 0x51, 0x09, 0x06,    // 63 (?)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 64 (@)
   0x7C, 0x12, 0x11, 0x12, 0x7C,    // 65 (A)
   0x7F, 0x49, 0x49, 0x49, 0x36,    // 66 (B)
   0x3E, 0x41, 0x41, 0x41, 0x22,    // 67 (C)
   0x7F, 0x41, 0x41, 0x41, 0x3E,    // 68 (D)
   0x7F, 0x49, 0x49, 0x49, 0x41,    // 69 (E)
   0x7F, 0x09, 0x09, 0x09, 0x01,    // 70 (F)
   0x3E, 0x41, 0x49, 0x49, 0x7A,    // 71 (G)
   0x7F, 0x08, 0x08, 0x08, 0x7F,    // 72 (H)
   0x00, 0x00, 0x7F, 0x00, 0x00,    // 73 (I)
   0x20, 0x40, 0x40, 0x40, 0x3F,    // 74 (J)
   0x7F, 0x08, 0x14, 0x22, 0x41,    // 75 (K)
   0x7F, 0x40, 0x40, 0x40, 0x40,    // 76 (L)
   0x7F, 0x04, 0x08, 0x04, 0x7F,    // 77 (M)
   0x7F, 0x04, 0x08, 0x10, 0x7F,    // 78 (N)
   0x3E, 0x41, 0x41, 0x41, 0x3E,    // 79 (O)
   0x7F, 0x09, 0x09, 0x09, 0x06,    // 80 (P)
   0x3E, 0x41, 0x41, 0x21, 0x5E,    // 81 (Q)
   0x7F, 0x09, 0x19, 0x29, 0x46,    // 82 (R)
   0x26, 0x49, 0x49, 0x49, 0x32,    // 83 (S)
   0x02, 0x02, 0x7E, 0x02, 0x02,    // 84 (T)
   0x3F, 0x40, 0x40, 0x40, 0x3F,    // 85 (U)
   0x07, 0x38, 0x40, 0x38, 0x07,    // 86 (V)
   0x3F, 0x40, 0x20, 0x40, 0x3F,    // 87 (W)
   0x63, 0x14, 0x08, 0x14, 0x63,    // 88 (X)
   0x07, 0x08, 0x70, 0x08, 0x07,    // 89 (Y)
   0x61, 0x51, 0x49, 0x45, 0x43,    // 90 (Z)
   0x00, 0x7F, 0x41, 0x41, 0x00,    // 91 ([)
   0x03, 0x04, 0x08, 0x10, 0x60,    // 92 (\)
   0x00, 0x41, 0x41, 0x7F, 0x00,    // 93 (])
   0x00, 0x02, 0x01, 0x02, 0x00,    // 94 (^)
   0x00, 0x40, 0x40, 0x40, 0x00,    // 95 (_)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 96 (`)
   0x20, 0x54, 0x54, 0x54, 0x78,    // 97 (a)
   0x00, 0x7F, 0x48, 0x48, 0x30,    // 98 (b)
   0x3C, 0x42, 0x42, 0x42, 0x00,    // 99 (c)
   0x30, 0x48, 0x48, 0x7F, 0x00,    // 100 (d)
   0x38, 0x54, 0x54, 0x54, 0x08,    // 101 (e)
   0x00, 0x7C, 0x12, 0x02, 0x00,    // 102 (f)
   0x00, 0x5C, 0x54, 0x54, 0x38,    // 103 (g)
   0x00, 0x7F, 0x08, 0x08, 0x70,    // 104 (h)
   0x00, 0x00, 0x7A, 0x00, 0x00,    // 105 (i)
   0x00, 0x40, 0x40, 0x3D, 0x00,    // 106 (j)
   0x00, 0x7F, 0x10, 0x6C, 0x00,    // 107 (k)
   0x00, 0x00, 0x7F, 0x00, 0x00,    // 108 (l)
   0x78, 0x08, 0x78, 0x08, 0x70,    // 109 (m)
   0x00, 0x78, 0x08, 0x08, 0x70,    // 110 (n)
   0x38, 0x44, 0x44, 0x44, 0x38,    // 111 (o)
   0x00, 0x7C, 0x14, 0x08, 0x00,    // 112 (p)
   0x00, 0x08, 0x14, 0x78, 0x00,    // 113 (q)
   0x00, 0x78, 0x10, 0x08, 0x00,    // 114 (r)
   0x00, 0x58, 0x54, 0x24, 0x00,    // 115 (s)
   0x08, 0x3E, 0x48, 0x20, 0x00,    // 116 (t)
   0x00, 0x38, 0x40, 0x40, 0x78,    // 117 (u)
   0x00, 0x38, 0x40, 0x38, 0x00,    // 118 (v)
   0x38, 0x40, 0x20, 0x40, 0x38,    // 119 (w)
   0x44, 0x28, 0x10, 0x28, 0x44,    // 120 (x)
   0x00, 0x4C, 0x50, 0x50, 0x3C,    // 121 (y)
   0x44, 0x64, 0x54, 0x54, 0x4C,    // 122 (z)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 123 ({)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 124 (|)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 125 (})
   0x00, 0x00, 0x00, 0x00, 0x00,    // 126 (~)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 127 ()
   0x00, 0x00, 0x00, 0x00, 0x00,    // 128 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 129 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 130 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 131 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 132 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 133 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 134 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 135 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 136 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 137 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 138 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 139 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 140 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 141 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 142 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 143 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 144 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 145 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 146 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 147 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 148 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 149 (�)
   0x00, 0x08, 0x08, 0x08, 0x00,    // 150 (�)
   0x08, 0x08, 0x08, 0x08, 0x08,    // 151 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 152 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 153 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 154 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 155 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 156 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 157 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 158 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 159 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 160 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 161 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 162 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 163 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 164 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 165 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 166 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 167 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 168 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 169 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 170 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 171 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 172 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 173 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 174 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 175 (�)
   0x00, 0x04, 0x0A, 0x04, 0x00,    // 176 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 177 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 178 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 179 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 180 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 181 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 182 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 183 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 184 (�)
   0x14, 0x3E, 0x14, 0x3E, 0x14,    // 185 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 186 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 187 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 188 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 189 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 190 (�)
   0x00, 0x00, 0x00, 0x00, 0x00,    // 191 (�)
   0x7C, 0x12, 0x11, 0x12, 0x7C,    // 192 (�)
   0x7F, 0x49, 0x49, 0x49, 0x31,    // 193 (�)
   0x7F, 0x49, 0x49, 0x49, 0x36,    // 194 (�)
   0x7F, 0x01, 0x01, 0x01, 0x01,    // 195 (�)
   0x60, 0x3E, 0x21, 0x3E, 0x60,    // 196 (�)
   0x7F, 0x49, 0x49, 0x49, 0x41,    // 197 (�)
   0x77, 0x08, 0x3E, 0x08, 0x77,    // 198 (�)
   0x22, 0x41, 0x49, 0x49, 0x36,    // 199 (�)
   0x7F, 0x10, 0x08, 0x04, 0x7F,    // 200 (�)
   0x7F, 0x10, 0x09, 0x04, 0x7F,    // 201 (�)
   0x7F, 0x08, 0x14, 0x22, 0x41,    // 202 (�)
   0x40, 0x3E, 0x01, 0x01, 0x7F,    // 203 (�)
   0x7F, 0x04, 0x08, 0x04, 0x7F,    // 204 (�)
   0x7F, 0x08, 0x08, 0x08, 0x7F,    // 205 (�)
   0x3E, 0x41, 0x41, 0x41, 0x3E,    // 206 (�)
   0x7F, 0x01, 0x01, 0x01, 0x7F,    // 207 (�)
   0x7F, 0x09, 0x09, 0x09, 0x06,    // 208 (�)
   0x3E, 0x41, 0x41, 0x41, 0x22,    // 209 (�)
   0x01, 0x01, 0x7F, 0x01, 0x01,    // 210 (�)
   0x27, 0x48, 0x48, 0x48, 0x3F,    // 211 (�)
   0x0E, 0x11, 0x7F, 0x11, 0x0E,    // 212 (�)
   0x77, 0x08, 0x08, 0x08, 0x77,    // 213 (�)
   0x3F, 0x20, 0x20, 0x20, 0x7F,    // 214 (�)
   0x0F, 0x08, 0x08, 0x08, 0x7F,    // 215 (�)
   0x7F, 0x40, 0x7F, 0x40, 0x7F,    // 216 (�)
   0x3F, 0x20, 0x3F, 0x20, 0x5F,    // 217 (�)
   0x01, 0x7F, 0x48, 0x48, 0x30,    // 218 (�)
   0x7F, 0x50, 0x70, 0x00, 0x7F,    // 219 (�)
   0x00, 0x7F, 0x48, 0x48, 0x30,    // 220 (�)
   0x22, 0x41, 0x49, 0x49, 0x3E,    // 221 (�)
   0x7F, 0x08, 0x3E, 0x41, 0x3E,    // 222 (�)
   0x46, 0x29, 0x19, 0x09, 0x7F,    // 223 (�)
   0x20, 0x54, 0x54, 0x78, 0x00,    // 224 (�)
   0x38, 0x54, 0x52, 0x22, 0x00,    // 225 (�)
   0x7C, 0x54, 0x54, 0x28, 0x00,    // 226 (�)
   0x7C, 0x04, 0x04, 0x04, 0x00,    // 227 (�)
   0x20, 0x52, 0x52, 0x3C, 0x00,    // 228 (�)
   0x38, 0x54, 0x54, 0x08, 0x00,    // 229 (�)
   0x44, 0x28, 0x7C, 0x28, 0x44,    // 230 (�)
   0x28, 0x44, 0x54, 0x28, 0x00,    // 231 (�)
   0x7C, 0x20, 0x10, 0x7C, 0x00,    // 232 (�)
   0x7C, 0x22, 0x12, 0x7C, 0x00,    // 233 (�)
   0x7C, 0x10, 0x18, 0x64, 0x00,    // 234 (�)
   0x40, 0x38, 0x04, 0x7C, 0x00,    // 235 (�)
   0x7C, 0x18, 0x18, 0x7C, 0x00,    // 236 (�)
   0x7C, 0x10, 0x10, 0x7C, 0x00,    // 237 (�)
   0x38, 0x44, 0x44, 0x38, 0x00,    // 238 (�)
   0x7C, 0x04, 0x04, 0x7C, 0x00,    // 239 (�)
   0x7C, 0x14, 0x14, 0x08, 0x00,    // 240 (�)
   0x38, 0x44, 0x44, 0x44, 0x00,    // 241 (�)
   0x00, 0x04, 0x7C, 0x04, 0x00,    // 242 (�)
   0x4C, 0x50, 0x50, 0x3C, 0x00,    // 243 (�)
   0x08, 0x14, 0x7C, 0x14, 0x08,    // 244 (�)
   0x44, 0x38, 0x38, 0x44, 0x00,    // 245 (�)
   0x3C, 0x20, 0x20, 0x3C, 0x60,    // 246 (�)
   0x1C, 0x10, 0x10, 0x7C, 0x00,    // 247 (�)
   0x7C, 0x40, 0x78, 0x7C, 0x00,    // 248 (�)
   0x3C, 0x20, 0x38, 0x3C, 0x60,    // 249 (�)
   0x04, 0x7C, 0x50, 0x20, 0x00,    // 250 (�)
   0x7C, 0x50, 0x70, 0x7C, 0x00,    // 251 (�)
   0x7C, 0x50, 0x50, 0x20, 0x00,    // 252 (�)
   0x28, 0x44, 0x54, 0x38, 0x00,    // 253 (�)
   0x7C, 0x10, 0x7C, 0x7C, 0x00,    // 254 (�)
   0x48, 0x34, 0x14, 0x7C, 0x00};   // 255 (�)
