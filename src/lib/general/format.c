#include <stdint.h>
#include <stdarg.h>

#include <general.h>


// Supported formats
// - %s : string
// - %b : 8-bit unsigned integer
void format(char* buffer, const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	for (uint8_t i = 0; *format; i += 1) {
		if (*format != '%') {
			*(buffer++) = *(format++);
		}
		else {
			format += 1;

			switch (*(format++)) {
			case 's':
				for (const char* str = va_arg(args, char*); *str;) *(buffer++) = *(str++);
				break;
			
			case 'b':
				uint8_t val = va_arg(args, uint8_t);
				char out[3] = {0, 0, '0'};
				uint8_t j = 3;
				for (; val; out[--j] = '0' + val % 10) val /= 10;
				for (; j < 3; *(buffer++) = out[j]);
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
