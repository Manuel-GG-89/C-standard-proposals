# Proposal for Result Type in Standard C

This is a proposal to implement the type result in the language C standard, whose purpose is to grant a safe way to handle results produced by fucnions.

The main characteristic is that when using this type, the result is automatically released from the memory when leaving the context of the function where an instance is believed.

If used directly in the Main function, the result (result) will be released at the end of the program, but if used within another function, the result will be released when leaving the context of said function.

The implementation is made by a simple combination of macros, in addition to using the predefined macro __attribute__ available by the GCC and CLANG compiler.

Consider that I do not have much experience in C language, but in other languages, on which I based to try a clean implementation of this characteristic.In addition, I leaned with Copilot, who usually used it as a guide of references and errors explanations (... do not expect Copilot to give them an accurate response in the first instruction, in the end, you must reach the final solution for yourself by yourself, and that requires more than a couple of instructions in the prompt).

I have not yet tried this characteristic, but I will try to use it in Toretto, a fork from Tore app, created by Tsoding.

Check the result_type_c_standard_gc.c to see details and examples.