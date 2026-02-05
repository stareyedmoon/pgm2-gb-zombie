#include <stdint.h>
#include <stdarg.h>

#include <general.h>


// Supported formats
// - %s : string
// - %U : string to upper-case
// - %L : string to lower-case
// - %b : 8-bit unsigned integer
// - %B : 8-bit unsigned integer padded with spaces (right-aligned)
// - %w : 16-bit unsigned integer
// - %W : 16-bit unsigned integer padded with spaces (right-aligned)
// - %p : Pointer as hex
void format(char* buffer, const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	for (uint8_t i = 0; *format; i += 1) {
		if (*format != '%') {
			*(buffer++) = *(format++);
		}
		else {
			format += 1;

			uint8_t val, j;
			uint16_t val16;
			uintptr_t valptr;
			char out[5] = {' ', ' ', ' ', ' ', '0'};

			switch (*(format++)) {
			case 's':
				for (const char* str = va_arg(args, char*); *str;) *(buffer++) = *(str++);
				break;
			case 'U':
				for (const char* str = va_arg(args, char*); *str;) {
					if (*str >= 'a' && *str <= 'z') *(buffer++) = (*str++) & ~0x20;
					else *(buffer++) = *(str++);
				}
				break;
			case 'L':
				for (const char* str = va_arg(args, char*); *str;) {
					if (*str >= 'A' && *str <= 'Z') *(buffer++) = (*str++) | 0x20;
					else *(buffer++) = *(str++);
				}
				break;
			
			case 'b':
				val = va_arg(args, uint8_t);
				j = 5;
				for (; val; val /= 10) out[--j] = '0' + val % 10;
				for (; j < 5; *(buffer++) = out[j++]);
				break;
			
			case 'B':
				val = va_arg(args, uint8_t);
				j = 5;
				for (; val; val /= 10) out[--j] = '0' + val % 10;
				for (j = 2; j < 5; *(buffer++) = out[j++]);
				break;
				
			case 'w':
				val16 = va_arg(args, uint16_t);
				j = 5;
				for (; val16; val16 /= 10) out[--j] = '0' + val16 % 10;
				for (; j < 5; *(buffer++) = out[j++]);
				break;
			
			case 'W':
				val16 = va_arg(args, uint16_t);
				j = 5;
				for (; val16; val16 /= 10) out[--j] = '0' + val16 % 10;
				for (j = 0; j < 5; *(buffer++) = out[j++]);
				break;
				
			case 'p':
				valptr = (uintptr_t)va_arg(args, char*);
				for (uint8_t i = sizeof(uintptr_t) * 2; i > 0; i -= 1) {
					*(buffer++) = "0123456789ABCDEF"[(valptr >> ((i-1) * 4)) & 0xF];
				}
				break;

			default:
				for (uint8_t i = 0; i < 3; i++) *(buffer++) = "ERF"[i];
				break;
			}
		}
	}

	*buffer = 0;

	va_end(args);
}
