#include <stdio.h>

int
main(int argc, char *argv[]) {
  printf("Boot Loader Installer for µOS (Micro Operating System)\n");
  printf("Copyright (C) raodm@muohio.edu\n");
  if (argc != 3) {
    printf("Usage: blinstall [src] [dest]     where\n");
    printf("*  [src]  Is the file that contains the boot loader.\n");
    printf("          (Note that this file must be 512 bytes in size).\n");
    printf("*  [dest] Is the target floppy image to with the boot loader ");
    printf("must be written.\n");
    return 1;
  }

  // Ensure that the [src] file is valid and can be opened.
  FILE *bl = fopen(argv[1], "rb");
  if (bl == NULL) {
    // Error opening boot loader binary file.
    printf("* Error: Unable to read boot loader code from '%s'\n", argv[1]);
    return 2;
  }
  // Read 512 bytes from the boot loader file into memory.
  char bootLoader[512];
  if (fread(bootLoader, 1, sizeof(bootLoader), bl) != sizeof(bootLoader)) {
    // Error reading boot loader information from the file.
    printf("* Error: Unable to read %d bytes from '%s'\n", sizeof(bootLoader),
	   argv[1]);
    return 3;
  }
  fclose(bl);
  
  // Check to see if the floppy disk can be opened properly.
  FILE *floppy = fopen(argv[2], "r+b");
  if (floppy == NULL) {
    // Error opening floppy file for I/O
    printf("* Error: Unable to access '%s'\n", argv[2]);
    printf("*      : (If it is a floppy check if floppy is inserted)\n");
    return 3;
  }

  // Read current sector 0 from the floppy disk.
  char bootSector[512];
  if (fread(bootSector, 1, sizeof(bootSector), floppy) != sizeof(bootSector)) {
    // Error reading boot loader information from the file.
    printf("* Error: Unable to read %d bytes from '%s'\n", sizeof(bootSector),
	   argv[2]);
    return 4;
  }
  // Copy necessary BPB information from old boot sector to the boot
  // loader at the appropriate spot.
  int i;
  for(i = 11; (i < 62); i++) {
    bootLoader[i] = bootSector[i];
  }

  // Now seek back to the original starting point and write the
  // bootLoader out to the device.
  fseek(floppy, 0, SEEK_SET);
  if (fwrite(bootLoader, 1, sizeof(bootLoader), floppy) != sizeof(bootLoader)){
    printf("* Error: Unable to write boot loader to '%s'\n", argv[2]);
    return 5;
  }
  
  fclose(floppy);
  printf("Boot loader in %s written to Sector 0 on %s\n", argv[1], argv[2]);
  return 0;
}
