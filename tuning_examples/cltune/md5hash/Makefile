CC=nvcc
CLTUNE_PATH?=/usr/local/CLTune
CLTUNE_INCLUDE?=$(CLTUNE_PATH)/include
CLTUNE_LIBRARY?=$(CLTUNE_PATH)/build

default:
	@echo "\033[93m[i]\033[0m Compiling... 🔨"
	@$(CC) ../cltune_json_saver.cpp md5hash_tuner.cpp -o md5hash -I .. -I $(CLTUNE_INCLUDE) -Xlinker=-rpath,$(CLTUNE_LIBRARY)  -L $(CLTUNE_LIBRARY) -lcltune
	@echo "\033[92m[✓]\033[0m Done compiling program! 🎉"

clean:
	@echo "\033[93m[i]\033[0m Cleaning up... 🗑️"
	@$(RM) md5hash
	@echo "\033[92m[✓]\033[0m Done cleaning up! 🎉"
