#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <elf.h>
#include <string.h>

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */
#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

static void check_for_shared_object(Elf64_Ehdr *ehdr);
static void fail(char *reason, int err_code);
static Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name);
static Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx);

int main(int argc, char **argv) {
  int fd;
  size_t len;
  void *p;
  Elf64_Ehdr *ehdr;

  if (argc != 2)
    fail("expected one file on the command line", 0);

  /* Open the shared-library file */
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    fail("could not open file", errno);

  /* Find out how big the file is: */
  len = lseek(fd, 0, SEEK_END);

  /* Map the whole file into memory: */
  p = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  if (p == (void*)-1)
    fail("mmap failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Add a call to your work here */
  
 // Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");

  // Test code that prints section names
  /*{
    Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
    char *strs = (void*)ehdr+shdrs[ehdr->e_shstrndx].sh_offset;
    int i;
    for (i = 0; i < ehdr->e_shnum; i++){
      printf("%s\n", strs + shdrs[i].sh_name);
    }
  }*/

  // Point towards the Dynamic Symbol Section Header
  {
    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym *dynsym_syms = AT_SEC(ehdr, dynsym_shdr);
    char *dynsym_strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
    int i, dynsym_count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
    for (i = 0; i < dynsym_count; i++){
      if ((ELF64_ST_TYPE(dynsym_syms[i].st_info) == STT_FUNC) && (ELF64_ST_TYPE(dynsym_syms[i].st_size) != 0)){
        printf("%s\n", dynsym_strs + dynsym_syms[i].st_name);
        
        // Point to .text section header
        Elf64_Shdr *text_shdr = section_by_name(ehdr, ".text");
        //Elf64_Sym *text_syms = AT_SEC(ehdr, text_shdr);
        unsigned char *machine_code = AT_SEC(ehdr, text_shdr) + (dynsym_syms[i].st_value - text_shdr->sh_addr);
        int k, text_count = dynsym_syms[i].st_size;
         /*for(k = 0; k < text_count; k++){
          // Varibles for function machine code

          switch(machine_code[k]){
            
            case 0xc3:{ // Return instruction
              break;
            }//End of 0xc3 case

            case 0xe9:{ // Jump instruction
              int jump_displacement;
              memcpy((&jump_displacement),(&machine_code[k+1]),sizeof(int));
              Elf64_Shdr *jump_address = (AT_SEC(ehdr, machine_code[k+i]) + jump_displacement);
              break;
            }//End of 0xe9 case

            case 0xeb:{
              printf("  %s\n", "0xeb case!");
              break;
            }//End of 0xeb case

            case 0x48:{
              if(machine_code[k+1] == 0x8b){
                printf("  %s\n", "0x48 0x8b case!");
                break;
              }
            }//End of 0x48 case
          }//End of switch statement
        }// End of for loop */       
      }// End of if statement
    }//End of for loop
  }//End of enclosed block of code


  return 0;
}

static void check_for_shared_object(Elf64_Ehdr *ehdr) {
  if ((ehdr->e_ident[EI_MAG0] != ELFMAG0)
      || (ehdr->e_ident[EI_MAG1] != ELFMAG1)
      || (ehdr->e_ident[EI_MAG2] != ELFMAG2)
      || (ehdr->e_ident[EI_MAG3] != ELFMAG3))
    fail("not an ELF file", 0);

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    fail("not a 64-bit ELF file", 0);
  
  if (ehdr->e_type != ET_DYN)
    fail("not a shared-object file", 0);
}

static void fail(char *reason, int err_code) {
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}

static Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name){
    Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
    char *strs = (void*)ehdr+shdrs[ehdr->e_shstrndx].sh_offset;
    int i;
    for (i = 0; i < ehdr->e_shnum; i++){
      if (strcmp(name, strs + shdrs[i].sh_name) == 0){
        Elf64_Shdr *result = (&shdrs[i]);
        return result;
        break;
      }
    }

}
