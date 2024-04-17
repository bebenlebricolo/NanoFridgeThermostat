This folder is used to map files that use Avr code directly, and hence are more difficult to Unit test as they would require proper stubbing from the AVR world.

This is a sort of HAL (Hardware Abstraction Library) and thus is really tied to avr architecture.

Note : Atmega328pxx devices *have* "MUL", MULS, MULSU, FMUL, FMULS, FMULSU instructions ! No need to worry much about multiplication cycles !
However, they don't have division instructions -> will loop and decrement until reaching the right criteria.