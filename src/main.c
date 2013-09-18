#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x5B, 0x63, 0x0C, 0x96, 0x50, 0x3C, 0x41, 0x84, 0xB4, 0x7F, 0x6F, 0xB5, 0xBD, 0xB7, 0xDF, 0x00 }
PBL_APP_INFO(MY_UUID,
             "HestAlHeurt", "Elze Kool",
             1, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer minute_layer;
TextLayer rel_layer;
TextLayer hour_layer;

GFont light_font;
GFont bold_font;

static const char *numbers[] = {
  "iene", "twai", "drai", "veere", "vieve", "zesse", "zeum", "achte", "neeng", "tiene", "elf", "twaalve", "dartien", "virtien", "kwart"
};

static const char *rels[] = {
	"over", "veur half", "half", "over half", "veur"
};

/**
 * Update Time on display
 **/
void update_time(PblTm* t) {

	static char minute_text[] = "               ";
	static char rel_text[]    = "               ";
	static char hour_text[]   = "               ";	

	int min = t->tm_min;	
	int hour = t->tm_hour;

	// Whole hour until kwarter past	
	if ((min > 0) && (min <= 15)) {
		strcpy(minute_text, numbers[min - 1]);
		strcpy(rel_text, rels[0]);
			
	// 14..1 minutes before half
	} else if ((min > 15) && (min < 30)) {
		strcpy(minute_text, numbers[29 - min]);
		strcpy(rel_text, rels[1]);
		hour = hour+1;
		
	// half past
	} else if (min == 30) {
		strcpy(rel_text, rels[2]);
		hour = hour+1;
		
	// 1..14 minutes past half
	} else if ((min > 30) && (min < 45)) {
		strcpy(minute_text, numbers[min - 31]);
		strcpy(rel_text, rels[3]);
		hour = hour+1;
		
	// 14..1 minutes before
	} else if (min >= 45) {
		strcpy(minute_text, numbers[59 - min]);
		strcpy(rel_text, rels[4]);
		hour = hour+1;
	}
		
	strcpy(hour_text, numbers[(hour % 12) - 1]);
			   
	text_layer_set_text(&minute_layer, minute_text);
	text_layer_set_text(&rel_layer, rel_text);
	text_layer_set_text(&hour_layer, hour_text);			   
	
}

	
/**
 * Initialize Application
 */
void handle_init(AppContextRef ctx) {
	
	ResHandle light_font_r;
	ResHandle bold_font_r;
	
	// Load resources
	resource_init_current_app(&APP_RESOURCES);
	
	// Load Fonts	
	light_font_r = resource_get_handle(RESOURCE_ID_FONT_OPENSANS_REGULAR_32);
	bold_font_r = resource_get_handle(RESOURCE_ID_FONT_OPENSANS_BOLD_32);
	light_font = fonts_load_custom_font(light_font_r);
	bold_font = fonts_load_custom_font(bold_font_r);
	
  	window_init(&window, "Hest Al Heurt");
  	window_stack_push(&window, true /* Animated */);
	window_set_background_color(&window, GColorBlack);
	
	text_layer_init(&minute_layer, GRect(6, 25, 138, 40));
  	text_layer_set_text_alignment(&minute_layer, GTextAlignmentLeft);
	text_layer_set_text_color(&minute_layer, GColorWhite);
	text_layer_set_background_color(&minute_layer, GColorClear);
  	text_layer_set_text(&minute_layer, "");
  	text_layer_set_font(&minute_layer, light_font);
  	layer_add_child(&window.layer, &minute_layer.layer);
	
	text_layer_init(&rel_layer, GRect(6, 55, 138, 40));
  	text_layer_set_text_alignment(&rel_layer, GTextAlignmentLeft);
	text_layer_set_text_color(&rel_layer, GColorWhite);
	text_layer_set_background_color(&rel_layer, GColorClear);
  	text_layer_set_text(&rel_layer, "");
  	text_layer_set_font(&rel_layer, light_font);
  	layer_add_child(&window.layer, &rel_layer.layer);
	
	text_layer_init(&hour_layer, GRect(6, 85, 138, 40));
  	text_layer_set_text_alignment(&hour_layer, GTextAlignmentLeft);
	text_layer_set_text_color(&hour_layer, GColorWhite);
	text_layer_set_background_color(&hour_layer, GColorClear);
  	text_layer_set_text(&hour_layer, "");
  	text_layer_set_font(&hour_layer, bold_font);
  	layer_add_child(&window.layer, &hour_layer.layer);
	
}

/**
 * Minute tick handler
 **/
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent* e) {
	update_time(e->tick_time);
}

/**
 * Shut down the application
 */
void handle_deinit(AppContextRef ctx) {
    fonts_unload_custom_font(light_font);
	fonts_unload_custom_font(bold_font);
}

/**
 * Pebble Starting Point
 **/
void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.deinit_handler = &handle_deinit,
	.tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
