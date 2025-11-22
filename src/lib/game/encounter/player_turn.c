#include <stdint.h>
#include <stdbool.h>

#include <gb/gb.h>

#include <game/encounter.h>

#include <defines.h>


static void draw_player_attack_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_item_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_info_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_run_menu(EncounterEntity* player, EncounterEntity* enemy) {}


static void player_turn_attack(EncounterEntity* player, EncounterEntity* enemy) {

}
static void player_turn_items(EncounterEntity* player, EncounterEntity* enemy) {

}

void encounter_player_turn(EncounterEntity* player, EncounterEntity* enemy) {
    encounter_swap_button_color(0);
    // TODO - Find a neater way to do this.
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x18);
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x19);
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x1B);
    set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x1C);
    set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x1D);
    
    uint8_t menu_button = 0;
    bool menu_changed = true;

    uint8_t prev_joy = joypad();

    while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy; 
		uint8_t just_released = prev_joy & ~cur_joy;


        if (just_pressed & J_LEFT && menu_button > 0) {
            menu_button -= 1;
            menu_changed = true;
            encounter_button_move_left(menu_button);
        }
        if (just_pressed & J_RIGHT && menu_button < 3) {
            menu_button += 1;
            menu_changed = true;
            encounter_button_move_right(menu_button);
        }
        if (menu_changed) {
            encounter_text_scroll = 0;

            switch (menu_button) {
            case 0: draw_player_attack_menu(player, enemy); break;
            case 1: draw_player_item_menu(player, enemy); break;
            case 2: draw_player_info_menu(player, enemy); break;
            case 3: draw_player_run_menu(player, enemy); break;
            }

            menu_changed = false;
        }

        if (just_pressed & J_A) {
            // Attack
            if (menu_button == 0) {
                player_turn_attack(player, enemy);
            }
            // Item
            else if (menu_button == 1) {
                player_turn_items(player, enemy);
            }
            // TODO - Implement running away

            break;
        }


        prev_joy = cur_joy;

        vsync();
    }

    encounter_swap_button_color(menu_button);
    // TODO - Put this code somewhere else or figure out a way to remove it entirely.
    switch (menu_button) {
    case 0:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x15);
        break;
    case 1:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x15);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x12);
        encounter_swap_button_parity();
        break;
    case 2:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x15);
        break;
    case 3:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x15);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 18, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 19, 0x12);
        encounter_swap_button_parity();
        break;
    }
}
