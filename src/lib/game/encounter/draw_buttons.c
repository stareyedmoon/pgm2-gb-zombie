#include <stdint.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

void encounter_swap_button_color(uint8_t menu_button) {
	uint16_t offset_start = (uint16_t)(0x20 + menu_button * 4) * 16;

	for (uint16_t i = 0; i < 64; i += 2) {
		uint8_t a = get_vram_byte(TILEBLOCK2 + offset_start + i);
		uint8_t b = get_vram_byte(TILEBLOCK2 + offset_start + i + 1);

		set_vram_byte(TILEBLOCK2 + offset_start + i, b);
		set_vram_byte(TILEBLOCK2 + offset_start + i + 1, a);
	}
}
void encounter_swap_button_parity(void) {
    for (uint16_t i = 0x1B0; i < 0x1E0; i += 1) {
        set_vram_byte(TILEBLOCK2 + i, flip_byte(get_vram_byte(TILEBLOCK2 + i)));
    }
}

void encounter_button_move_left(uint8_t menu_button) {
    encounter_swap_button_color(menu_button + 1);
    encounter_swap_button_color(menu_button);
    encounter_swap_button_parity();

    switch (menu_button) {
    case 0:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x18);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x12);
        break;
    case 1:
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x1D);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x1C);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x1B);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x1A);

        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13); // set_vram_byte seems to have a bug where, with the right timing, it still writes during Mode 3.
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x15);
        break;
    case 2:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x18);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 18, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 19, 0x12);
        break;
    }
}
void encounter_button_move_right(uint8_t menu_button) {
    encounter_swap_button_color(menu_button - 1);
    encounter_swap_button_color(menu_button);
    encounter_swap_button_parity();

    switch (menu_button) {
    case 1:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x1A);
        break;
    case 2:
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x15);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x14);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x13);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x12);

        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x18);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x19);// set_vram_byte seems to have a bug where, with the right timing, it still writes during Mode 3.
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x1B);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x1C);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x1D);
        break;
    case 3:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 18, 0x19);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 19, 0x1A);
        break;
    }
}
