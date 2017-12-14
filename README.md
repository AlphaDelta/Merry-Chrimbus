# 🎄 Merry Chrimbus!
A colourful Chrimbus tree right in your very own CMD

# Overview
I wrote this yesterday in C#, then decided to try and rewrite it in C.

The tree is generated from a constant height variable and coloured lights are added that flash with colours outside of Windows' cmd colour palette (Pink, orange, every coloured light is outside of the palette).

# Compiling
This is a Windows only program.

It can be compiled very simply with MingW GCC like so: `gcc -o tree.exe tree.c -m32 -s`

As well as TCC with `tcc -o tree.exe tree.c` providing you add the following lines to `tcc\lib\kernel32.def`:

```
GetConsoleScreenBufferInfoEx
SetConsoleScreenBufferInfoEx
```

Compressing the TCC result with UPX results in a 3.50 KB executable I assume is aligned by 512 B.

---

❤ Merry Chrimbus ❤