# R.I.P Pelly : 26.1.2023 - 23.2.2023

## Pelly

16 bit operating system

Named after [river Pelly](https://www.google.com/maps/place/Pelly+River/@62.294708,-134.7159487,8z/data=!3m1!4b1!4m5!3m4!1s0x5150567ab729172f:0x2429c8233184845e!8m2!3d62.285926!4d-133.6080992) near [Pelly Crossing](https://www.google.com/maps/place/Pelly+Crossing,+YT,+Canada/@62.8257353,-136.5785531,14.33z/data=!4m5!3m4!1s0x514e2eec14cd51e1:0x3a02c1697ff0e317!8m2!3d62.817979!4d-136.568795)

## Compile Pelly

### Required packages

| Package: | Command:                                                                                                                            |
| -------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| nasm     | <table><tr><th>Debian</th><th>Arch Linux</th></tr><td>`sudo apt-get install nasm`</td><td>`sudo pacman -S nasm`</td></tr></table>   |
| qemu     | <table><tr><th>Debian</th><th>Arch Linux</th></tr><td>`sudo apt-get install qemu`</td><td>`sudo pacman -S qemu`</td></tr></table>   |
| wcc      | Download `2023-02-01` version of `Open Watcom V2.0` (include 16 bit compilers, disable target OS and set host OS to be `linux x86`) |

_Only `i386` version is required for Qemu._

_Recommend to install desktop version of Qemu because it does not require to use VNC clients (it is called `qemu-desktop` on Arch Linux)_

### Compile

## TODO

- [X] Bootloader
  - [X] FAT12
- [ ] Kernel
  - [ ] GDT
  - [ ] IDT
  - [X] PS/2 keyboard (BIOS Int. 16h)
  - [ ] PS/2 mouse
  - [ ] GUI
    - [ ] Basic windows
    - [ ] Taskbar
    - [ ] Rounded windows (optional feature)


## Credits
- [Nanobyte](https://www.youtube.com/@nanobyte-dev)
- [Moving a cursor with the keyboard in assembly - Code Review](https://codereview.stackexchange.com/questions/156257/moving-a-cursor-with-the-keyboard-in-assembly)
