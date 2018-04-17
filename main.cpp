#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string.h>
#include <array>

//const int REGISTERS_COUNT = 8;
const int MEMORY_SIZE = 1024 * 1024;
enum Register { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, REGISTERS_COUNT };
//std::registers_name<int, 4> ar{1, 2, 3, 4};
std::vector<std::string> registers_name{"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
//#define REGISTERS_COUNT 8
//#define MEMORY_SIZE 10240

class Emulator {

private:
  //const int REGISTERS_COUNT = 8;
  //const int MEMORY_SIZE = 10240;
  size_t size;
  //uint32_t eip;
  uint32_t esp;

public:
  std::vector <uint32_t> registers;
  uint32_t eflags;
  uint8_t* memory;
  uint32_t eip;

  Emulator() {
    size = 0;
    registers.resize(REGISTERS_COUNT);
    fill(registers.begin(), registers.end(), 0);
  }

  Emulator(size_t size, uint32_t eip_arg, uint32_t esp_arg) {
    memory = new uint8_t[size];
    eip = eip_arg;
    registers.resize(REGISTERS_COUNT);
    fill(registers.begin(), registers.end(), 0);
    registers[ESP] = esp;
  }

  //~Emulator() {
  //  delete[] memory;
  //}
};

uint32_t get_code8(Emulator emu, int index) {
  return emu.memory[emu.eip + index];
}

uint32_t get_code32(Emulator emu, int index) {
  uint32_t ret = 0;
  for (int i = 0; i < 4; i++) {
    ret |= get_code8(emu, index +i) << (i*8);
  }
  return ret;
}

uint32_t get_sign_code8(Emulator emu, int index) {
  return static_cast<int8_t>(emu.memory[emu.eip + index]);
}

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

void dump_registers(Emulator emu) {
  for (int i = 0; i < REGISTERS_COUNT; i++) {
    printf("%s = 0x%08x\n",registers_name[i].c_str(), emu.registers[i]);
  }
  //printf("EIP = 0x%08x\n",emu.eip);
}

typedef void instruction_func_t(Emulator);
instruction_func_t* instructions[256];

void init_instructions(void) {
  memset(instructions, 0, sizeof(instructions));
  for (int i = 0; i < 8; i++) {
    instructions[0xB8 + i ] = mov_r32_imm32;
  }
  instructions[0xEB] = short_jump;
}

int main(int argc, char* argv[]) {
  FILE* binary;
  if (argc != 2) {
    printf("usage:px86 filename\n");
    return 1;
  }
  Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);

  binary = fopen(argv[1], "rb");
  if (binary == nullptr) {
    printf("%s cannnot open file\n", argv[1]);
    return 1;
  }
  fread(emu.memory,sizeof(uint8_t), 0x200, binary);
  fclose(binary);
  printf("memory:%x\n",emu.memory[0]);
  printf("memory:%x\n",emu.memory[1]);
  printf("%x\n",0x200);
  init_instructions();
  while(emu.eip < MEMORY_SIZE) {
    uint8_t code = get_code8(emu, 0);
    printf("code:%x",code);
    if (instructions[code] == nullptr) {
      printf("\n\nNot Implemented: %x\n",code);
      break;
    }
    instructions[code](emu);
    if (emu.eip == 0x00) {
      printf("\n\n end of program.\n\n");
      break;
    }
  }
  dump_registers(emu);
}
