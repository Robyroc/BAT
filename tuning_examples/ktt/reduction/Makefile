CC=g++
KTT_PATH?=/usr/local/KTT
KTT_INCLUDE?=$(KTT_PATH)/source/
KTT_LIBRARY?=$(KTT_PATH)/build/x86_64_Release

default:
	@echo "\033[93m[i]\033[0m Compiling... 🔨"
	@$(CC) ../ktt_json_saver.cpp reduction_tuner.cpp -o reduction -I .. -I $(KTT_INCLUDE) -Wl,-rpath $(KTT_LIBRARY) -L $(KTT_LIBRARY) -lktt
	@echo "\033[92m[✓]\033[0m Done compiling program! 🎉"

clean:
	@echo "\033[93m[i]\033[0m Cleaning up... 🗑️"
	@$(RM) reduction
	@echo "\033[92m[✓]\033[0m Done cleaning up! 🎉"
