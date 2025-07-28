# TC375 AURIX Project

A multi-core embedded project for the Infineon AURIX TC375 microcontroller using the iLLD (Infineon Low Level Driver) framework.

## Overview

This project implements a basic multi-core application for the TC375 microcontroller with three CPU cores:
- **CPU0**: Main application core
- **CPU1**: Secondary processing core  
- **CPU2**: Additional processing core

## Project Structure

```
src/
├── Cpu0_Main.c          # Main application code for CPU0
├── Cpu1_Main.c          # Secondary core code for CPU1
├── Cpu2_Main.c          # Additional core code for CPU2
├── Configurations/      # Generated configuration files
├── Libraries/           # iLLD libraries and services
│   ├── iLLD/           # Infineon Low Level Drivers
│   ├── Service/        # Service layer components
│   └── Infra/          # Infrastructure components
├── Lcf_Gnuc_Tricore_Tc.lsl    # GNU linker script
└── Lcf_Tasking_Tricore_Tc.lsl # TASKING linker script
```

## Features

- **Multi-core synchronization**: All three cores are synchronized using events
- **Watchdog management**: Individual watchdog timers for each core (currently disabled)
- **Interrupt handling**: Interrupts enabled on all cores
- **iLLD framework**: Uses Infineon's Low Level Driver libraries

## Prerequisites

### Required Software
- **AURIX Development Studio (ADS)** - IDE for AURIX development
- **TASKING VX-toolset** or **GNU ARM Embedded Toolchain** - Compiler toolchain
- **Infineon iLLD Libraries** - Low-level driver libraries (included)

### Hardware
- **Infineon AURIX TC375** microcontroller
- **Development board** with TC375 (e.g., AURIX TC375 Lite Kit)
- **Debug probe** (e.g., MiniWiggler, DAS, or J-Link)

## Building the Project

### Using AURIX Development Studio (ADS)
1. Open AURIX Development Studio
2. Import the project: `File > Import > Existing Projects into Workspace`
3. Select the project directory
4. Build the project: `Project > Build All`

### Using Command Line
```bash
# Navigate to project directory
cd src/

# Build using TASKING toolchain
make -f Makefile

# Or build using GNU toolchain
make -f Makefile.gnu
```

## Configuration

### Pin Configuration
The project includes a pin configuration file:
- `src/Libraries/pinmapper.pincfg` - Pin mapping configuration

### Linker Scripts
Two linker scripts are provided:
- `Lcf_Gnuc_Tricore_Tc.lsl` - For GNU toolchain
- `Lcf_Tasking_Tricore_Tc.lsl` - For TASKING toolchain

## Multi-Core Architecture

### CPU0 (Main Core)
- Primary application core
- Handles main system initialization
- Coordinates with other cores via synchronization events

### CPU1 (Secondary Core)
- Secondary processing core
- Synchronized with main core
- Can handle parallel processing tasks

### CPU2 (Additional Core)
- Third processing core
- Synchronized with other cores
- Available for additional parallel processing

## Development Notes

### Watchdog Timers
⚠️ **Warning**: Watchdog timers are currently disabled in all cores. For production use:
- Enable watchdog timers
- Implement proper watchdog service routines
- Consider safety requirements

### Interrupts
- All cores have interrupts enabled
- Implement interrupt service routines as needed
- Consider interrupt priority levels

### Memory Management
- Each core has dedicated memory regions
- Shared memory areas available for inter-core communication
- Stack and heap configurations in linker scripts

## Debugging

### Debug Configuration
1. Connect debug probe to development board
2. Configure debug settings in ADS
3. Set breakpoints in individual core files
4. Use multi-core debugging features

### Common Debug Issues
- **Synchronization**: Ensure all cores reach sync points
- **Memory**: Check stack/heap allocation
- **Interrupts**: Verify interrupt vector table setup

## License

This project uses the Boost Software License - Version 1.0. See individual source files for license details.

## Contributing

1. Follow the existing code style
2. Add proper documentation for new features
3. Test on actual hardware when possible
4. Update this README for significant changes

## Support

For issues and questions:
- Check Infineon AURIX documentation
- Review iLLD library documentation
- Consult AURIX Development Studio help

## Version 
- **v1.0.0**: Initial multi-core setup with basic synchronization
- Basic watchdog and interrupt configuration
- iLLD framework integration