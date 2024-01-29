# Raspberry Pi Pico EEPROM Programmer

## Assembly

Each component should be laid out as in the picture below:

![programmer reference picture](schematics/reference.jpg)

The wires are intentionally curved so that the further wires don't block the holes for the closer ones.
For detailed wire connections, reference the [schematic diagram](schematics/eeprom_programmer.pdf).

The EEPROM should be inserted with its notch facing the Pico,
but with a little bit of space between the level shifter and the edge of the EEPROM,
so that removing the EEPROM after programming is easier to do without damaging the pins.

## Usage

Insert the EEPROM and then plug the Pico into your computer.
The serial port for the Pico can be found in Device Manager on Windows
or by looking at the output of `ls /dev/tty` on MacOS/Linux.

Invoke the `eeprom.py` script with the appropriate file and serial port:

```bash
# Windows
python eeprom.py asm.bin COMnumberhere

# Unix
python3 eeprom.py asm.bin /dev/name-of-port
```

The `deploy` command from the [debugger](https://github.com/SuperTails/byobc-debugger/)
will generate the asm.bin for you and place it in the root folder of the debugger repo.
