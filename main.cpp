//typedef struct {
//  uint32_t registers[REGISTERS_COUNT];
//  uint32_t eflags;
//  uint8_t* memory;
//  uint32_t eip;
//} Emulator;

#include <vector>

class Emulator {
public:
  std::vector <uint32_t> registers;
  uint32_t eflags;
  uint8_t* memory;
  uint32_t eip;

private:
  size_t size;
  uint32_t eip;
  uint32_t esp;

  Emulator() {
    size = 0;
    registers(REGISTERS_COUNT,0);
  }

  Emulator(size_t size, uint32_t eip_arg, uint32_t esp_arg) {
    memory = new uint8_t[size];
    eip = eip_arg;
    registers[ESP] = esp;
  }

  ~Emulator() {
    delete[] memory;
  }
};

void mov_r32_imm32(Emulator emu) {
  uint8_t reg = get_code8(emu, 0) - 0xB8;
  uint32_t value = get_code32(emu, 1);
  emu.registers[reg] = value;
  emu.eip += 5;
}

void short_jump(Emulator emu) {
  int8_t diff = get_sign_code8(emu, 1);
  emu.eip += (diff + 2);
}

typedef void instruction_func_t(Emulator);


int main(int argc, char* argv[]) {
  FILE* binary;
  if (argc != 2) {
    printf("usage:px86 filename\n");
  }
  Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);

  binary = fopen(argv[1], "rb");
  if (binary == nullptr) {
    printf("%s cannnot open file\n", argv[1]);
  }
  fread(emu.memory,1, 0x200, binary);
  fclose(binary);
}
