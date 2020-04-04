BOOTMAGIC_ENABLE = no
MOUSEKEY_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MIDI_ENABLE = no  # MIDI controls
AUDIO_ENABLE = no  # Audio output on port C6
UNICODE_ENABLE = no  # Unicode
BLUETOOTH_ENABLE = no  # Enable Bluetooth with the Adafruit EZ-Key HID
RGBLIGHT_ENABLE = no  # Enable WS2812 RGB underlight.
SWAP_HANDS_ENABLE = no  # Enable one-hand typing

RGBLIGHT_ENABLE = yes
TAP_DANCE_ENABLE = yes
OLED_DRIVER_ENABLE = yes

# If you want to change the display of OLED, you need to change here
SRC +=  ./lib/glcdfont.c \
	./lib/rgb_state_reader.c \
	./lib/layer_state_reader.c \
	./lib/logo_reader.c \
	./lib/keylogger.c \
#        ./lib/mode_icon_reader.c \
#        ./lib/host_led_state_reader.c \
#        ./lib/timelogger.c \
